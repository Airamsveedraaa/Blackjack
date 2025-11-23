cmake_minimum_required(VERSION 3.23)
project(Blackjack)

set(CMAKE_CXX_STANDARD 20)

# Buscar SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(include)

# Ejecutable de CONSOLA (tu juego actual)
add_executable(Blackjack_Console
        main.cpp
        src/Card.cpp
        src/Deck.cpp
        src/Player.cpp
        src/Dealer.cpp
        src/Game.cpp
        include/Deck.h
        include/Player.h
        include/Dealer.h
        include/Game.h
        include/Comun.h
        include/Card.h
)

# Ejecutable GUI (nuevo - para probar SFML)
add_executable(Blackjack_GUI
        main_gui.cpp
)

# Linkear SFML solo al ejecutable GUI
target_link_libraries(Blackjack_GUI sfml-graphics sfml-window sfml-system)