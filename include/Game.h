//
// Created by Airam on 20/10/2025.
//

#ifndef BLACKJACK_GAME_H
#define BLACKJACK_GAME_H
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include "Comun.h"

enum class GameState {
    BETTING,
    DEALING,
    PLAYER_TURN,
    DEALER_TURN,
    ROUND_END
};

class Game {
private:
    Deck deck;
    Player player;
    Dealer dealer;
    int playerMoney;
    int currentBet;
    GameState state;
    bool hasBlackjack(int handValue, size_t numCards)const;

public:
    Game(const char* playerName, int initialMoney = 1000);

    void playRound();           // Ejecuta una ronda completa
    void dealInitialCards();    // Reparte 2 cartas a cada uno
    void playerTurn();          // Turno del jugador (Hit/Stand)
    void dealerTurn();          // Turno del dealer (automático)
    void determineWinner();     // Calcula y muestra ganador
    void resetRound();          // Limpia manos y prepara nueva ronda

    bool canContinue() const;   // ¿Tiene dinero el jugador?
    int getPlayerMoney() const{return this->playerMoney;}; // Getter del dinero
};

#endif //BLACKJACK_GAME_H