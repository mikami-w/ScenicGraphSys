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
