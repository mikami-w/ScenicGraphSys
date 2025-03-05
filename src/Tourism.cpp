//
// Created on 25-2-28.
//

#include "Tourism.h"
#include "Graph.h"

#include <format>
#include <iostream>

void createGraph()
{
    auto& g = Graph::getInstance();
    auto [adjM, vexes, vexNum] = g.getData();
    std::cout
        << "===== 创建景区景点图 =====" << std::endl
        << "顶点数目: " << vexNum << std::endl
        << "----- 顶点 -----" << std::endl;
    // 遍历所有顶点
    for (int i = 0; i < vexNum; i++)
    {
        std::cout << vexes[i].num << '-' << vexes[i].name << std::endl;
    }

    std::cout << "----- 边 -----" << std::endl;
    // 遍历所有边
    for (int i = 0; i < vexNum; i++)
    {
        for (int j = i; j < vexNum; j++)
        {
            if (adjM[i][j])
                std::cout << std::format("<v{}, v{}> {}", i, j, adjM[i][j]) << std::endl;
            // std::format 为 c++20 新增特性, 若使用 C++ 标准版本较低, 可使用 std::cout 手动拼接
        }
    }
}

void getSpotInfo()
{
    auto& g = Graph::getInstance();
    auto [adjM, vexes, vexNum] = g.getData();

    std::cout << "===== 查询景点信息 =====" << std::endl;
    for (int i = 0; i < vexNum; i++)
    {
        std::cout << vexes[i].num << '-' << vexes[i].name << std::endl;
    }
    std::cout << "请输入想要查询的景点编号: ";
    int num = 0; // 要查询的景点编号
    std::cin >> num;
    std::cout << std::endl;

    // 查找相应编号的景点vex索引
    auto& vex_found = g.getVertex(num);
    if (vex_found == Graph::INVALID_VEX)
    {
        // 查找失败
        std::cout << "该编号不存在." << std::endl;
        return;
    }

    std::cout
        << vex_found.name << std::endl
        << vex_found.desc << std::endl;

    std::cout << "----- 周边景区 -----" << std::endl;
    for (int i = 0; i < vexNum; i++)
    {
        if (adjM[g.getIndex(num)][i])
        {
            auto& vex_aside = g.getVertex(i);
            std::cout << vex_found.name << "->" << vex_aside.name << ' ' << adjM[num][i] << "m\n";
        }
    }
}

void travelPath()
{
    auto& g = Graph::getInstance();
    auto [adjM, vexes, vexNum] = g.getData();

    std::cout << "===== 旅游景点导航 =====" << std::endl;
    for (int i = 0; i < vexNum; i++)
    {
        std::cout << vexes[i].num << '-' << vexes[i].name << std::endl;
    }
    std::cout << "请输入起点编号: ";
    int num = 0; // 要查询路线的起点编号
    std::cin >> num;
    std::cout << std::endl;
    int vIndex = g.getIndex(num);
    if (vIndex == -1)
    {
        std::cout << "该编号不存在." << std::endl;
        return;
    }

    std::cout << "导游路线为:" << std::endl;
    auto routes = g.DFSTraverse(vIndex);
    for (int i = 0; i < routes.size(); i++)
    {
        std::cout << "路线" << i + 1 << ": " << vexes[routes[i][0]].name;
        for (int j = 1; j < routes[i].size(); j++)
        {
            std::cout << " -> " << vexes[routes[i][j]].name;
        }
        std::cout << std::endl;
    }
}

void findShortRoute()
{
    auto& g = Graph::getInstance();
    auto [adjM, vexes, vexNum] = g.getData();
    std::cout << "===== 搜索最短路径 =====" << std::endl;
    for (int i = 0; i < vexNum; i++)
    {
        std::cout << vexes[i].num << '-' << vexes[i].name << std::endl;
    }

    int beginNum, endNum;
    int beginIndex, endIndex;
    std::cout << "请输入起点的编号: ";
    std::cin >> beginNum;
    if ((beginIndex = g.getIndex(beginNum)) == -1)
    {
        std::cout << "该编号不存在." << std::endl;
        return;
    }
    std::cout << "请输入终点的编号: ";
    std::cin >> endNum;
    if ((endIndex = g.getIndex(endNum)) == -1)
    {
        std::cout << "该编号不存在." << std::endl;
        return;
    }
    if (beginNum == endNum)
    {
        std::cout << "起点与终点不应相同!" << std::endl;
        return;
    }

    auto route = g.findShortRoute(beginIndex, endIndex);
    std::cout << "最短路线为: ";
    int distance = 0; // 距离值
    std::cout << vexes[route[0]].name;

    for (int i = 1; i < route.size(); i++)
    {
        std::cout << "->" << vexes[route[i]].name;
        // 计算距离
        distance += adjM[route[i - 1]][route[i]];
    }
    std::cout << "\n最短距离为: " << distance << 'm' << std::endl;
}

void designPath()
{
    auto& g = Graph::getInstance();
    auto [adjM, vexes, vexNum] = g.getData();
    std::cout << "===== 铺设电路规划 =====" << std::endl;

    std::cout << "在以下两个景点之间铺设电路: " << std::endl;
    int length = 0;
    auto MSTree = g.findMinimumSpanningTree(0);
    for (auto& e : MSTree)
    {
        std::cout << vexes[e.vex1].name << " - " << vexes[e.vex2].name
            << "  " << adjM[e.vex1][e.vex2] << 'm' << std::endl;
        length += e.weight;
    }
    std::cout << "铺设电路的总长度为: " << length << 'm' << std::endl;
}
