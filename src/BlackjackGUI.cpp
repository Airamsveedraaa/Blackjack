//
// Created by Airam on 23/11/2025.
//

#include "BlackjackGUI.h"
#include <format>


BlackjackGUI::BlackjackGUI()
:window(sf::VideoMode({1280,720}),"Blackjack"),
currentState(GUIState::MENU),
currentBet(0),
resultMessage("")
{
    window.setFramerateLimit(60);
    if (!font.openFromFile("assets/fonts/DejaVuSans.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente" << std::endl;
        throw std::runtime_error("No se pudo cargar la fuente");
    }

    //crear el game
    game = std::make_unique<Game> ("Jugador",1000); //nombre y dinero por defecto

    //inicializar botones
    initializeButtons();

    std::cout<<"Blackjack inicializado correctamente"  << std::endl;
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

    //Botones de INSURANCE
    InsuranceYesButton.setSize(sf::Vector2f(220, 50));
    InsuranceYesButton.setPosition({400, 600});
    InsuranceYesButton.setFillColor(sf::Color(100, 150, 255));

    InsuranceNoButton.setSize(sf::Vector2f(220, 50));
    InsuranceNoButton.setPosition({760, 600});
    InsuranceNoButton.setFillColor(sf::Color(255, 100, 100));


    // Botón DOUBLE
    doubleButton.setSize(sf::Vector2f(120, 50));
    doubleButton.setPosition({720, 600});
    doubleButton.setFillColor(sf::Color(255, 200, 100));

    // Botón NEXT ROUND
    nextRoundButton.setSize(sf::Vector2f(200, 60));
    nextRoundButton.setPosition({540, 600});
    nextRoundButton.setFillColor(sf::Color(100, 200, 100));

    // Botones de apuesta
    bet10Button.setSize(sf::Vector2f(80, 50));
    bet10Button.setPosition(sf::Vector2f(340, 380));
    bet10Button.setFillColor(sf::Color(100, 100, 255));

    bet50Button.setSize(sf::Vector2f(80, 50));
    bet50Button.setPosition(sf::Vector2f(450, 380));
    bet50Button.setFillColor(sf::Color(100, 100, 255));

    bet100Button.setSize(sf::Vector2f(80, 50));
    bet100Button.setPosition(sf::Vector2f(560, 380));
    bet100Button.setFillColor(sf::Color(100, 100, 255));

    AllInButton.setSize(sf::Vector2f(150,50));
    AllInButton.setPosition(sf::Vector2f(670, 380));
    AllInButton.setFillColor(sf::Color(100, 100, 255));


    clearBetButton.setSize(sf::Vector2f(150, 50));
    clearBetButton.setPosition(sf::Vector2f(860, 380));
    clearBetButton.setFillColor(sf::Color(255, 100, 100));

    dealButton.setSize(sf::Vector2f(150, 60));
    dealButton.setPosition(sf::Vector2f(565, 480));
    dealButton.setFillColor(sf::Color(100, 200, 100));

    //Boton de reset round
    restartButton.setSize(sf::Vector2f(300,60));
    restartButton.setPosition(sf::Vector2f(490,600));
    restartButton.setFillColor(sf::Color(255, 100, 100));

    //Boton de stats
    statsButton.setSize(sf::Vector2f(200,60));
    statsButton.setPosition(sf::Vector2f(540, 480));
    statsButton.setFillColor(sf::Color(100, 200, 100));

    //Boton back
    backButton.setSize(sf::Vector2f(200,60));
    backButton.setPosition(sf::Vector2f(540, 480));
    backButton.setFillColor(sf::Color(100, 100, 100));

    //Boton menu
    menuButton.setSize(sf::Vector2f(200,60));
    menuButton.setPosition(sf::Vector2f(540, 580));
    menuButton.setFillColor(sf::Color(100, 100, 100));

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
            case GUIState::GAME_OVER:
            drawGameOverScreen();
            break;
        case GUIState::STATS:
            drawStatsScreen();
            break;
        case GUIState::INSURANCE:
            drawInsuranceScreen();
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

    //Stats
    drawButton(statsButton, "STATS");

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
    drawButton(AllInButton, "ALL IN");
    drawButton(clearBetButton, "CLEAR");
    drawButton(menuButton, "MENU");

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
    title.setPosition(sf::Vector2f(520, 15));
    window.draw(title);

    // Etiqueta Dealer
    sf::Text dealerLabel(font);
    dealerLabel.setString("DEALER");
    dealerLabel.setCharacterSize(22);
    dealerLabel.setFillColor(sf::Color::Yellow);
    dealerLabel.setPosition(sf::Vector2f(100, 130));
    window.draw(dealerLabel);

    // Dibujar mano del dealer (primera carta oculta)
    drawDealerHand(true);

    //Maybe añadir linea separatoria ¿?
    //probare como queda para ver si se añade o no se añade

    // Etiqueta Player
    sf::Text playerLabel(font);
    playerLabel.setString("PLAYER");
    playerLabel.setCharacterSize(22);
    playerLabel.setFillColor(sf::Color::Yellow);
    playerLabel.setPosition(sf::Vector2f(100, 410));
    window.draw(playerLabel);

    // Dibujar mano del jugador
    drawPlayerHand();

    // Mostrar dinero y apuesta
    sf::Text moneyText(font);
    moneyText.setString("Money: $" + std::to_string(game->getPlayerMoney()));
    moneyText.setCharacterSize(24);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setPosition(sf::Vector2f(50, 20));
    window.draw(moneyText);

    sf::Text betText(font);
    betText.setString("Bet: $" + std::to_string(game->getCurrentBet()));
    betText.setCharacterSize(24);
    betText.setFillColor(sf::Color::White);
    betText.setPosition(sf::Vector2f(50, 60));
    window.draw(betText);

    // Dibujar botones
    drawButton(hitButton, "HIT");
    drawButton(standButton, "STAND");

    // Double solo si tiene 2 cartas y suficiente dinero
    if (game->canDoubleDown()) {
        drawButton(doubleButton, "DOUBLE");
    }
}

void BlackjackGUI::drawResultScreen() {
    // Título
    sf::Text title(font);
    title.setString("ROUND OVER");
    title.setCharacterSize(45);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(480, 20));
    window.draw(title);

    // Etiqueta Dealer
    sf::Text dealerLabel(font);
    dealerLabel.setString("DEALER");
    dealerLabel.setCharacterSize(22);
    dealerLabel.setFillColor(sf::Color::Yellow);
    dealerLabel.setPosition(sf::Vector2f(100, 130));
    window.draw(dealerLabel);

    // Mostrar todas las cartas del dealer (reveladas)
    drawDealerHand(false);

    //Linea separatoria ¿?
    // ya la probare aqui tambien

    // Etiqueta Player
    sf::Text playerLabel(font);
    playerLabel.setString("PLAYER");
    playerLabel.setCharacterSize(22);
    playerLabel.setFillColor(sf::Color::Yellow);
    playerLabel.setPosition(sf::Vector2f(100, 410));
    window.draw(playerLabel);

    // Mostrar cartas del jugador
    drawPlayerHand();

    // Mostrar resultado
    sf::Text resultText(font);
    resultText.setString(resultMessage);
    resultText.setCharacterSize(38);

    // Color según resultado
    switch (lastOutcome ) {

        using enum RoundResult::Outcome;
        case PLAYER_WIN:
        case PLAYER_BLACKJACK:
        case DEALER_BUST:
            resultText.setFillColor(sf::Color::Green);
        break;

        case DEALER_BLACKJACK:
        case DEALER_WIN:
        case PLAYER_BUST:
            resultText.setFillColor(sf::Color::Red);
        break;

        case TIE:
        case BOTH_BLACKJACK:
            resultText.setFillColor(sf::Color::Yellow);
        break;

    }

    // Centrar texto del resultado
    sf::FloatRect textBounds = resultText.getLocalBounds();
    resultText.setPosition(sf::Vector2f(
        640 - textBounds.size.x / 2.0f,
        540
    ));
    window.draw(resultText);

    // Dinero actual
    sf::Text moneyText(font);
    moneyText.setString("Money: $" + std::to_string(game->getPlayerMoney()));
    moneyText.setCharacterSize(28);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setPosition(sf::Vector2f(770, 610));
    window.draw(moneyText);

    // Botón siguiente ronda
    drawButton(nextRoundButton, "NEXT ROUND");
}

