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

    //ESTADISTICAS
    int initialMoney;        // Dinero inicial para calcular beneficio
    int wins;                // Victorias
    int losses;              // Derrotas
    int ties;                // Empates
    int currentStreak;       // Racha actual (positivo = victorias, negativo = derrotas)
    int bestStreak;          // Mejor racha de victorias
    int blackjacksHit;       // Blackjacks naturales conseguidos
    int maxMoney;            // Dinero máximo alcanzado
    int totalBet;            // Total apostado

public:
    Game(const char* playerName, int initialMoney = 1000);

    void playRound();           // Ejecuta una ronda completa
    void dealInitialCards();    // Reparte 2 cartas a cada uno
    void playerTurn();          // Turno del jugador (Hit/Stand)
    void dealerTurn();          // Turno del dealer (automático)
    void determineWinner();     // Calcula y muestra ganador
    void resetRound();          // Limpia manos y prepara nueva ronda

    void showStatistics() const;
    void updateStats(bool won);

    bool canContinue() const;   // ¿Tiene dinero el jugador?
    int getPlayerMoney() const{return this->playerMoney;}; // Getter del dinero
};

#endif //BLACKJACK_GAME_H