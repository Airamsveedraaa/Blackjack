#include <iostream>
#include "include/Game.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);  // Para Windows

    Game game("Jugador", 1000);

    while (game.canContinue()) {
        game.playRound();

        cout << "¿Quieres jugar otra ronda? (S/N): ";
        char continuar;
        cin >> continuar;
        cin.ignore();

        if (toupper(continuar) != 'S') {
            break;
        }
    }

    cout << "\n¡Gracias por jugar!" << endl;
    cout << "Dinero final: $" << game.getPlayerMoney() << endl;

    return 0;
}