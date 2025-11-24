cmake_minimum_required(VERSION 3.23)
project(Blackjack)

set(CMAKE_CXX_STANDARD 20)

# Buscar SFML 3.0
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

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
        src/BlackjackGUI.cpp
        include/BlackjackGUI.h
)

# Ejecutable GUI
add_executable(Blackjack_GUI
        main_gui.cpp
        src/BlackjackGUI.cpp
        src/Card.cpp
        src/Deck.cpp
        src/Player.cpp
        src/Dealer.cpp
        src/Game.cpp
        include/BlackjackGUI.h
)

# Linkear SFML 3 al ejecutable GUI
target_link_libraries(Blackjack_GUI SFML::Graphics SFML::Window SFML::System)
