#include <iostream>
#include <windows.h>  // Para UTF-8 en Windows
#include "include/Game.h"

using namespace std;

int main() {
    // Configurar UTF-8 para que se vean bien los símbolos
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    cout << "╔════════════════════════════════════╗" << endl;
    cout << "║     BIENVENIDO AL BLACKJACK        ║" << endl;
    cout << "╚════════════════════════════════════╝" << endl << endl;

    // Crear juego
    cout << "Introduce tu nombre: ";
    char nombre[50];
    cin.getline(nombre, 50);

    cout << "¿Con cuánto dinero quieres empezar? (por defecto $1000): ";
    string input;
    getline(cin, input);

    int dineroInicial = 1000;
    if (!input.empty()) {
        try {
            dineroInicial = stoi(input);
            if (dineroInicial <= 0) dineroInicial = 1000;
        } catch (...) {
            dineroInicial = 1000;
        }
    }

    Game game(nombre, dineroInicial);

    // Bucle principal
    while (game.canContinue()) {
        game.playRound();

        if (!game.canContinue()) {
            cout << "\n¡TE HAS QUEDADO SIN DINERO!" << endl;
            break;
        }

        cout << "\n╔════════════════════════════════════╗" << endl;
        cout << "║ [J] Jugar otra ronda               ║" << endl;
        cout << "║ [E] Ver estadísticas               ║" << endl;
        cout << "║ [S] Salir                          ║" << endl;
        cout << "╚════════════════════════════════════╝" << endl;
        cout << "Elige una opción: ";

        char opcion;
        cin >> opcion;
        cin.ignore();
        opcion = toupper(opcion);

        if (opcion == 'E') {
            game.showStatistics();
            cout << "\nPresiona Enter para continuar...";
            cin.get();
        }
        else if (opcion == 'S') {
            cout << "\n¿Seguro que quieres salir? (S/N): ";
            char confirmar;
            cin >> confirmar;
            cin.ignore();

            if (toupper(confirmar) == 'S') {
                break;
            }
        }
        // Si es 'J' o cualquier otra cosa, continúa el bucle
    }

    // Resumen final
    cout << "\n╔════════════════════════════════════╗" << endl;
    cout << "║     PARTIDA FINALIZADA             ║" << endl;
    cout << "╚════════════════════════════════════╝" << endl;

    game.showStatistics();

    cout << "\n¡Gracias por jugar al Blackjack!" << endl;
    cout << "Presiona Enter para salir...";
    cin.get();

    return 0;
}