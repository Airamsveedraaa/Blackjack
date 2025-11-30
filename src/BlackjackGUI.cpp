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
    if (!font.openFromFile("C:/Windows/Fonts/seguisym.ttf")) {
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

    // Botones de apuesta
    bet10Button.setSize(sf::Vector2f(80, 50));
    bet10Button.setPosition(sf::Vector2f(350, 380));
    bet10Button.setFillColor(sf::Color(100, 100, 255));

    bet50Button.setSize(sf::Vector2f(80, 50));
    bet50Button.setPosition(sf::Vector2f(460, 380));
    bet50Button.setFillColor(sf::Color(100, 100, 255));

    bet100Button.setSize(sf::Vector2f(80, 50));
    bet100Button.setPosition(sf::Vector2f(570, 380));
    bet100Button.setFillColor(sf::Color(100, 100, 255));

    clearBetButton.setSize(sf::Vector2f(80, 50));
    clearBetButton.setPosition(sf::Vector2f(680, 380));
    clearBetButton.setFillColor(sf::Color(255, 100, 100));

    dealButton.setSize(sf::Vector2f(150, 60));
    dealButton.setPosition(sf::Vector2f(565, 480));
    dealButton.setFillColor(sf::Color(100, 200, 100));
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

        if (const auto* mouseClick=event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseClick->button==sf::Mouse::Button::Left) {
                handleClick();
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
    title.setPosition(sf::Vector2f(420, 100));
    window.draw(title);

    // Mostrar dinero actual
    sf::Text moneyText(font);
    moneyText.setString("Money: $" + std::to_string(game->getPlayerMoney()));
    moneyText.setCharacterSize(35);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setPosition(sf::Vector2f(500, 200));
    window.draw(moneyText);

    // Mostrar apuesta actual
    sf::Text betText(font);
    betText.setString("Current Bet: $" + std::to_string(currentBet));
    betText.setCharacterSize(30);
    betText.setFillColor(sf::Color::White);
    betText.setPosition(sf::Vector2f(490, 280));
    window.draw(betText);

    // Dibujar botones de apuesta
    drawButton(bet10Button, "+$10");
    drawButton(bet50Button, "+$50");
    drawButton(bet100Button, "+$100");
    drawButton(clearBetButton, "CLEAR");

    // Botón DEAL (solo activo si hay apuesta)
    if (currentBet > 0) {
        drawButton(dealButton, "DEAL");
    } else {
        // Dibujar deshabilitado
        sf::RectangleShape disabledBtn = dealButton;
        disabledBtn.setFillColor(sf::Color(80, 80, 80));
        window.draw(disabledBtn);

        sf::Text btnText(font);
        btnText.setString("DEAL");
        btnText.setCharacterSize(24);
        btnText.setFillColor(sf::Color(120, 120, 120));

        sf::FloatRect textBounds = btnText.getLocalBounds();
        sf::Vector2f btnPos = dealButton.getPosition();
        sf::Vector2f btnSize = dealButton.getSize();

        float textX = btnPos.x + (btnSize.x - textBounds.size.x) / 2.0f - textBounds.position.x;
        float textY = btnPos.y + (btnSize.y - textBounds.size.y) / 2.0f - textBounds.position.y;
        btnText.setPosition(sf::Vector2f(textX, textY));

        window.draw(btnText);
    }
}

void BlackjackGUI::drawGameScreen() {
    // Título
    sf::Text title(font);
    title.setString("BLACKJACK");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(520, 20));
    window.draw(title);

    // Etiqueta Dealer
    sf::Text dealerLabel(font);
    dealerLabel.setString("DEALER");
    dealerLabel.setCharacterSize(25);
    dealerLabel.setFillColor(sf::Color::Yellow);
    dealerLabel.setPosition(sf::Vector2f(580, 70));
    window.draw(dealerLabel);

    // Dibujar mano del dealer (primera carta oculta)
    drawDealerHand(true);

    // Etiqueta Player
    sf::Text playerLabel(font);
    playerLabel.setString("PLAYER");
    playerLabel.setCharacterSize(25);
    playerLabel.setFillColor(sf::Color::Yellow);
    playerLabel.setPosition(sf::Vector2f(575, 420));
    window.draw(playerLabel);

    // Dibujar mano del jugador
    drawPlayerHand();

    // Mostrar dinero y apuesta
    sf::Text moneyText(font);
    moneyText.setString("Money: $" + std::to_string(game->getPlayerMoney()));
    moneyText.setCharacterSize(25);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setPosition(sf::Vector2f(50, 50));
    window.draw(moneyText);

    sf::Text betText(font);
    betText.setString("Bet: $" + std::to_string(game->getCurrentBet()));
    betText.setCharacterSize(25);
    betText.setFillColor(sf::Color::White);
    betText.setPosition(sf::Vector2f(50, 90));
    window.draw(betText);

    // Dibujar botones
    drawButton(hitButton, "HIT");
    drawButton(standButton, "STAND");

    // Double solo si tiene 2 cartas y suficiente dinero
    if (game->getPlayer().getHandSize() == 2 && game->getPlayerMoney() >= game->getCurrentBet()) {
        drawButton(doubleButton, "DOUBLE");
    }
}

