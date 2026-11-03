//
// Created by Airam on 20/10/2025.
//

#include "../include/Game.h"


bool Game::placeBet(int amount) {

    if (amount <= 0 ||amount > playerMoney) {
        return false;
    }

    currentBet=amount;
    totalBet+=amount;
    state=GameState::BETTING;
    return true;
}


void Game::startRound() {

    if (deck.getRemainingCards() <= 10) {
        deck.shuffle(); //if we have 10 or less cards reshuffle so the deck is full again
    }

    state=GameState::DEALING;
    player.clearHand();
    dealer.clearHand();

    player.addCard(deck.dealCard());
    dealer.addCard(deck.dealCard());
    player.addCard(deck.dealCard());
    dealer.addCard(deck.dealCard());

    state=GameState::PLAYER_TURN; //pasamos a turno de player justo al repartir
    if (hasBlackjack(player.getHandValue(),player.getHandSize()) || hasBlackjack(dealer.getHandValue(), dealer.getHandSize())) {
        state=GameState::ROUND_END;
    }


}


PlayerActionResult Game::playerHit() {
    if (state != GameState::PLAYER_TURN) {
        return PlayerActionResult::CONTINUE; //si no es su turno, continúa el game
    }

    player.addCard(deck.dealCard());
    int value=player.getHandValue();

    if (value > 21) {
        return PlayerActionResult::BUST;
    }
    else if (value==21) {
        return PlayerActionResult::BLACKJACK;
    }

    return PlayerActionResult::CONTINUE;
}


PlayerActionResult Game::playerStand() {
    if (state != GameState::PLAYER_TURN) {
        return PlayerActionResult::CONTINUE; //si no es su turno, continúa el game
    }
    state = GameState::DEALER_TURN;
    return PlayerActionResult::STAND;
    //Cambiamos a turno de dealer porque el player se mantiene
}


PlayerActionResult Game::playerDoubleDown () {
    if (state != GameState::PLAYER_TURN) {
        return PlayerActionResult::CONTINUE; //si no es su turno, continúa el game
    }
    if (player.getHandSize() !=2 || playerMoney < currentBet) {
        return PlayerActionResult::CONTINUE; //si no tiene para apostar, continua el juego
    }

    currentBet*=2;
    totalBet+=currentBet / 2;
    player.addCard(deck.dealCard());

    state=GameState::DEALER_TURN; //pasamos a turno de dealer automaticamente

    if (player.getHandValue() > 21) {
        return PlayerActionResult::BUST;
    }

    return PlayerActionResult::STAND; //se mantiene por defecto ya que no puede pedir mas cartas
}


Game::Game(const char* PlayerName, int initialMoney)
:player(PlayerName),
dealer(),
deck()
{

    playerMoney = initialMoney;
    this->initialMoney=initialMoney;
    currentBet = 0;
    state=GameState::BETTING;
    wins = 0;
    losses = 0;
    ties = 0;
    currentStreak = 0;
    bestStreak = 0;
    blackjacksHit = 0;
    maxMoney = initialMoney;
    totalBet = 0;
    deck.shuffle();

    std::cout << "¡Bienvenido a la mesa de Blackjack, " << PlayerName << "!" << std::endl;
    std::cout << "Dinero inicial: $" << playerMoney << std::endl << std::endl;
}


void Game::resetForNewRound() {
    player.clearHand();
    dealer.clearHand();
    currentBet=0;
    state=GameState::BETTING; //reset de ronda -> volver a tiempo de apuestas
}

double Game::getWinRate() const {
    int total = wins + losses + ties;
    if (total == 0) return 0.0;
    return (double)wins / total *100.0;
}


void Game::playDealerTurn() {
    if (state != GameState::DEALER_TURN) {
        return; //si no es su turno, se acaba el metodo
    }


    if (player.getHandValue() > 21) {
        state=GameState::ROUND_END; //si player tiene BJ se acaba la ronda
        return; //plater gana
    }

    while (dealer.shouldHit()) {
        dealer.addCard(deck.dealCard());
    }

    state=GameState::ROUND_END; //termina ronda al acavabar el turno del dealer
}

