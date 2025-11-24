//
// Created by Airam on 23/11/2025.
//

#include "BlackjackGUI.h"
#include "BlackjackGUI.h"


BlackjackGUI::BlackjackGUI()
:window(sf::VideoMode({1280,720}),"Blackjack"),
currentState(GUIState::MENU),
currentBet(0),
resultMessage("")
{
    window.setFramerateLimit(60);
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente" << std::endl;
        throw std::runtime_error("No se pudo cargar la fuente");
    }

    //crear el game
    game = new Game("Jugador",1000); //nombre y dinero por defecto

    //inicializar botones
    initializeButtons();

    std::cout<<"Blackjack inicializado correctamente"  << std::endl;
}


BlackjackGUI::~BlackjackGUI() {
    delete game;
}


void BlackjackGUI::initializeButtons() {

    // Botón PLAY (menú)
    playButton.setSize(sf::Vector2f(200, 60));
    playButton.setPosition({540, 400});
    playButton.setFillColor(sf::Color(100, 150, 100));

    // Botón HIT
    hitButton.setSize(sf::Vector2f(120, 50));
    hitButton.setPosition({400, 600});
    hitButton.setFillColor(sf::Color(100, 150, 255));

    // Botón STAND
    standButton.setSize(sf::Vector2f(120, 50));
    standButton.setPosition({560, 600});
    standButton.setFillColor(sf::Color(255, 100, 100));

    // Botón DOUBLE
    doubleButton.setSize(sf::Vector2f(120, 50));
    doubleButton.setPosition({720, 600});
    doubleButton.setFillColor(sf::Color(255, 200, 100));

    // Botón NEXT ROUND
    nextRoundButton.setSize(sf::Vector2f(200, 60));
    nextRoundButton.setPosition({540, 500});
    nextRoundButton.setFillColor(sf::Color(100, 200, 100));
}


bool BlackjackGUI::mouseOver(const sf::RectangleShape& rect) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect bounds = rect.getGlobalBounds();
    return bounds.contains(sf::Vector2f(static_cast<float>(mousePos.x),
                                        static_cast<float>(mousePos.y)));
}


void BlackjackGUI::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void BlackjackGUI::handleEvents() {
    while (auto event = window.pollEvent()) {
        if (event ->is<sf::Event::Closed>()) {
            window.close(); //si cierra se cierra la ventana
        }
        if (const auto* keyPress= event->getIf<sf::Event::KeyPressed>()) {
            if (keyPress->code==sf::Keyboard::Key::Escape) {
                window.close();
            }
        }

        //TO DO ---> MANEJO DE CLICKS
    }

}

void BlackjackGUI::update() {
    //TO DO ---> IMPLEMENTARLO
}


void BlackjackGUI::render() {
    window.clear(sf::Color(0,100,0)); //Verde casino


    switch (currentState) {
        case GUIState::MENU:
            drawMenu();
            break;
        case GUIState::BETTING:
            drawBettingScreen();
            break;
        case GUIState::PLAYING:
            drawGameScreen();
            break;
        case GUIState::RESULT:
            drawResultScreen();
            break;
    }
    window.display();
}


void BlackjackGUI::drawButton(const sf::RectangleShape& btn, const std::string& text, sf::Color textColor) {
    // Cambiar color si mouse está encima
    sf::RectangleShape buttonCopy = btn;
    if (mouseOver(btn)) {
        sf::Color hoverColor = btn.getFillColor();
        hoverColor.r = std::min(255, hoverColor.r + 50);
        hoverColor.g = std::min(255, hoverColor.g + 50);
        hoverColor.b = std::min(255, hoverColor.b + 50);
        buttonCopy.setFillColor(hoverColor);
    }

    window.draw(buttonCopy);

    // Dibujar texto centrado
    sf::Text buttonText(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(textColor);

    // SFML 3.0: usar getSize() directamente
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    sf::Vector2f btnPos = btn.getPosition();
    sf::Vector2f btnSize = btn.getSize();

    buttonText.setPosition(sf::Vector2f(
        btnPos.x + (btnSize.x - textBounds.size.x) / 2.0f - textBounds.position.x,
        btnPos.y + (btnSize.y - textBounds.size.y) / 2.0f - textBounds.position.y
    ));

    window.draw(buttonText);
}


void BlackjackGUI::drawMenu() {
    // Título
    sf::Text title(font);
    title.setString("BLACKJACK");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color::White);
    title.setPosition({400, 200});
    window.draw(title);

    // Botón PLAY
    drawButton(playButton, "PLAY");

    // Instrucciones
    sf::Text instructions(font);
    instructions.setString("Press ESC to quit");
    instructions.setCharacterSize(20);
    instructions.setFillColor(sf::Color(200, 200, 200));
    instructions.setPosition({560, 650});
    window.draw(instructions);
}


void BlackjackGUI::drawBettingScreen() {
    // Título
    sf::Text title(font);
    title.setString("PLACE YOUR BET");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(450, 150));
    window.draw(title);

    // Mostrar dinero actual
    sf::Text moneyText(font);
    moneyText.setString("Money: $" + std::to_string(game->getPlayerMoney()));
    moneyText.setCharacterSize(30);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setPosition(sf::Vector2f(520, 250));
    window.draw(moneyText);

    //TO DO ---> Añadir botones para apostar
    sf::Text placeholder(font);
    placeholder.setString("(Betting screen - TODO)");
    placeholder.setCharacterSize(20);
    placeholder.setFillColor(sf::Color(200, 200, 200));
    placeholder.setPosition(sf::Vector2f(500, 400));
    window.draw(placeholder);
}

void BlackjackGUI::drawGameScreen() {
    //TO DO ---> Dibujar cartas y botones
    sf::Text placeholder(font);
    placeholder.setString("(Game screen - TODO)");
    placeholder.setCharacterSize(20);
    placeholder.setFillColor(sf::Color::White);
    placeholder.setPosition(sf::Vector2f(500, 300));
    window.draw(placeholder);
}

void BlackjackGUI::drawResultScreen() {
    // Mostrar resultado
    sf::Text resultText(font);
    resultText.setString(resultMessage);
    resultText.setCharacterSize(50);
    resultText.setFillColor(sf::Color::Yellow);
    resultText.setPosition(sf::Vector2f(400, 250));
    window.draw(resultText);

    // Botón siguiente ronda
    drawButton(nextRoundButton, "NEXT ROUND");

    sf::Text placeholder(font);
    placeholder.setString("(Result screen - TODO)");
    placeholder.setCharacterSize(20);
    placeholder.setFillColor(sf::Color(200, 200, 200));
    placeholder.setPosition(sf::Vector2f(500, 400));
    window.draw(placeholder);
}

void BlackjackGUI::drawPlayerHand() {
    //TO DO ---> Implementar
}

void BlackjackGUI::drawDealerHand(bool hideFirst) {
    //TO DO ---> Implementar
}

void BlackjackGUI::drawCard(sf::Vector2f position, const Card& card, bool hidden) {
    //TO DO ---> Implementar
}