void BlackjackGUI::drawGameOverScreen() {

    //titulo
    sf::Text title(font);
    title.setString("GAME OVER");
    title.setCharacterSize(45);
    title.setFillColor(sf::Color::Red);
    title.setPosition(sf::Vector2f(490, 20));
    window.draw(title);

    //Boton para resetear
    drawButton(restartButton, "PLAY AGAIN");

    //Estadisticas de esa partida que puedan interesar
    title.setString("STATISTICS");
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(580, 120));
    window.draw(title);

    drawStats(280,250);


}

void BlackjackGUI::drawStatsScreen() {
    //Estadisticas de esa partida que puedan interesar
    sf::Text title(font);
    title.setString("STATISTICS");
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(580, 20));
    window.draw(title);

    drawStats(280,50);

    drawButton(backButton, "BACK");
}

void BlackjackGUI::drawInsuranceScreen() {

    drawInsuranceGameScreen();
    drawButton(InsuranceYesButton, "INSURANCE YES");
    drawButton(InsuranceNoButton, "INSURANCE NO");


}

void BlackjackGUI::drawInsuranceGameScreen() {
    // Título
    sf::Text title(font);
    title.setString(std::string("INSURANCE?") + "($" + std::to_string(game->getCurrentBet()/2) + ")");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(520, 15));
    window.draw(title);

    // Etiqueta Dealer
    sf::Text dealerLabel(font);
    dealerLabel.setString("DEALER");
    dealerLabel.setCharacterSize(22);
    dealerLabel.setFillColor(sf::Color::Yellow);
    dealerLabel.setPosition(sf::Vector2f(100, 130));
    window.draw(dealerLabel);

    // Dibujar mano del dealer (primera carta oculta)
    drawDealerHand(true);

    //Maybe añadir linea separatoria ¿?
    //probare como queda para ver si se añade o no se añade

    // Etiqueta Player
    sf::Text playerLabel(font);
    playerLabel.setString("PLAYER");
    playerLabel.setCharacterSize(22);
    playerLabel.setFillColor(sf::Color::Yellow);
    playerLabel.setPosition(sf::Vector2f(100, 410));
    window.draw(playerLabel);

    // Dibujar mano del jugador
    drawPlayerHand();

    // Mostrar dinero y apuesta
    sf::Text moneyText(font);
    moneyText.setString("Money: $" + std::to_string(game->getPlayerMoney()));
    moneyText.setCharacterSize(24);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setPosition(sf::Vector2f(50, 20));
    window.draw(moneyText);

    sf::Text betText(font);
    betText.setString("Bet: $" + std::to_string(game->getCurrentBet()));
    betText.setCharacterSize(24);
    betText.setFillColor(sf::Color::White);
    betText.setPosition(sf::Vector2f(50, 60));
    window.draw(betText);

}


