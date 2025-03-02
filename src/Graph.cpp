//
// Created on 25-2-28.
//

#include "Graph.h"

#include <fstream>
#include <vector>

// 用于从文件中读取顶点与边的静态类
class FileReader
{
public:
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

    //为使此相对路径能够正确起作用, 请保证data文件夹位于程序当前工作路径的上一级目录; 或者直接换成你的绝对路径
    static constexpr const char* path_vex = "../data/Vex.txt";
    static constexpr const char* path_edge = "../data/Edge.txt";
};

// 静态哨兵对象, 用于指示查找失败等
const Vex Graph::INVALID_VEX(0x7fffffff);

Graph::Graph()
{
    for (auto& x : FileReader::readVertices())
    {
        insertVertex(x);
    }
    for (auto& x : FileReader::readEdges())
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
