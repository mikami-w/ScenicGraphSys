//
// Created on 25-2-28.
//

#ifndef TOURISM_H
#define TOURISM_H

#include <iostream>

// 清空输入缓冲
inline void clearBuffer()
{
    char ch;
    do
    {
        ch = getchar();
    }
    while (ch != '\n' && ch != EOF); // 控制台接收到 Ctrl+Z 等组合键时 getchar() 将返回 EOF
}

void createGraph();
void getSpotInfo();
void travelPath();
void findShortRoute();
void designPath();

#endif //TOURISM_H
