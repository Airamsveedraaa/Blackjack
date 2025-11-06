cmake_minimum_required(VERSION 3.23)
project(Blackjack)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(Blackjack main.cpp
        src/Card.cpp
        src/Deck.cpp
        src/Player.cpp
        src/Dealer.cpp
        src/Game.cpp
        include/Deck.h
        include/Player.h
        include/Dealer.h
        include/Game.h
        include/Comun.h)
