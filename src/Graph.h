//
// Created on 25-2-28.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <cstring>
#include <tuple>
#include <vector>


constexpr int MAX_VERTEX_NUM = 20;


struct Vex
{
    int num; // 景点编号
    char name[20]{}; // 景点名
    char desc[256]{}; // 景点介绍

    explicit Vex(int _num = 0, const char* _name = "", const char* _desc = ""): num(_num)
    {
        strcpy(name, _name);
        strcpy(desc, _desc);
    }

    Vex(const Vex& _vex): num(_vex.num)
    {
        strcpy(name, _vex.name);
        strcpy(desc, _vex.desc);
    }

    bool operator==(const Vex& vex) const
    {
        // 防止字符串末尾'\0'后的无效数据影响比较, 使用 strcmp 函数而不是默认实现的 memcmp
        return num == vex.num && strcmp(name, vex.name) == 0 && strcmp(desc, vex.desc) == 0;
    }
};

struct Edge
{
    int vex1; // 边的第一个顶点(景点索引)
    int vex2; // 边的第二个顶点(景点索引)
    int weight; // 权值

    explicit Edge(int _vex1 = 0, int _vex2 = 0, int _weight = 0): vex1(_vex1), vex2(_vex2), weight(_weight)
    {
    }
};

// 因为全局只会存在唯一一个Graph对象, 所以使用单例模式
class Graph
{
public:
    static const Vex INVALID_VEX;

    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;

    static Graph& getInstance()
    {
        static Graph instance;
        return instance;
    }

    // 依次返回 邻接矩阵, 顶点信息数组, 当前顶点个数
    std::tuple<const int(*)[MAX_VERTEX_NUM], const Vex*, int> getData()
    {
        return std::make_tuple(adjMatrix, vexes, vexNum);
    }

    void insertVertex(const Vex& vex);
    void insertEdge(const Edge& edge);
    const Vex& getVertex(int num) noexcept; // 查找编号为 num 的顶点并返回, 若查找失败则返回Graph::INVALID_VEX

    // 返回编号为 num 的顶点在 vexex 的索引, 若编号不存在则返回-1
    int getIndex(int num) noexcept
    {
        // 有这个成员函数是因为不敢假设编号与索引相同; 若能保证编号与索引相同则此成员函数多余
        auto& v = getVertex(num);
        return v != INVALID_VEX ? (&v - vexes) : -1;
    }


private:
    Graph();
    ~Graph() = default;

    int adjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM]{}; // 邻接矩阵
    Vex vexes[MAX_VERTEX_NUM]; // 顶点信息数组
    int vexNum = 0; // 当前图的顶点个数
};


#endif //GRAPH_H
