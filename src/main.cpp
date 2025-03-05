#include <iostream>
#include "Tourism.h"
#include "Graph.h"

inline void printMenu()
{
    std::cout
        << "===== 景区信息管理系统 =====\n"
        << "1. 创建景区景点图\n"
        << "2. 查询景点信息\n"
        << "3. 旅游景点导航\n"
        << "4. 搜索最短路径\n"
        << "5. 铺设电路规划\n"
        << "0. 退出\n"
        << std::endl
        << "请输入选项: ";
}

inline char getValidInput()
{
    // 获取合法的菜单项输入
    char selection;
    while (isspace(selection = getchar())); //丢弃开头空白字符并读取第一个字符
    //检查输入，若输入超过2个字符则一定输入错误
    if (getchar() != '\n')
    {
        clearBuffer();
        return 0x7f; //进入if意味着非法输入，返回特殊值0x7f
    }
    return selection;
}

int main()
{
    // 控制台编码使用 UTF-8, 使其与源文件编码相同以正确输出中文字符; 若使用 Visual Studio 一般不存在此问题
    system("chcp 65001 > nul");
    for (;;)
    {
        printMenu();
        switch (getValidInput())
        {
        case '1':
            {
                createGraph();
                break;
            }
        case '2':
            {
                getSpotInfo();
                break;
            }
        case '3':
            {
                travelPath();
                break;
            }
        case '4':
            {
                findShortRoute();
                break;
            }
        case '5':
            {
                break;
            }
        case '0':
            exit(0);
            break;
        default:
            {
                std::cout << "输入的菜单序号错误！\n请重新输入......\n";
                break;
            }
        }
        std::cout << std::endl;
    }
    return 0;
}