void BlackjackGUI::drawResultScreen() {
    // Título
    sf::Text title(font);
    title.setString("ROUND OVER");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(450, 50));
    window.draw(title);

    // Etiqueta Dealer
    sf::Text dealerLabel(font);
    dealerLabel.setString("DEALER");
    dealerLabel.setCharacterSize(25);
    dealerLabel.setFillColor(sf::Color::Yellow);
    dealerLabel.setPosition(sf::Vector2f(580, 120));
    window.draw(dealerLabel);

    // Mostrar todas las cartas del dealer (reveladas)
    drawDealerHand(false);

    // Etiqueta Player
    sf::Text playerLabel(font);
    playerLabel.setString("PLAYER");
    playerLabel.setCharacterSize(25);
    playerLabel.setFillColor(sf::Color::Yellow);
    playerLabel.setPosition(sf::Vector2f(575, 350));
    window.draw(playerLabel);

    // Mostrar cartas del jugador
    drawPlayerHand();

    // Mostrar resultado
    sf::Text resultText(font);
    resultText.setString(resultMessage);
    resultText.setCharacterSize(40);

    // Color según resultado
    if (resultMessage.find("WIN") != std::string::npos ||
        resultMessage.find("BLACKJACK") != std::string::npos) {
        resultText.setFillColor(sf::Color::Green);
    }
    else if (resultMessage.find("BUST") != std::string::npos ||
             resultMessage.find("LOSS") != std::string::npos ||
             resultMessage.find("DEALER WINS") != std::string::npos) {
        resultText.setFillColor(sf::Color::Red);
    }
    else {
        resultText.setFillColor(sf::Color::Yellow);
    }

    // Centrar texto del resultado
    sf::FloatRect textBounds = resultText.getLocalBounds();
    resultText.setPosition(sf::Vector2f(
        640 - textBounds.size.x / 2.0f,
        520
    ));
    window.draw(resultText);

    // Dinero actual
    sf::Text moneyText(font);
    moneyText.setString("Money: $" + std::to_string(game->getPlayerMoney()));
    moneyText.setCharacterSize(30);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setPosition(sf::Vector2f(520, 580));
    window.draw(moneyText);

    // Botón siguiente ronda
    drawButton(nextRoundButton, "NEXT ROUND");
}

void BlackjackGUI::drawPlayerHand() {
    const Player& player = game->getPlayer();
    const auto& hand = player.getHand();

    float startX = 400.0f;
    float y = 450.0f;
    float spacing = 90.0f;

    for (size_t i = 0; i < hand.size(); i++) {
        drawCard(sf::Vector2f(startX + i * spacing, y), hand[i], false);
    }

    // Mostrar valor de la mano
    sf::Text valueText(font);
    valueText.setString("Value: " + std::to_string(player.getHandValue()));
    valueText.setCharacterSize(25);
    valueText.setFillColor(sf::Color::White);
    valueText.setPosition(sf::Vector2f(startX, y + 130));
    window.draw(valueText);
}

