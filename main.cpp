#include "include/Deck.h"
#include "include/Player.h"
#include "include/Dealer.h"

using namespace std;

int main() {
    cout << "=== TEST BLACKJACK CLASSES ===" << endl << endl;

    // Test 1: Crear y barajar baraja
    cout << "TEST 1: Deck" << endl;
    Deck deck;
    deck.shuffle();
    cout << "✓ Deck creado y barajado" << endl << endl;

    // Test 2: Crear jugador y dealer
    cout << "TEST 2: Player y Dealer" << endl;
    Player player("Alice");
    Dealer dealer;
    cout << "✓ Player y Dealer creados" << endl << endl;

    // Test 3: Repartir cartas
    cout << "TEST 3: Repartir cartas iniciales" << endl;
    player.addCard(deck.dealCard());
    player.addCard(deck.dealCard());
    dealer.addCard(deck.dealCard());
    dealer.addCard(deck.dealCard());
    cout << "✓ Cartas repartidas" << endl << endl;

    // Test 4: Mostrar manos
    cout << "TEST 4: Mostrar manos" << endl;
    player.showHand();
    dealer.showHand(true);  // Ocultar primera carta
    cout << endl;

    // Test 5: Jugador pide carta
    cout << "TEST 5: Jugador pide una carta más" << endl;
    player.addCard(deck.dealCard());
    player.showHand();
    cout << endl;

    // Test 6: Revelar mano del dealer
    cout << "TEST 6: Revelar mano completa del dealer" << endl;
    dealer.showHand(false);  // Mostrar todas
    cout << endl;

    // Test 7: Lógica shouldHit del dealer
    cout << "TEST 7: Dealer shouldHit?" << endl;
    cout << "Valor del dealer: " << dealer.getHandValue() << endl;
    cout << "¿Debe pedir carta? " << (dealer.shouldHit() ? "SÍ" : "NO") << endl;

    while (dealer.shouldHit()) {
        cout << "Dealer pide carta..." << endl;
        dealer.addCard(deck.dealCard());
        dealer.showHand(false);
    }
    cout << endl;

    // Test 8: Determinar ganador
    cout << "TEST 8: Resultado final" << endl;
    int playerValue = player.getHandValue();
    int dealerValue = dealer.getHandValue();

    cout << "Jugador: " << playerValue << endl;
    cout << "Dealer: " << dealerValue << endl;

    if (playerValue > 21) {
        cout << "¡Jugador se pasó! Dealer gana" << endl;
    } else if (dealerValue > 21) {
        cout << "¡Dealer se pasó! Jugador gana" << endl;
    } else if (playerValue > dealerValue) {
        cout << "¡Jugador gana!" << endl;
    } else if (dealerValue > playerValue) {
        cout << "¡Dealer gana!" << endl;
    } else {
        cout << "¡Empate!" << endl;
    }

    cout << endl << "=== FIN DE LOS TESTS ===" << endl;

    return 0;
}