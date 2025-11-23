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

    state=GameState::DEALING;
    player.clearHand();
    dealer.clearHand();

    player.addCard(deck.dealCard());
    dealer.addCard(deck.dealCard());
    player.addCard(deck.dealCard());
    dealer.addCard(deck.dealCard());

    state=GameState::PLAYER_TURN; //pasamos a turno de player justo al repartir
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
    state = GameState::DEALER_TURN;
    return PlayerActionResult::STAND;
    //Cambiamos a turno de dealer porque el player se mantiene
}


PlayerActionResult Game::playerDoubleDown () {
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

    cout << "¡Bienvenido a la mesa de Blackjack, " << PlayerName << "!" << endl;
    cout << "Dinero inicial: $" << playerMoney << endl << endl;
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
    cout << "\n--- NUEVA RONDA ---" << endl;
    cout << "Tienes $" << playerMoney << endl;
    cout << "¿Cuánto quieres apostar? ";
    int bet;
    cin >> bet;
    cin.ignore();

    while (!placeBet(bet)) {
        cout << "Apuesta inválida. Debe ser entre $1 y $" << playerMoney << ": ";
        cin >> bet;
        cin.ignore();
    }

    cout << "Apostaste $" << currentBet << endl << endl;

    // 2. Iniciar ronda
    startRound();

    // 3. Mostrar cartas iniciales
    cout << "--- CARTAS INICIALES ---" << endl;
    player.showHand();
    dealer.showHand(true);

    // 4. Verificar Blackjack natural
    bool playerBJ = hasBlackjack(player.getHandValue(), player.getHandSize());
    bool dealerBJ = hasBlackjack(dealer.getHandValue(), dealer.getHandSize());

    if (playerBJ) {
        cout << "¡¡¡BLACKJACK NATURAL!!! 🎰" << endl;
    }

    if (dealerBJ) {
        cout << "\n--- El dealer revela sus cartas ---" << endl;
        dealer.showHand(false);
        cout << "El dealer tiene BLACKJACK..." << endl;
    }

    // 5. Turno del jugador (si no hay Blackjack natural)
    if (!playerBJ && !dealerBJ) {
        bool playing = true;
        bool firstTurn = true;

        while (playing) {
            if (firstTurn && playerMoney >= currentBet) {
                cout << "Opciones: [H]it | [S]tand | [D]ouble Down: ";
            } else {
                cout << "¿Quieres pedir carta? (H/S): ";
            }

            char resp;
            cin >> resp;
            cin.ignore();
            resp = toupper(resp);

            if (resp == 'H') {
                PlayerActionResult result = playerHit();
                cout << "\n--- Nueva carta ---" << endl;
                player.showHand();

                if (result == PlayerActionResult::BUST) {
                    cout << "¡TE PASASTE!" << endl;
                    playing = false;
                }
                else if (result == PlayerActionResult::BLACKJACK) {
                    cout << "¡Tienes 21!" << endl;
                    playing = false;
                }

                firstTurn = false;
            }
            else if (resp == 'S') {
                playerStand();
                cout << "Te plantas con " << player.getHandValue() << endl;
                playing = false;
            }
            else if (resp == 'D' && firstTurn && playerMoney >= currentBet) {
                cout << "\n--- DOUBLE DOWN ---" << endl;
                PlayerActionResult result = playerDoubleDown();
                cout << "Doblas tu apuesta a $" << currentBet << endl;
                cout << "Recibes una carta final:" << endl;
                player.showHand();

                if (result == PlayerActionResult::BUST) {
                    cout << "¡TE PASASTE!" << endl;
                }
                else {
                    cout << "Te plantas con " << player.getHandValue() << endl;
                }
                playing = false;
            }
            else {
                cout << "Opción no válida." << endl;
            }
        }
    }

    // 6. Turno del dealer
    if (!playerBJ && !dealerBJ && player.getHandValue() <= 21) {
        playDealerTurn();
        cout << "\n--- TURNO DEL DEALER ---" << endl;
        dealer.showHand(false);

        if (dealer.getHandValue() > 21) {
            cout << "¡Dealer se pasó!" << endl;
        } else {
            cout << "Dealer se planta con " << dealer.getHandValue() << endl;
        }
    }

    // 7. Determinar ganador
    RoundResult result = finishRound();

    cout << "\n=== RESULTADO ===" << endl;
    cout << "Jugador: " << result.playerValue << endl;
    cout << "Dealer: " << result.dealerValue << endl;

    switch (result.outcome) {
        case RoundResult::Outcome::PLAYER_WIN:
            cout << "¡GANASTE! Tu mano es mejor." << endl;
            break;
        case RoundResult::Outcome::DEALER_WIN:
            cout << "Perdiste. La mano del dealer es mejor." << endl;
            break;
        case RoundResult::Outcome::TIE:
            cout << "¡EMPATE! Recuperas tu apuesta." << endl;
            break;
        case RoundResult::Outcome::PLAYER_BLACKJACK:
            cout << "¡¡¡BLACKJACK NATURAL!!! Ganas 1.5x tu apuesta 🎰💰" << endl;
            break;
        case RoundResult::Outcome::DEALER_BLACKJACK:
            cout << "El dealer tiene Blackjack. Pierdes." << endl;
            break;
        case RoundResult::Outcome::BOTH_BLACKJACK:
            cout << "¡EMPATE! Ambos tienen Blackjack." << endl;
            break;
        case RoundResult::Outcome::PLAYER_BUST:
            cout << "¡Perdiste! Te pasaste de 21." << endl;
            break;
        case RoundResult::Outcome::DEALER_BUST:
            cout << "¡GANASTE! El dealer se pasó." << endl;
            break;
    }

    if (result.moneyChange > 0) {
        cout << "Ganaste $" << result.moneyChange << endl;
    } else if (result.moneyChange < 0) {
        cout << "Perdiste $" << (-result.moneyChange) << endl;
    }

    cout << "Dinero actual: $" << playerMoney << endl << endl;

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
        result.moneyChange = currentBet + winnings;
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
    cout << "\n╔════════════════════════════════════╗" << endl;
    cout << "║        ESTADÍSTICAS                ║" << endl;
    cout << "╠════════════════════════════════════╣" << endl;
    cout << "║ Victorias:           " << setw(12) << wins << " ║" << endl;
    cout << "║ Derrotas:            " << setw(12) << losses << " ║" << endl;
    cout << "║ Empates:             " << setw(12) << ties << " ║" << endl;

    int totalRounds = wins + losses + ties;
    if (totalRounds > 0) {
        double winRate = (double)wins / totalRounds * 100;
        cout << "║ Win Rate:            " << setw(9) << fixed << setprecision(1) << winRate << "% ║" << endl;
    }

    cout << "║ Racha actual:        " << setw(12) << currentStreak << " ║" << endl;
    cout << "║ Mejor racha:         " << setw(12) << bestStreak << " ║" << endl;
    cout << "║ Blackjacks:          " << setw(12) << blackjacksHit << " ║" << endl;
    cout << "║ Dinero máximo:      $" << setw(11) << maxMoney << " ║" << endl;
    cout << "║ Total apostado:     $" << setw(11) << totalBet << " ║" << endl;

    int netProfit = playerMoney - initialMoney;
    cout << "║ Beneficio neto:     $" << setw(11) << netProfit << " ║" << endl;
    cout << "╚════════════════════════════════════╝" << endl << endl;
}






