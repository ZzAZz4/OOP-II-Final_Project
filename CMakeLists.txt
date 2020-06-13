cmake_minimum_required(VERSION 3.14)
project(OOP_II_Final_Project)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(Headers)

add_executable(OOP_II_Final_Project
        main.cpp
        Headers
        Definitions
        Battleship.h)
