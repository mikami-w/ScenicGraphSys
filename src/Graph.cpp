//
// Created on 25-2-28.
//

#include "Graph.h"

#include <algorithm>
#include <fstream>
#include <queue>
#include <vector>

// 用于从文件中读取顶点与边
namespace GraphIO
{
    //为使此相对路径能够正确起作用, 请保证data文件夹位于程序当前工作路径的上一级目录; 或者直接换成你的绝对路径
    static constexpr const char* path_vex = "../data/Vex.txt";
    static constexpr const char* path_edge = "../data/Edge.txt";

    // 从文件读取顶点信息
    static std::vector<Vex> readVertices(const std::string& path = path_vex)
    {
        std::ifstream fileIn(path);
        if (!fileIn.is_open())
            throw std::runtime_error("Could not open file: " + path);

        int vertexNum = 0;
        fileIn >> vertexNum;

        std::vector<Vex> vertices;
        Vex vex;
        for (int i = 0; i < vertexNum; i++)
        {
            fileIn >> vex.num >> vex.name >> vex.desc;
            vertices.push_back(vex);
        }

        return vertices;
    }

    static std::vector<Edge> readEdges(const std::string& path = path_edge)
    {
        std::ifstream fileIn(path);
        if (!fileIn.is_open())
            throw std::runtime_error("Could not open file: " + path);

        std::vector<Edge> v;
        Edge edge;
        while (fileIn >> edge.vex1 >> edge.vex2 >> edge.weight)
        {
            v.push_back(edge);
        }
        return v;
    }
};

// 静态哨兵对象, 用于指示查找失败等
const Vex Graph::INVALID_VEX(0x7fffffff);

Graph::Graph()
{
    for (auto& x : GraphIO::readVertices())
    {
        insertVertex(x);
    }
    for (auto& x : GraphIO::readEdges())
    {
        insertEdge(x);
    }
}

void Graph::insertVertex(const Vex& vex)
{
    if (vexNum >= MAX_VERTEX_NUM)
    {
        throw std::runtime_error("顶点数超过容量上限...");
    }
    vexes[vexNum++] = vex;
}

void Graph::insertEdge(const Edge& edge)
{
    if (edge.vex1 >= vexNum || edge.vex2 >= vexNum || edge.vex1 < 0 || edge.vex2 < 0)
    {
        // edge中vex下标越界
        throw std::runtime_error("试图插入顶点不存在的边...");
    }
    // 在邻接矩阵中插入边信息
    adjMatrix[edge.vex1][edge.vex2] = edge.weight;
    adjMatrix[edge.vex2][edge.vex1] = edge.weight;
}

const Vex& Graph::getVertex(int num) noexcept
{
    // 一个 Vex 对象也太大了(难视) 所以传常引用, 而不是像操作手册一样返回对象拷贝
    int index = 0;
    for (; index < vexNum; index++)
    {
        if (vexes[index].num == num)
            break;
    }
    if (index == vexNum)
    {
        // 查找失败
        return INVALID_VEX;
    }
    return vexes[index];
}

void Graph::dfs(int vIndex, std::vector<int>& path, std::vector<std::vector<int>>& allPaths, bool* visited)
{
    if (path.size() == vexNum)
    {
        // 结束标志: 访问了所有顶点
        allPaths.push_back(path);
        return;
    }

    visited[vIndex] = true;
    for (int neighbor = 0; neighbor < vexNum; neighbor++)
    {
        if (adjMatrix[vIndex][neighbor] && !visited[neighbor]) //节点相邻且未被访问, 则访问该节点
        {
            path.push_back(neighbor);
            dfs(neighbor, path, allPaths, visited); // 到此为止是找到一条 path 的代码, 下面开始回溯找其他路径
            path.pop_back(); // 退出上面的dfs函数时上面访问的节点被设为了未访问以允许在后续的路线中访问
        }
    }
    visited[vIndex] = false;
}

std::vector<std::vector<int>> Graph::DFSTraverse(int vIndex)
{
    std::vector<std::vector<int>> allPaths;
    std::vector<int> path;
    bool visited[MAX_VERTEX_NUM]{};
    path.push_back(vIndex); // 先存起点!
    dfs(vIndex, path, allPaths, visited);

    return allPaths;
}

std::vector<int> Graph::findShortRoute(int beginIndex, int endIndex)
{
    std::vector<int> route;
    bool visited[MAX_VERTEX_NUM]{};
    int prev[MAX_VERTEX_NUM]{}; // 记录到 i 的路径的前驱
    int dist[MAX_VERTEX_NUM]; // 记录从起点到顶点 i 的当前最短距离
    std::fill(dist, dist + MAX_VERTEX_NUM, INT_MAX);
    dist[beginIndex] = 0;

    struct PathInfo
    {
        int distance;
        int index;
    };

    auto cmp = [](PathInfo& a, PathInfo& b)-> bool { return a.distance > b.distance; };
    std::priority_queue<PathInfo, std::vector<PathInfo>,
                        decltype(cmp)> pq(cmp); // 大根堆
    pq.emplace(dist[beginIndex], beginIndex); // 初始最短距离

    // 因为矩阵中存储的距离为 0 表示无连接, 用这个 lambda 表达式把 0 转换成 0x7fffffff, 同时返回 long long 避免加法导致上溢
    auto infDist = [](int dist)-> long long { return dist == 0 ? INT_MAX : dist; };
    while (!pq.empty())
    {
        int p = pq.top().index; // 目前队列中路径最短的顶点
        pq.pop();

        if (visited[p]) continue;
        visited[p] = true;

        for (int i = 0; i < vexNum; i++)
        {
            // 起点经 p 到达 i 的距离小于目前起点到 i 的距离,
            if (dist[p] + infDist(adjMatrix[p][i]) < dist[i])
            {
                dist[i] = dist[p] + adjMatrix[p][i]; // 更新到 i 的距离
                prev[i] = p; // 标记前驱
                pq.emplace(dist[i], i);
            }
        }
    }

    // 考虑到景区是连通无向图, 没有处理起点与终点不连通的情况
    // vector 在表头插入数据性能很烂, 所以倒序存储最短路线, 最后再翻转
    route.push_back(endIndex);
    for (int i = endIndex; i != beginIndex; i = prev[i])
    {
        route.push_back(prev[i]);
    }
    std::reverse(route.begin(), route.end());

    return route;
}
