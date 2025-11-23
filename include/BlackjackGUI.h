//
// Created by Airam on 23/11/2025.
//

#ifndef BLACKJACK_BLACKJACKGUI_H
#define BLACKJACK_BLACKJACKGUI_H
#include <SFML/Graphics.hpp>
#include "Game.h"

class BlackjackGUI {
private:
    sf::RenderWindow window;
    sf::Font font;
    Game game;  // Tu clase Game

    // Estados de la GUI
    enum class GUIState {
        MENU,
        BETTING,
        PLAYING,
        RESULT
    };
    GUIState state;

    // Elementos visuales
    vector<sf::RectangleShape> playerCards;
    vector<sf::RectangleShape> dealerCards;
    sf::RectangleShape hitButton;
    sf::RectangleShape standButton;
    sf::RectangleShape doubleButton;

public:
    BlackjackGUI();
    void run();  // Bucle principal

private:
    void handleEvents();
    void update();
    void render();

    void drawCard(sf::Vector2f position, const Card& card);
    void drawButton(const sf::RectangleShape& button, const string& text);
};


#endif //BLACKJACK_BLACKJACKGUI_H