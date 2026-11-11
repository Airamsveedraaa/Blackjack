#ifndef BLACKJACK_BLACKJACKGUI_H
#define BLACKJACK_BLACKJACKGUI_H

#include <SFML/Graphics.hpp>
#include "Game.h"
#include <vector>
#include <string>
#include <memory>
#include <array>

class BlackjackGUI {
private:
    sf::RenderWindow window;
    sf::Font font;
    std::unique_ptr<Game> game;
    
    enum class GUIState {
        MENU,
        BETTING,
        PLAYING,
        RESULT,
        GAME_OVER,
        STATS,
        INSURANCE
    };
    GUIState currentState;
    
    // Botones del juego
    sf::RectangleShape hitButton;
    sf::RectangleShape standButton;
    sf::RectangleShape doubleButton;
    sf::RectangleShape playButton;
    sf::RectangleShape nextRoundButton;
    sf::RectangleShape restartButton;
    sf::RectangleShape statsButton;
    sf::RectangleShape backButton;
    sf::RectangleShape menuButton;
    sf::RectangleShape InsuranceYesButton;
    sf::RectangleShape InsuranceNoButton;

    // Botones de apuesta
    sf::RectangleShape bet10Button;
    sf::RectangleShape bet50Button;
    sf::RectangleShape bet100Button;
    sf::RectangleShape AllInButton;
    sf::RectangleShape clearBetButton;
    sf::RectangleShape dealButton;
    
    int currentBet;
    std::string resultMessage;
    RoundResult::Outcome lastOutcome;
    void handleClick();

    std::array<std::array<sf::Texture,13>,4> cardTextures;
    sf::Texture cardBackTexture;
    static constexpr int CARD_RECT_X = 11;
    static constexpr const int CARD_RECT_Y = 2;
    static constexpr const int CARD_RECT_W = 42;
    static constexpr const int CARD_RECT_H = 60;

public:
    BlackjackGUI();
    void run();
    
private:
    void handleEvents();
    void update();
    void render();
    
    void drawMenu();
    void drawBettingScreen();
    void drawGameScreen();
    void drawResultScreen();
    void drawGameOverScreen();
    void drawStatsScreen();
    void drawInsuranceScreen();
    void drawInsuranceGameScreen();
    void drawStats(float x, float y);
    
    void drawCard(sf::Vector2f position, const Card& card, bool hidden = false);
    void drawButton(const sf::RectangleShape& btn, const std::string& text, sf::Color textColor = sf::Color::White);
    void drawPlayerHand();
    void drawDealerHand(bool hideFirst = true);
    void continueAfterDeal();

    std::string buildResultMessage(const RoundResult& r, bool doubled) const;
    
    bool mouseOver(const sf::RectangleShape& rect) const;
    void initializeButtons();
};

#endif