void BlackjackGUI::drawDealerHand(bool hideFirst) {
    const Dealer& dealer = game->getDealer();
    const auto& hand = dealer.getHand();

    float startX = 400.0f;
    float y = 100.0f;
    float spacing = 90.0f;

    for (size_t i = 0; i < hand.size(); i++) {
        bool shouldHide = (hideFirst && i == 0);
        drawCard(sf::Vector2f(startX + i * spacing, y), hand[i], shouldHide);
    }

    // Mostrar valor solo si no hay cartas ocultas
    if (!hideFirst) {
        sf::Text valueText(font);
        valueText.setString("Value: " + std::to_string(dealer.getHandValue()));
        valueText.setCharacterSize(25);
        valueText.setFillColor(sf::Color::White);
        valueText.setPosition(sf::Vector2f(startX, y - 40));
        window.draw(valueText);
    }
}

void BlackjackGUI::drawCard(sf::Vector2f position, const Card& card, bool hidden) {
    // Rectángulo de la carta
    sf::RectangleShape cartaRect(sf::Vector2f(80, 110));
    cartaRect.setPosition(position);
    cartaRect.setFillColor(sf::Color::White);
    cartaRect.setOutlineColor(sf::Color::Black);
    cartaRect.setOutlineThickness(2);
    window.draw(cartaRect);

    if (hidden) {
        // Carta oculta - mostrar patrón
        sf::Text oculto(font);
        oculto.setString("?");
        oculto.setCharacterSize(50);
        oculto.setFillColor(sf::Color::Blue);
        oculto.setPosition(sf::Vector2f(position.x + 25, position.y + 25));
        window.draw(oculto);
    } else {
        // Mostrar rango (arriba izquierda)
        sf::Text rango(font);
        rango.setString(card.getCardString());
        rango.setCharacterSize(30);
        rango.setFillColor(sf::Color::Black);
        rango.setPosition(sf::Vector2f(position.x + 10, position.y + 10));
        window.draw(rango);

        // Mostrar palo (centro)
        sf::Text palo(font);
        palo.setString(card.getSuitString());
        palo.setCharacterSize(40);

        // Color rojo para corazones y diamantes
        if (card.getsuit() == Suit::Hearts || card.getsuit() == Suit::Diamonds) {
            palo.setFillColor(sf::Color::Red);
        } else {
            palo.setFillColor(sf::Color::Black);
        }

        palo.setPosition(sf::Vector2f(position.x + 25, position.y + 50));
        window.draw(palo);
    }
}


