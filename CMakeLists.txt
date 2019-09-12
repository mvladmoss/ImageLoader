cmake_minimum_required(VERSION 3.14)
project(ImageLoader)

set(CMAKE_CXX_STANDARD 11)
link_libraries(ws2_32 wsock32)

add_executable(ImageLoader main.cpp)