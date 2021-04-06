cmake_minimum_required(VERSION 3.15)
project(JB)

set(CMAKE_CXX_STANDARD 14)

add_executable(JB main.cpp MyX11Window.cpp MyX11Window.h)
target_link_libraries(JB X11 GL)
