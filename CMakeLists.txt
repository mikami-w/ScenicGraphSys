cmake_minimum_required(VERSION 3.30)
project(ScenicGraphSys)

set(CMAKE_CXX_STANDARD 20)

add_executable(ScenicGraphSys src/main.cpp
        src/Graph.cpp
        src/Tourism.cpp
)
