cmake_minimum_required(VERSION 4.0)
project(Blackjack)

set(CMAKE_CXX_STANDARD 26)

add_executable(Blackjack main.cpp
        src/Card.cpp
        include/Card.h)
