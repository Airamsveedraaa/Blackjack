#include <iostream>
#include "include/Game.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Game game("Jugador", 1000);

    while (game.canContinue()) {
        game.playRound();

        cout << "Opciones: [J]ugar otra ronda | [E]stadísticas | [S]alir: ";
        char opcion;
        cin >> opcion;
        cin.ignore();
        opcion = toupper(opcion);

        if (opcion == 'E') {
            game.showStatistics();
            cout << "Presiona Enter para continuar...";
            cin.get();
        }
        else if (opcion == 'S') {
            break;
        }
        // Si es 'J' o cualquier otra cosa, continúa el bucle
    }

    cout << "\n=== PARTIDA FINALIZADA ===" << endl;
    game.showStatistics();
    cout << "¡Gracias por jugar!" << endl;

    return 0;
}