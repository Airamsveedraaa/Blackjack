cmake_minimum_required(VERSION 3.23)
project(Blackjack)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(Blackjack main.cpp
        src/Card.cpp
        src/Deck.cpp
        include/Deck.h
        src/Player.cpp
        include/Player.h
        src/Dealer.cpp
        include/Dealer.h
        include/Comun.h)
