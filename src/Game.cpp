//
// Created by Airam on 20/10/2025.
//

#include "../include/Game.h"

Game::Game(const char* PlayerName, int initialMoney)
:player(PlayerName),
dealer(),
deck()
{

    playerMoney = initialMoney;
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



bool Game::canContinue() const {

return playerMoney > 0;
}


void Game::resetRound() {
    player.clearHand();
    dealer.clearHand();
    currentBet=0;
    state=GameState::BETTING; //reset de ronda -> volver a tiempo de apuestas

    if (deck.isEmpty()) {
        cout << "Barajando...." << endl;
        deck=Deck();
        deck.shuffle();
    }
}


void Game::dealInitialCards() {

    player.addCard(deck.dealCard());
    dealer.addCard(deck.dealCard());
    player.addCard(deck.dealCard());
    dealer.addCard(deck.dealCard());
    cout << "--- CARTAS INICIALES ---" << endl;
    player.showHand();
    dealer.showHand(true);
    // 2 cartas para cada uno inicialmente y se enseñan

    //Comprobar blackjack natural para ambos
    if (hasBlackjack(player.getHandValue(),player.getHandSize())) {
        cout << "¡¡¡BLACKJACK!!! " << endl;
    }
    if (hasBlackjack(dealer.getHandValue(),dealer.getHandSize())) {
        cout << "El dealer tiene BLACKJACK..."<<endl;
        dealer.showHand(false);
    }

    // Verificar Blackjack natural
    bool playerBJ = hasBlackjack(player.getHandValue(), player.getHandSize());
    bool dealerBJ = hasBlackjack(dealer.getHandValue(), dealer.getHandSize());

    if (playerBJ) {
        cout << "¡¡¡BLACKJACK NATURAL!!! 🎰" << endl;
    }

    // Solo revelar dealer si tiene Blackjack (para terminar ronda inmediatamente)
    if (dealerBJ) {
        cout << "\n--- El dealer revela sus cartas ---" << endl;
        dealer.showHand(false);
        cout << "El dealer tiene BLACKJACK..." << endl;
    }
}

    void Game::playerTurn() {
        state = GameState::PLAYER_TURN;
        char resp;
    bool firstTurn=true;

    if (hasBlackjack(player.getHandValue(),player.getHandSize())) {
        return;
    }

        while (player.getHandValue() < 21) {
            if (firstTurn && playerMoney >=currentBet) {
                cout << "Opciones: [H]it  |  [S]tand  |  [D]ouble Down ";
            }
            else {
                cout << "¿Quieres pedir una carta? (H/S): "; //si no tiene suficiente o no es el primer turno solo podra hitear o stand
            }
            cin >> resp;
            cin.ignore();  // ← IMPORTANTE: Limpiar buffer

            if (toupper(resp)== 'H') {
                player.addCard(deck.dealCard());
                cout << "\n--- Nueva carta ---" << endl;
                player.showHand();

                if (player.getHandValue() > 21) {
                    cout << "¡TE PASASTE! Pierdes esta ronda." << endl;
                    return;
                }
                firstTurn=false; //ya no es el primer turno, no puede doblar
            }
            else if (toupper(resp) == 'D' && firstTurn && playerMoney >= currentBet) {
                cout << "\n--- DOUBLE DOWN ---" << endl;
                cout << "Doblas tu apuesta a $" << (currentBet*2) << endl;
                currentBet*=2;
                player.addCard(deck.dealCard());
                cout << "Recibes una carta final." << endl;
                player.showHand();

                if (player.getHandValue() > 21) {
                    cout << "¡TE PASASTE!" << endl;
                }
                else{
                    cout << "Te plantas con " << player.getHandValue() << endl;
                }
                return; //termina automaticamente si entra aqui
            }
            else if (toupper(resp) == 'S') {
                cout << "Te plantas con " << player.getHandValue() << endl;
                return;  // ← Sale del método
            }
            else {
                cout << "Opción no válida. Usa [S|H|D]." << endl;
            }
        }

        // Si llega aquí es porque tiene exactamente 21
        if (player.getHandValue() == 21) {
            cout << "¡BLACKJACK! Tienes 21." << endl;
        }
    }

void Game::dealerTurn() {
    if (player.getHandValue() > 21) {
        return; //plater gana
    }

    if (hasBlackjack(player.getHandValue(),player.getHandSize()) ||
        hasBlackjack(dealer.getHandValue(),dealer.getHandSize())) {
        return;
    }

    state=GameState::DEALER_TURN;
    cout << "\n--- TURNO DEL DEALER ---"<< endl;
    dealer.showHand(false); //muestra todas

    //dealer pide hasta 17 automaticamente
    while (dealer.shouldHit()) {
        cout << "Dealer pide una carta..." << endl;
        dealer.addCard(deck.dealCard());
        dealer.showHand(false);

        if (dealer.getHandValue() > 21) {
            cout << "¡Dealer se pasó! " << endl;
            return;
        }
    }

    cout << "Dealer se planta con " << dealer.getHandValue() << endl;
}

void Game::playRound()
{
    state=GameState::BETTING;
    cout << "\n--- NUEVA RONDA ---"<< endl;
    cout << "Tienes $" << playerMoney << endl;
    cout << "¿Cuánto quieres apostar? ";
    cin >> currentBet;
    cin.ignore();

    while (currentBet <=0 || currentBet > playerMoney) {
        cout << "Apuesta inválida- Debe ser entre $1 y $" << playerMoney << ": ";
        cin >> currentBet;
    }

    cout << "Apostaste $" << currentBet << endl << endl;
    totalBet+=currentBet;

    //Repartir cartas iniciales
    dealInitialCards();

    //turno del jugador
    playerTurn();

    //turno del dealer (solo si player no se paso)
    dealerTurn();

    //determinar ganador
    determineWinner();

    //resetear ronda
    resetRound();
}

void Game::determineWinner() {
    state = GameState::ROUND_END;

    int playerValue = player.getHandValue();
    int dealerValue = dealer.getHandValue();
    size_t playerCards = player.getHandSize();
    size_t dealerCards = dealer.getHandSize();

    bool playerBJ = hasBlackjack(playerValue, playerCards);
    bool dealerBJ = hasBlackjack(dealerValue, dealerCards);

    cout << "\n=== RESULTADO ===" << endl;
    cout << "Jugador: " << playerValue << endl;
    cout << "Dealer: " << dealerValue << endl;

    // CASO ESPECIAL: Ambos tienen Blackjack
    if (playerBJ && dealerBJ) {
        cout << "¡EMPATE! Ambos tienen Blackjack. Recuperas tu apuesta." << endl;
        // No se gana ni se pierde
        ties++;
        currentStreak=0;
    }
    // CASO ESPECIAL: Solo jugador tiene Blackjack
    else if (playerBJ && !dealerBJ) {
        cout << "¡¡¡BLACKJACK NATURAL!!! Ganas 1.5x tu apuesta 🎰💰" << endl;
        int winnings = currentBet + (currentBet * 3 / 2); // Apuesta + 1.5x
        playerMoney =currentBet + winnings;
        cout << "Ganaste $" << (currentBet + winnings) << endl;
        wins++;
        blackjacksHit++;
        currentStreak= (currentStreak >= 0) ? currentStreak +1 : 1;
        if (currentStreak > bestStreak ) bestStreak=currentStreak;
    }
    // CASO ESPECIAL: Solo dealer tiene Blackjack
    else if (dealerBJ && !playerBJ) {
        cout << "El dealer tiene Blackjack. Pierdes." << endl;
        playerMoney -= currentBet;
        losses++;
        currentStreak = (currentStreak <= 0) ? currentStreak - 1 : -1;
    }
    // Casos normales (sin Blackjack natural)
    else if (playerValue > 21) {
        cout << "¡Perdiste! Te pasaste de 21." << endl;
        playerMoney -= currentBet;
        losses++;
        currentStreak = (currentStreak <= 0) ? currentStreak - 1 : -1;
    }
    else if (dealerValue > 21) {
        cout << "¡GANASTE! El dealer se pasó." << endl;
        playerMoney += currentBet;
        wins++;
        currentStreak = (currentStreak >= 0) ? currentStreak + 1 : 1;
        if (currentStreak > bestStreak) bestStreak = currentStreak;
    }
    else if (playerValue > dealerValue) {
        cout << "¡GANASTE! Tu mano es mejor." << endl;
        playerMoney += currentBet;
        wins++;
        currentStreak = (currentStreak >= 0) ? currentStreak + 1 : 1;
        if (currentStreak > bestStreak) bestStreak = currentStreak;
    }
    else if (dealerValue > playerValue) {
        cout << "Perdiste. La mano del dealer es mejor." << endl;
        playerMoney -= currentBet;
        losses++;
        currentStreak = (currentStreak <= 0) ? currentStreak - 1 : -1;
    }
    else {
        cout << "¡EMPATE! Recuperas tu apuesta." << endl;
        ties++;
        currentStreak=0;
    }

    if (playerMoney > maxMoney) {
        maxMoney=playerMoney;
    }
    cout << "Dinero actual: $" << playerMoney << endl << endl;
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






