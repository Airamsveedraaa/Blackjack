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

    if (hasBlackjack(player.getHandValue(),player.getHandSize())) {
        return;
    }

        while (player.getHandValue() < 21) {
            cout << "¿Quieres pedir carta? (S/N): ";
            cin >> resp;
            cin.ignore();  // ← IMPORTANTE: Limpiar buffer

            if (toupper(resp) == 'S') {
                player.addCard(deck.dealCard());
                cout << "\n--- Nueva carta ---" << endl;
                player.showHand();

                if (player.getHandValue() > 21) {
                    cout << "¡TE PASASTE! Pierdes esta ronda." << endl;
                    return;
                }
            }
            else if (toupper(resp) == 'N') {
                cout << "Te plantas con " << player.getHandValue() << endl;
                return;  // ← Sale del método
            }
            else {
                cout << "Opción no válida. Usa S o N." << endl;
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
    }
    // CASO ESPECIAL: Solo jugador tiene Blackjack
    else if (playerBJ) {
        cout << "¡¡¡BLACKJACK NATURAL!!! Ganas 1.5x tu apuesta 🎰💰" << endl;
        int winnings = currentBet + (currentBet * 3 / 2); // Apuesta + 1.5x
        playerMoney += winnings;
        cout << "Ganaste $" << winnings << endl;
    }
    // CASO ESPECIAL: Solo dealer tiene Blackjack
    else if (dealerBJ) {
        cout << "El dealer tiene Blackjack. Pierdes." << endl;
        playerMoney -= currentBet;
    }
    // Casos normales (sin Blackjack natural)
    else if (playerValue > 21) {
        cout << "¡Perdiste! Te pasaste de 21." << endl;
        playerMoney -= currentBet;
    }
    else if (dealerValue > 21) {
        cout << "¡GANASTE! El dealer se pasó." << endl;
        playerMoney += currentBet;
    }
    else if (playerValue > dealerValue) {
        cout << "¡GANASTE! Tu mano es mejor." << endl;
        playerMoney += currentBet;
    }
    else if (dealerValue > playerValue) {
        cout << "Perdiste. La mano del dealer es mejor." << endl;
        playerMoney -= currentBet;
    }
    else {
        cout << "¡EMPATE! Recuperas tu apuesta." << endl;
    }

    cout << "Dinero actual: $" << playerMoney << endl << endl;
}


bool Game::hasBlackjack(int handValue, size_t numCards) const {
    return handValue==21 && numCards==2;
}







