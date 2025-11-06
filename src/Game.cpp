//
// Created by Airam on 20/10/2025.
//

#include "../include/Game.h"

Game::Game(const char* PlayerName, int initialMoney)
:player(PlayerName),
dealer(),
deck() {

    playerMoney = initialMoney;
    currentBet = 0;
    state=GameState::BETTING;
    deck.shuffle();

    cout << "¡Bienvenido a la mesa de Blackjack, " << PlayerName << "!" << endl;
    cout << "Dinero inicial: $" << playerMoney << endl << endl;
}

bool Game::canContinue() const {

    if (playerMoney > 0)
        return true;
    return false;
}


void Game::resetRound() {
    player.clearHand();
    dealer.clearHand();
    Game::state=GameState::BETTING; //reset de ronda -> volver a tiempo de apuestas
}


void Game::dealInitialCards() {

    player.addCard(deck.dealCard());
    player.addCard(deck.dealCard());
    dealer.addCard(deck.dealCard());
    dealer.addCard(deck.dealCard());
    player.showHand();
    dealer.showHand();
    // 2 cartas para cada uno inicialmente y se enseñan
}

void Game::playerTurn() {
    char resp;
    cout << "¿Quieres una carta más? S|N " << endl;
    cin >> resp;
    if (toupper(resp)=='S')
        player.addCard(deck.dealCard());
    else {
        dealerTurn();
    }
}

void Game::dealerTurn() {
    if (dealer.getHandValue() < 17)
        dealer.addCard(deck.dealCard());
}

void Game::playRound() {
}

void Game::determineWinner() {

}