void Game::playRound() {
    // 1. Pedir apuesta (mantener cout/cin aquí)
    std::cout << "\n--- NUEVA RONDA ---" << std::endl;
    std::cout << "Tienes $" << playerMoney << std::endl;
    std::cout << "¿Cuánto quieres apostar? ";
    int bet;
    std::cin >> bet;
    std::cin.ignore();

    while (!placeBet(bet)) {
        std::cout << "Apuesta inválida. Debe ser entre $1 y $" << playerMoney << ": ";
        std::cin >> bet;
        std::cin.ignore();
    }

    std::cout << "Apostaste $" << currentBet << std::endl << std::endl;

    // 2. Iniciar ronda
    startRound();

    // 3. Mostrar cartas iniciales
    std::cout << "--- CARTAS INICIALES ---" << std::endl;
    player.showHand();
    dealer.showHand(true);

    // 4. Verificar Blackjack natural
    bool playerBJ = hasBlackjack(player.getHandValue(), player.getHandSize());
    bool dealerBJ = hasBlackjack(dealer.getHandValue(), dealer.getHandSize());

    if (playerBJ) {
        std::cout << "¡¡¡BLACKJACK NATURAL!!! 🎰" << std::endl;
    }

    if (dealerBJ) {
        std::cout << "\n--- El dealer revela sus cartas ---" << std::endl;
        dealer.showHand(false);
        std::cout << "El dealer tiene BLACKJACK..." << std::endl;
    }

    // 5. Turno del jugador (si no hay Blackjack natural)
    if (!playerBJ && !dealerBJ) {
        bool playing = true;
        bool firstTurn = true;

        while (playing) {
            if (firstTurn && playerMoney >= currentBet) {
                std::cout << "Opciones: [H]it | [S]tand | [D]ouble Down: ";
            } else {
                std::cout << "¿Quieres pedir carta? (H/S): ";
            }

            char resp;
            std::cin >> resp;
            std::cin.ignore();
            resp = toupper(resp);

            if (resp == 'H') {
                PlayerActionResult result = playerHit();
                std::cout << "\n--- Nueva carta ---" << std::endl;
                player.showHand();

                if (result == PlayerActionResult::BUST) {
                    std::cout << "¡TE PASASTE!" << std::endl;
                    playing = false;
                }
                else if (result == PlayerActionResult::BLACKJACK) {
                    std::cout << "¡Tienes 21!" << std::endl;
                    playing = false;
                }

                firstTurn = false;
            }
            else if (resp == 'S') {
                playerStand();
                std::cout << "Te plantas con " << player.getHandValue() << std::endl;
                playing = false;
            }
            else if (resp == 'D' && firstTurn && playerMoney >= currentBet) {
                std::cout << "\n--- DOUBLE DOWN ---" << std::endl;
                PlayerActionResult result = playerDoubleDown();
                std::cout << "Doblas tu apuesta a $" << currentBet << std::endl;
                std::cout << "Recibes una carta final:" << std::endl;
                player.showHand();

                if (result == PlayerActionResult::BUST) {
                    std::cout << "¡TE PASASTE!" << std::endl;
                }
                else {
                    std::cout << "Te plantas con " << player.getHandValue() << std::endl;
                }
                playing = false;
            }
            else {
                std::cout << "Opción no válida." << std::endl;
            }
        }
    }

    // 6. Turno del dealer
    if (!playerBJ && !dealerBJ && player.getHandValue() <= 21) {
        playDealerTurn();
        std::cout << "\n--- TURNO DEL DEALER ---" << std::endl;
        dealer.showHand(false);

        if (dealer.getHandValue() > 21) {
            std::cout << "¡Dealer se pasó!" << std::endl;
        } else {
            std::cout << "Dealer se planta con " << dealer.getHandValue() << std::endl;
        }
    }

    // 7. Determinar ganador
    RoundResult result = finishRound();

    std::cout << "\n=== RESULTADO ===" << std::endl;
    std::cout << "Jugador: " << result.playerValue << std::endl;
    std::cout << "Dealer: " << result.dealerValue << std::endl;

    switch (result.outcome) {
        case RoundResult::Outcome::PLAYER_WIN:
            std::cout << "¡GANASTE! Tu mano es mejor." << std::endl;
            break;
        case RoundResult::Outcome::DEALER_WIN:
            std::cout << "Perdiste. La mano del dealer es mejor." << std::endl;
            break;
        case RoundResult::Outcome::TIE:
            std::cout << "¡EMPATE! Recuperas tu apuesta." << std::endl;
            break;
        case RoundResult::Outcome::PLAYER_BLACKJACK:
            std::cout << "¡¡¡BLACKJACK NATURAL!!! Ganas 1.5x tu apuesta 🎰💰" << std::endl;
            break;
        case RoundResult::Outcome::DEALER_BLACKJACK:
            std::cout << "El dealer tiene Blackjack. Pierdes." << std::endl;
            break;
        case RoundResult::Outcome::BOTH_BLACKJACK:
            std::cout << "¡EMPATE! Ambos tienen Blackjack." << std::endl;
            break;
        case RoundResult::Outcome::PLAYER_BUST:
            std::cout << "¡Perdiste! Te pasaste de 21." << std::endl;
            break;
        case RoundResult::Outcome::DEALER_BUST:
            std::cout << "¡GANASTE! El dealer se pasó." << std::endl;
            break;
    }

    if (result.moneyChange > 0) {
        std::cout << "Ganaste $" << result.moneyChange << std::endl;
    } else if (result.moneyChange < 0) {
        std::cout << "Perdiste $" << (-result.moneyChange) << std::endl;
    }

    std::cout << "Dinero actual: $" << playerMoney << std::endl << std::endl;

    // 8. Resetear
    resetForNewRound();
}