void BlackjackGUI::handleClick() {
switch (currentState) {
    case GUIState::MENU:
        if (mouseOver(playButton)) {
            std::cout << "PLAY clicked - Cambiando a BETTING" << std::endl;
            currentState = GUIState::BETTING;
            currentBet = 0;
        }
        break;

    case GUIState::BETTING:// Botón +$10
        if (mouseOver(bet10Button)) {
            if (currentBet + 10 <= game->getPlayerMoney()) {
                currentBet += 10;
                std::cout << "Apuesta: $" << currentBet << std::endl;
            }
        }

        // Botón +$50
        if (mouseOver(bet50Button)) {
            if (currentBet + 50 <= game->getPlayerMoney()) {
                currentBet += 50;
                std::cout << "Apuesta: $" << currentBet << std::endl;
            }
        }

        // Botón +$100
        if (mouseOver(bet100Button)) {
            if (currentBet + 100 <= game->getPlayerMoney()) {
                currentBet += 100;
                std::cout << "Apuesta: $" << currentBet << std::endl;
            }
        }

        // Botón CLEAR
        if (mouseOver(clearBetButton)) {
            currentBet = 0;
            std::cout << "Apuesta limpiada" << std::endl;
        }

        // Botón DEAL
        if (mouseOver(dealButton) && currentBet > 0) {
            if (game->placeBet(currentBet)) {
                std::cout << "Iniciando ronda con apuesta: $" << currentBet << std::endl;
                game->startRound();
                currentState = GUIState::PLAYING;
            }
        }
        break;

    case GUIState::PLAYING:
        // Botón HIT
        if (mouseOver(hitButton)) {
            std::cout << "HIT clicked" << std::endl;
            PlayerActionResult result = game->playerHit();

            if (result == PlayerActionResult::BUST) {
                std::cout << "¡TE PASASTE!" << std::endl;
                // Terminar ronda inmediatamente
                game->playDealerTurn();
                RoundResult roundResult = game->finishRound();
                resultMessage = "YOU BUSTED! DEALER WINS";
                currentState = GUIState::RESULT;
            }
            else if (result == PlayerActionResult::BLACKJACK) {
                std::cout << "¡BLACKJACK! (21)" << std::endl;
            }
        }

        // Botón STAND
        else if (mouseOver(standButton)) {
            std::cout << "STAND clicked" << std::endl;
            game->playerStand();

            // Dealer juega
            game->playDealerTurn();

            // Determinar ganador
            RoundResult roundResult = game->finishRound();

            // Mensaje según resultado
            switch (roundResult.outcome) {
                case RoundResult::Outcome::PLAYER_WIN:
                    resultMessage = "YOU WIN! +$" + std::to_string(roundResult.moneyChange);
                    break;
                case RoundResult::Outcome::DEALER_WIN:
                    resultMessage = "DEALER WINS! -$" + std::to_string(-roundResult.moneyChange);
                    break;
                case RoundResult::Outcome::TIE:
                    resultMessage = "TIE! Push";
                    break;
                case RoundResult::Outcome::PLAYER_BLACKJACK:
                    resultMessage = "BLACKJACK! +$" + std::to_string(roundResult.moneyChange);
                    break;
                case RoundResult::Outcome::DEALER_BLACKJACK:
                    resultMessage = "DEALER BLACKJACK! -$" + std::to_string(-roundResult.moneyChange);
                    break;
                case RoundResult::Outcome::PLAYER_BUST:
                    resultMessage = "BUST! -$" + std::to_string(-roundResult.moneyChange);
                    break;
                case RoundResult::Outcome::DEALER_BUST:
                    resultMessage = "DEALER BUST! +$" + std::to_string(roundResult.moneyChange);
                    break;
                case RoundResult::Outcome::BOTH_BLACKJACK:
                    resultMessage = "BOTH BLACKJACK! Push";
                    break;
            }

            currentState = GUIState::RESULT;
        }

        // Botón DOUBLE
        else if (mouseOver(doubleButton)) {
            if (game->getPlayer().getHandSize() == 2 && game->getPlayerMoney() >= game->getCurrentBet()) {
                std::cout << "DOUBLE clicked" << std::endl;
                PlayerActionResult result = game->playerDoubleDown();

                // Después de double, siempre pasa al dealer
                game->playDealerTurn();
                RoundResult roundResult = game->finishRound();

                if (result == PlayerActionResult::BUST) {
                    resultMessage = "BUSTED AFTER DOUBLE! -$" + std::to_string(-roundResult.moneyChange);
                } else {
                    switch (roundResult.outcome) {
                        case RoundResult::Outcome::PLAYER_WIN:
                            resultMessage = "DOUBLE WIN! +$" + std::to_string(roundResult.moneyChange);
                            break;
                        case RoundResult::Outcome::DEALER_WIN:
                            resultMessage = "DOUBLE LOSS! -$" + std::to_string(-roundResult.moneyChange);
                            break;
                        default:
                            resultMessage = "DOUBLE TIE! Push";
                            break;
                    }
                }

                currentState = GUIState::RESULT;
            }
        }
        break;
    }
}