void BlackjackGUI::drawPlayerHand() {
    const Player& player = game->getPlayer();
    const auto& hand = player.getHand();

    float startX = 200.0f;
    float y = 420.0f;
    float spacing = 90.0f;

    for (size_t i = 0; i < hand.size(); i++) {
        drawCard(sf::Vector2f(startX + i * spacing, y), hand[i], false);
    }

    // Mostrar valor de la mano
    sf::Text valueText(font);
    valueText.setString("Value: " + std::to_string(player.getHandValue()));
    valueText.setCharacterSize(22);
    valueText.setFillColor(sf::Color::White);
    valueText.setPosition(sf::Vector2f(200, y + 120));
    window.draw(valueText);
}

void BlackjackGUI::drawDealerHand(bool hideFirst) {
    const Dealer& dealer = game->getDealer();
    const auto& hand = dealer.getHand();

    float startX = 200.0f;
    float y = 140.0f;
    float spacing = 90.0f;

    for (size_t i = 0; i < hand.size(); i++) {
        bool shouldHide = (hideFirst && i == 0);
        drawCard(sf::Vector2f(startX + i * spacing, y), hand[i], shouldHide);
    }

    // Mostrar valor solo si no hay cartas ocultas
    if (!hideFirst) {
        sf::Text valueText(font);
        valueText.setString("Value: " + std::to_string(dealer.getHandValue()));
        valueText.setCharacterSize(22);
        valueText.setFillColor(sf::Color::White);
        valueText.setPosition(sf::Vector2f(200, y + 120));
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
        sf::RectangleShape backPattern(sf::Vector2f(80,110));
        backPattern.setPosition((position));
        backPattern.setFillColor((sf::Color(50,50,150)));
        window.draw(backPattern);

        sf::Text pattern(font);
        pattern.setString(("?"));
        pattern.setCharacterSize(50);
        pattern.setFillColor(sf::Color::White);
        pattern.setPosition(sf::Vector2f(position.x+25,position.y+25));
        window.draw(pattern);
    } else {
        // Mostrar rango (arriba izquierda)
        sf::Text rango(font);
        rango.setString(card.getCardString());
        rango.setCharacterSize(24);
        if (card.getsuit()== Suit::Hearts || card.getsuit()==Suit::Diamonds) {
            //si es corazones o espadas -> color rojo
            rango.setFillColor(sf::Color::Red);
        }
        else {
            //para trebol y picas -> color negro
            rango.setFillColor(sf::Color::Black);
        }
        rango.setPosition(sf::Vector2f(position.x + 8, position.y + 5));
        window.draw(rango);

        // Mostrar palo (centro)
        sf::Text palo(font);
        std::string paloStr = card.getSuitString();
        palo.setString(sf::String::fromUtf8(paloStr.begin(), paloStr.end()));
        palo.setCharacterSize(35);

        // Color rojo para corazones y diamantes
        if (card.getsuit() == Suit::Hearts || card.getsuit() == Suit::Diamonds) {
            palo.setFillColor(sf::Color::Red);
        } else {
            palo.setFillColor(sf::Color::Black);
        }

        palo.setPosition(sf::Vector2f(position.x + 22, position.y + 45));
        window.draw(palo);

        //texto esquina inferior derecha (invertido)
        sf::Text rangoBottom(font);
        rangoBottom.setString((card.getCardString()));
        rangoBottom.setCharacterSize(24);

        if (card.getsuit() == Suit::Hearts || card.getsuit() == Suit::Diamonds) {
            rangoBottom.setFillColor(sf::Color::Red);
        } else {
            rangoBottom.setFillColor(sf::Color::Black);
        }

        rangoBottom.setPosition(sf::Vector2f(position.x + 45, position.y + 75));
        window.draw(rangoBottom);
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
            else if (mouseOver(statsButton)) {
                currentState = GUIState::STATS;
            }
            break;

        case GUIState::BETTING:// Botón +$10
            if (mouseOver(bet10Button)) {
                if (currentBet + 10 <= game->getPlayerMoney()) {
                    currentBet += 10;
                    std::cout << "Apuesta: $" << currentBet << std::endl;
                }
            }

            if (mouseOver(menuButton)) {
                currentState = GUIState::MENU;
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

            if (mouseOver(AllInButton)) {
                currentBet = game->getPlayerMoney();
                std::cout << "Apuesta: $" << currentBet << std::endl;
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
                    if (game->canOfferInsurance()) {
                        currentState = GUIState::INSURANCE;
                    }
                    else{
                        continueAfterDeal();
                    }
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
                    resultMessage = buildResultMessage(roundResult,false);
                    lastOutcome=roundResult.outcome;
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
                lastOutcome = roundResult.outcome;
                resultMessage = buildResultMessage(roundResult,false);

                currentState = GUIState::RESULT;
            }

            // Botón DOUBLE
            else if (mouseOver(doubleButton)) {
                if (game->canDoubleDown()) {
                    std::cout << "DOUBLE clicked" << std::endl;
                    game->playerDoubleDown();

                    // Después de double, siempre pasa al dealer
                    game->playDealerTurn();
                    RoundResult roundResult = game->finishRound();
                        resultMessage = buildResultMessage(roundResult,true);
                        lastOutcome=roundResult.outcome;

                        lastOutcome = roundResult.outcome;
                        resultMessage = buildResultMessage(roundResult,true);


                    currentState = GUIState::RESULT;
                }
            }
            break;

        case GUIState::RESULT:

            if (mouseOver(nextRoundButton)) {
                if (!game->canContinue()) {
                    currentState= GUIState::GAME_OVER;
                }
                else{
                    resultMessage.clear();
                    currentBet=0;
                    game->resetForNewRound();
                    std::cout << "NEXT ROUND clicked - Cambiando a BETTING" << std::endl;
                    currentState=GUIState::BETTING;
                }
            }
            break;

        case GUIState::GAME_OVER:
            if (mouseOver(restartButton)) {
                game = std::make_unique<Game>("Jugador", 1000);
                currentBet=0;
                currentState=GUIState::BETTING;
            }
        break;

    case GUIState::STATS:
            if (mouseOver(backButton))
            {
                //back from stats menu
                currentState = GUIState::MENU;
            }
            break;
        case GUIState::INSURANCE:
            if (mouseOver(InsuranceYesButton)) {
                game->takeInsurance(true);
                continueAfterDeal();
            }

            else if (mouseOver(InsuranceNoButton)) {
                game->takeInsurance(false);
                continueAfterDeal();
            }


            break;
    }
}

void BlackjackGUI::continueAfterDeal() {

    if (game->getState()==GameState::ROUND_END) {
        RoundResult roundResult = game->finishRound();
        // Mensaje según resultado
        lastOutcome = roundResult.outcome;
        resultMessage = buildResultMessage(roundResult,false);
        currentState = GUIState::RESULT;
    }
    else{
        currentState = GUIState::PLAYING;
    }

}

std::string BlackjackGUI::buildResultMessage(const RoundResult &r, bool doubled) const {
    RoundResult principal = r;

    if (principal.insuranceChange!=0) {
        principal.moneyChange = principal.moneyChange - principal.insuranceChange;
        principal.insuranceChange = 0;
        return buildResultMessage(principal,doubled) + " INSURANCE " + std::to_string(r.insuranceChange);
    }

    switch (r.outcome) {

        using enum RoundResult::Outcome;
        case PLAYER_BLACKJACK:
            if (!doubled) {
                return "PLAYER BLACKJACK! +$" + std::to_string(r.moneyChange);
            }
            else {
                return "DOUBLE PLAYER BLACKJACK! +$" + std::to_string(r.moneyChange);
            }
        break;
        case PLAYER_WIN:
            if (!doubled) {
                return "PLAYER WIN! +$" + std::to_string(r.moneyChange);
            }
            else {
                return "DOUBLE PLAYER WIN! +$" + std::to_string(r.moneyChange);
            }
        break;
        case DEALER_BUST:
            if (!doubled) {
                return "DEALER BUST! +$" + std::to_string(r.moneyChange);
            }
            else {
                return "DEALER BUST AFTER DOUBLE! +$" + std::to_string(r.moneyChange);
            }
        break;

        case PLAYER_BUST:
            if (!doubled) {
                return "PLAYER BUST! -$" + std::to_string(-r.moneyChange);
            }
        else {
            return "PLAYER BUST AFTER DOUBLE! -$" + std::to_string(-r.moneyChange);
        }
        break;

        case DEALER_WIN:
        if (!doubled) {
            return "DEALER WIN! -$" + std::to_string(-r.moneyChange);
        }
        else {
            return "DEALER WIN AFTER DOUBLE! -$" + std::to_string(-r.moneyChange);
        }
        break;

        case DEALER_BLACKJACK:
        if (!doubled) {
            return "DEALER BLACKJACK! -$" + std::to_string(-r.moneyChange);
        }
        else {
            return "DEALER BLACKJACK AFTER DOUBLE! -$" + std::to_string(-r.moneyChange);

        }
        break;

        case TIE:
        if (!doubled) {
            return "TIE! +$" + std::to_string(r.moneyChange);
        }
        else {
            return "TIE AFTER DOUBLE! +$" + std::to_string(r.moneyChange);
        }
        break;

        case BOTH_BLACKJACK:
        if (!doubled) {
            return "BOTH BLACKJACK! +$" + std::to_string(r.moneyChange);
        }
        else {
            return "BOTH BLACKJACK AFTER DOUBLE! +$" + std::to_string(r.moneyChange);
        }
        break;
    }
    return "";
}

void BlackjackGUI::drawStats(float x, float y)
    {
    float lineY = y;
    sf::Text title(font);
    title.setString("Wins: " + std::to_string(game->getWins()));
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(x, lineY));
    window.draw(title);

    lineY+=40;

    title.setString("Losses: " + std::to_string(game->getLosses()));
    title.setPosition(sf::Vector2f(x, lineY));
    window.draw(title);

    lineY+=40;

    title.setString("Ties: " + std::to_string(game->getTies()));
    title.setPosition(sf::Vector2f(x, lineY));
    window.draw(title);

    lineY+=40;

    title.setString("Best Winstreak: " + std::to_string(game->getBestStreak()));
    title.setPosition(sf::Vector2f(x, lineY));
    window.draw(title);

    lineY+=40;

    title.setString(std::format("Winrate: {:.1f}%", game->getWinRate()));
    title.setPosition(sf::Vector2f(x, lineY));
    window.draw(title);

    lineY+=40;

    title.setString("Blackjacks hit: " + std::to_string(game->getBlackjacksHit()));
    title.setPosition(sf::Vector2f(x, lineY));
    window.draw(title);

    lineY+=40;

    title.setString("Max money: " + std::to_string(game->getMaxMoney()));
    title.setPosition(sf::Vector2f(x, lineY));
    window.draw(title);

    lineY+=40;

    title.setString("Total Bet: " + std::to_string(game->getTotalBet()));
    title.setPosition(sf::Vector2f(x, lineY));
    window.draw(title);


    }