RoundResult Game::finishRound() {
    RoundResult result;

    int playerValue = player.getHandValue();
    int dealerValue = dealer.getHandValue();
    size_t playerCards = player.getHandSize();
    size_t dealerCards = dealer.getHandSize();

    bool playerBJ = hasBlackjack(playerValue, playerCards);
    bool dealerBJ = hasBlackjack(dealerValue, dealerCards);

    result.playerValue= playerValue;
    result.dealerValue= dealerValue;
    result.wasBlackjack = playerBJ;

     if (playerBJ && dealerBJ) {
        result.outcome = RoundResult::Outcome::BOTH_BLACKJACK;
        result.moneyChange = 0;
        ties++;
        currentStreak = 0;
    }
    else if (playerBJ && !dealerBJ) {
        result.outcome = RoundResult::Outcome::PLAYER_BLACKJACK;
        int winnings = (currentBet * 3) / 2;
        result.moneyChange = winnings;
        playerMoney += result.moneyChange;
        wins++;
        blackjacksHit++;
        currentStreak++;
        if (currentStreak > bestStreak) bestStreak = currentStreak;
    }
    else if (dealerBJ && !playerBJ) {
        result.outcome = RoundResult::Outcome::DEALER_BLACKJACK;
        result.moneyChange = -currentBet;
        playerMoney -= currentBet;
        losses++;
        currentStreak =0;
    }
    else if (playerValue > 21) {
        result.outcome = RoundResult::Outcome::PLAYER_BUST;
        result.moneyChange = -currentBet;
        playerMoney -= currentBet;
        losses++;
        currentStreak=0;
    }
    else if (dealerValue > 21) {
        result.outcome = RoundResult::Outcome::DEALER_BUST;
        result.moneyChange = currentBet;
        playerMoney += currentBet;
        wins++;
        currentStreak++;
        if (currentStreak > bestStreak) bestStreak = currentStreak;
    }
    else if (playerValue > dealerValue) {
        result.outcome = RoundResult::Outcome::PLAYER_WIN;
        result.moneyChange = currentBet;
        playerMoney += currentBet;
        wins++;
        currentStreak++;
        if (currentStreak > bestStreak) bestStreak = currentStreak;
    }
    else if (dealerValue > playerValue) {
        result.outcome = RoundResult::Outcome::DEALER_WIN;
        result.moneyChange = -currentBet;
        playerMoney -= currentBet;
        losses++;
        currentStreak = 0;
    }
    else {
        result.outcome = RoundResult::Outcome::TIE;
        result.moneyChange = 0;
        ties++;
        currentStreak = 0;
    }

    if (playerMoney > maxMoney) {
        maxMoney = playerMoney;
    }

    state = GameState::ROUND_END;
    return result;
}


bool Game::hasBlackjack(int handValue, size_t numCards) const {
    return handValue==21 && numCards==2;
}


void Game::showStatistics() const {
    std::cout << "\n╔════════════════════════════════════╗" << std::endl;
    std::cout << "║        ESTADÍSTICAS                ║" << std::endl;
    std::cout << "╠════════════════════════════════════╣" << std::endl;
    std::cout << "║ Victorias:           " << std::setw(12) << wins << " ║" << std::endl;
    std::cout << "║ Derrotas:            " << std::setw(12) << losses << " ║" << std::endl;
    std::cout << "║ Empates:             " << std::setw(12) << ties << " ║" << std::endl;

    int totalRounds = wins + losses + ties;
    if (totalRounds > 0) {
        double winRate = (double)wins / totalRounds * 100;
        std::cout << "║ Win Rate:            " << std::setw(9) << std::fixed << std::setprecision(1) << winRate << "% ║" << std::endl;
    }

    std::cout << "║ Racha actual:        " << std::setw(12) << currentStreak << " ║" << std::endl;
    std::cout << "║ Mejor racha:         " << std::setw(12) << bestStreak << " ║" << std::endl;
    std::cout << "║ Blackjacks:          " << std::setw(12) << blackjacksHit << " ║" << std::endl;
    std::cout << "║ Dinero máximo:      $" << std::setw(11) << maxMoney << " ║" << std::endl;
    std::cout << "║ Total apostado:     $" << std::setw(11) << totalBet << " ║" << std::endl;

    int netProfit = playerMoney - initialMoney;
    std::cout << "║ Beneficio neto:     $" << std::setw(11) << netProfit << " ║" << std::endl;
    std::cout << "╚════════════════════════════════════╝" << std::endl << std::endl;
}






