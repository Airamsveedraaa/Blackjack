//
// Created by Airam on 20/10/2025.
//

#ifndef BLACKJACK_GAME_H
#define BLACKJACK_GAME_H
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include "Comun.h"

//Estructura para devolver resultado de ronda
struct RoundResult {
    enum class Outcome {
        PLAYER_WIN,
        DEALER_WIN,
        TIE,
        PLAYER_BLACKJACK,
        DEALER_BLACKJACK,
        BOTH_BLACKJACK,
        PLAYER_BUST,
        DEALER_BUST
    };

    Outcome outcome;
    int moneyChange; //Dinero ganado/perdido
    int playerValue;
    int dealerValue;
    bool wasBlackjack;
};


//Resultado de acción del jugador
enum class PlayerActionResult {
    CONTINUE, //Puede seguir jugando
    BUST, //Se pasó de 21
    STAND, //Se plantó
    BLACKJACK //Tiene 21
};


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

    // Estadísticas
    int initialMoney;
    int wins;
    int losses;
    int ties;
    int currentStreak;
    int bestStreak;
    int blackjacksHit;
    int maxMoney;
    int totalBet;

    // Métodos privados de lógica pura
    bool hasBlackjack(int handValue, size_t numCards) const;
    void updateStatistics(RoundResult::Outcome outcome);

public:
    Game(const char* playerName, int initialMoney = 1000);

    // === MÉTODOS PARA GUI ===

    // Estado del juego
    GameState getState() const { return state; }
    int getPlayerMoney() const { return playerMoney; }
    int getCurrentBet() const { return currentBet; }
    bool canContinue() const { return playerMoney > 0; }

    // Acceso a jugadores
    const Player& getPlayer() const { return player; }
    const Dealer& getDealer() const { return dealer; }

    // Estadísticas
    int getWins() const { return wins; }
    int getLosses() const { return losses; }
    int getTies() const { return ties; }
    int getCurrentStreak() const { return currentStreak; }
    int getBestStreak() const { return bestStreak; }
    int getBlackjacksHit() const { return blackjacksHit; }
    int getMaxMoney() const { return maxMoney; }
    int getTotalBet() const { return totalBet; }
    double getWinRate() const;

    // === MÉTODOS DE JUEGO (sin cout/cin) ===

    // Iniciar ronda
    bool placeBet(int amount);           // Devuelve true si apuesta válida
    void startRound();                   // Reparte cartas iniciales

    // Acciones del jugador
    PlayerActionResult playerHit();      // Jugador pide carta
    PlayerActionResult playerStand();    // Jugador se planta
    PlayerActionResult playerDoubleDown();  // Jugador dobla

    // Turno del dealer (automático)
    void playDealerTurn();               // Dealer juega automáticamente

    // Determinar ganador y resetear
    RoundResult finishRound();           // Calcula ganador, devuelve resultado
    void resetForNewRound();             // Limpia para nueva ronda

    // === MÉTODOS PARA CONSOLA (temporal) ===
    void showStatistics() const;         // Mantener por ahora
    void playRound();                    // Mantener para consola
};

#endif //BLACKJACK_GAME_H