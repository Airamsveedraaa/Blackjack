//
// Tests automaticos de la logica del juego (clase Game), sin interfaz grafica.
// Ejecutar el target Blackjack_Tests: devuelve 0 si todo pasa y 1 si algun test falla.
//
// Cada test sigue el patron: preparar -> ejecutar -> comprobar.
// Para no depender del azar, Game::stackDeck fija las proximas cartas del mazo.
// Orden de reparto en startRound: jugador, dealer (tapada), jugador, dealer (visible),
// y despues las cartas que se pidan (primero las del jugador y luego las del dealer).
//

#include "../include/Game.h"
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace {

// Game escribe un saludo por std::cout al crearse; los tests escriben por 'out'
// y main() manda std::cout a un buffer descartado para que la salida se lea limpia.
std::ostream out(std::cout.rdbuf());

int checks = 0;
int fallos = 0;

void check(bool ok, const std::string& nombre) {
    checks++;
    if (!ok) {
        fallos++;
        out << "  FALLO  " << nombre << std::endl;
    }
}

void seccion(const std::string& nombre) {
    out << "- " << nombre << std::endl;
}

Card c(Rank r) {
    return Card(Suit::Spades, r);
}

using R = Rank;
using O = RoundResult::Outcome;

// Juega la ronda hasta el final plantandose (si aun es el turno del jugador)
RoundResult plantarseYTerminar(Game& g) {
    if (g.getState() == GameState::PLAYER_TURN) {
        g.playerStand();
    }
    g.playDealerTurn();
    return g.finishRound();
}

// ---------------------------------------------------------------- apuestas

void testCanContinue() {
    seccion("canContinue y apuesta minima");
    Game sinDinero("T", 0);
    Game cinco("T", 5);
    Game diez("T", 10);
    check(!sinDinero.canContinue(), "con 0 $ no se puede seguir");
    check(!cinco.canContinue(), "con 5 $ (menos que la apuesta minima) no se puede seguir");
    check(diez.canContinue(), "con 10 $ se puede seguir");
}

void testPlaceBet() {
    seccion("placeBet");
    Game g("T", 1000);
    check(!g.placeBet(0), "una apuesta de 0 se rechaza");
    check(!g.placeBet(-50), "una apuesta negativa se rechaza");
    check(!g.placeBet(1001), "no se puede apostar mas de lo que se tiene");
    check(g.placeBet(1000), "se puede apostar todo (ALL IN)");
    check(g.getCurrentBet() == 1000, "la apuesta queda guardada");
    check(g.getPlayerMoney() == 1000, "el dinero no se descuenta hasta el final de la ronda");
}

// ---------------------------------------------------------------- blackjack natural y peek

void testBlackjackNatural() {
    seccion("blackjack natural");
    Game g("T", 1000);
    g.stackDeck({c(R::ACE), c(R::NINE), c(R::KING), c(R::SEVEN)});
    g.placeBet(100);
    g.startRound();
    check(g.getState() == GameState::ROUND_END, "con blackjack natural la ronda termina en el reparto");
    RoundResult r = g.finishRound();
    check(r.outcome == O::PLAYER_BLACKJACK, "el resultado es PLAYER_BLACKJACK");
    check(r.moneyChange == 150, "paga 3:2 (+150 por 100)");
    check(g.getPlayerMoney() == 1150, "el dinero sube a 1150");
    check(g.getBlackjacksHit() == 1 && g.getWins() == 1, "cuenta como blackjack y como victoria");
}

void testAmbosBlackjack() {
    seccion("blackjack del jugador y del dealer");
    Game g("T", 1000);
    g.stackDeck({c(R::ACE), c(R::KING), c(R::QUEEN), c(R::ACE)});
    g.placeBet(100);
    g.startRound();
    check(!g.canOfferInsurance(), "con blackjack del jugador no se ofrece seguro");
    check(g.getState() == GameState::ROUND_END, "la ronda termina en el reparto");
    RoundResult r = g.finishRound();
    check(r.outcome == O::BOTH_BLACKJACK && r.moneyChange == 0, "empate: BOTH_BLACKJACK y 0 $");
    check(g.getTies() == 1, "cuenta como empate");
}

void testPeekConDiezVisible() {
    seccion("dealer peek con un 10 visible");
    Game g("T", 1000);
    g.stackDeck({c(R::NINE), c(R::ACE), c(R::SEVEN), c(R::KING)});
    g.placeBet(100);
    g.startRound();
    check(!g.canOfferInsurance(), "sin As visible no se ofrece seguro");
    check(g.getState() == GameState::ROUND_END, "el blackjack del dealer termina la ronda en el reparto");
    g.playerHit();
    g.playerDoubleDown();
    check(g.getPlayer().getHandSize() == 2, "despues del peek no se puede pedir carta");
    check(g.getCurrentBet() == 100, "despues del peek no se puede doblar");
    RoundResult r = g.finishRound();
    check(r.outcome == O::DEALER_BLACKJACK && r.moneyChange == -100, "se pierde solo la apuesta inicial");
}

// ---------------------------------------------------------------- seguro

void testSeguroSeOfreceYEspera() {
    seccion("seguro: se ofrece con As visible y la ronda espera");
    Game g("T", 1000);
    g.stackDeck({c(R::NINE), c(R::KING), c(R::SEVEN), c(R::ACE)});
    g.placeBet(100);
    g.startRound();
    check(g.canOfferInsurance(), "con As visible se ofrece seguro");
    check(g.getState() == GameState::PLAYER_TURN, "mientras se decide, el dealer aun no ha mirado (no hay ROUND_END)");
}

void testSeguroYesConBlackjack() {
    seccion("seguro: YES y el dealer tiene blackjack");
    Game g("T", 1000);
    g.stackDeck({c(R::NINE), c(R::KING), c(R::SEVEN), c(R::ACE)});
    g.placeBet(100);
    g.startRound();
    g.takeInsurance(true);
    check(g.getState() == GameState::ROUND_END, "despues de responder se hace el peek y termina la ronda");
    RoundResult r = g.finishRound();
    check(r.outcome == O::DEALER_BLACKJACK, "el resultado es DEALER_BLACKJACK");
    check(r.insuranceChange == 100, "el seguro paga 2:1 (+100 por 50)");
    check(r.moneyChange == 0 && g.getPlayerMoney() == 1000, "en total se queda igual (-100 + 100)");
    check(g.getTotalBet() == 150, "el seguro cuenta en el total apostado");
}

void testSeguroNoConBlackjack() {
    seccion("seguro: NO y el dealer tiene blackjack");
    Game g("T", 1000);
    g.stackDeck({c(R::NINE), c(R::KING), c(R::SEVEN), c(R::ACE)});
    g.placeBet(100);
    g.startRound();
    g.takeInsurance(false);
    RoundResult r = g.finishRound();
    check(r.insuranceChange == 0, "sin seguro no hay cambio por seguro");
    check(r.moneyChange == -100 && g.getPlayerMoney() == 900, "se pierden los 100 de la apuesta");
}

void testSeguroYesSinBlackjack() {
    seccion("seguro: YES y el dealer no tiene blackjack");
    Game g("T", 1000);
    // jugador 10+10=20, dealer 7 (tapada) + A = 18, se planta
    g.stackDeck({c(R::TEN), c(R::SEVEN), c(R::TEN), c(R::ACE)});
    g.placeBet(100);
    g.startRound();
    g.takeInsurance(true);
    check(g.getState() == GameState::PLAYER_TURN, "sin blackjack del dealer, despues de responder le toca al jugador");
    RoundResult r = plantarseYTerminar(g);
    check(r.outcome == O::PLAYER_WIN, "el jugador gana 20 contra 18");
    check(r.insuranceChange == -50, "el seguro se pierde (-50)");
    check(r.moneyChange == 50 && g.getPlayerMoney() == 1050, "en total +50 (+100 - 50)");
}

void testSeguroNoSinBlackjack() {
    seccion("seguro: NO y el dealer no tiene blackjack");
    Game g("T", 1000);
    g.stackDeck({c(R::TEN), c(R::SEVEN), c(R::TEN), c(R::ACE)});
    g.placeBet(100);
    g.startRound();
    g.takeInsurance(false);
    RoundResult r = plantarseYTerminar(g);
    check(r.moneyChange == 100 && g.getPlayerMoney() == 1100, "+100, sin nada de seguro");
}

void testSeguroCasosSinOferta() {
    seccion("seguro: cuando no se ofrece");
    {
        Game g("T", 120);  // apuesta 100 + seguro 50 = 150 > 120
        g.stackDeck({c(R::NINE), c(R::KING), c(R::SEVEN), c(R::ACE)});
        g.placeBet(100);
        g.startRound();
        check(!g.canOfferInsurance(), "no se ofrece si no llega el dinero para la apuesta mas el seguro");
    }
    {
        Game g("T", 1000);
        g.stackDeck({c(R::ACE), c(R::NINE), c(R::KING), c(R::ACE)});
        g.placeBet(100);
        g.startRound();
        check(!g.canOfferInsurance(), "no se ofrece si el jugador tiene blackjack");
        RoundResult r = g.finishRound();
        check(r.outcome == O::PLAYER_BLACKJACK && r.moneyChange == 150, "y el blackjack se paga 3:2");
    }
}

// ---------------------------------------------------------------- dealer

void testDealerPideCon16() {
    seccion("dealer: pide carta con 16");
    Game g("T", 1000);
    g.stackDeck({c(R::TEN), c(R::TEN), c(R::NINE), c(R::SIX), c(R::FIVE)});
    g.placeBet(100);
    g.startRound();
    RoundResult r = plantarseYTerminar(g);
    check(g.getDealer().getHandSize() == 3, "con 16 el dealer pide una carta");
    check(r.dealerValue == 21 && r.outcome == O::DEALER_WIN, "llega a 21 y gana a 19");
}

void testDealerSePlantaConBlando17() {
    seccion("dealer: se planta con 17 blando (As + 6)");
    Game g("T", 1000);
    g.stackDeck({c(R::TEN), c(R::SIX), c(R::NINE), c(R::ACE)});
    g.placeBet(100);
    g.startRound();
    g.takeInsurance(false);
    RoundResult r = plantarseYTerminar(g);
    check(g.getDealer().getHandSize() == 2, "con 17 blando el dealer no pide");
    check(r.outcome == O::PLAYER_WIN && r.moneyChange == 100, "el jugador gana 19 contra 17");
}

void testDealerSePasa() {
    seccion("dealer: se pasa");
    Game g("T", 1000);
    g.stackDeck({c(R::TEN), c(R::TEN), c(R::EIGHT), c(R::SIX), c(R::KING)});
    g.placeBet(100);
    g.startRound();
    RoundResult r = plantarseYTerminar(g);
    check(r.outcome == O::DEALER_BUST && r.moneyChange == 100, "DEALER_BUST y +100");
}

// ---------------------------------------------------------------- jugador

void testJugadorSePasa() {
    seccion("jugador: se pasa");
    Game g("T", 1000);
    g.stackDeck({c(R::TEN), c(R::TEN), c(R::SIX), c(R::SEVEN), c(R::KING)});
    g.placeBet(100);
    g.startRound();
    check(g.playerHit() == PlayerActionResult::BUST, "10 + 6 + K = 26: BUST");
    g.playDealerTurn();
    RoundResult r = g.finishRound();
    check(g.getDealer().getHandSize() == 2, "si el jugador se pasa, el dealer no pide");
    check(r.outcome == O::PLAYER_BUST && r.moneyChange == -100, "PLAYER_BUST y -100");
}

void testEmpate() {
    seccion("empate");
    Game g("T", 1000);
    g.stackDeck({c(R::TEN), c(R::TEN), c(R::EIGHT), c(R::EIGHT)});
    g.placeBet(100);
    g.startRound();
    RoundResult r = plantarseYTerminar(g);
    check(r.outcome == O::TIE && r.moneyChange == 0, "18 contra 18: TIE y 0 $");
    check(g.getTies() == 1 && g.getCurrentStreak() == 0, "cuenta como empate y corta la racha");
}

void testAsQueCambiaDeValor() {
    seccion("el As vale 1 u 11");
    Game g("T", 1000);
    g.stackDeck({c(R::ACE), c(R::TEN), c(R::ACE), c(R::SEVEN), c(R::NINE)});
    g.placeBet(100);
    g.startRound();
    check(g.getPlayer().getHandValue() == 12, "A + A = 12");
    check(g.playerHit() == PlayerActionResult::BLACKJACK, "A + A + 9 = 21");
    RoundResult r = plantarseYTerminar(g);
    check(r.outcome == O::PLAYER_WIN && r.moneyChange == 100, "21 con 3 cartas no es blackjack natural: paga 1:1");
    check(g.getBlackjacksHit() == 0, "no cuenta como blackjack");
}

// ---------------------------------------------------------------- doblar

void testDoblar() {
    seccion("doblar");
    Game g("T", 1000);
    g.stackDeck({c(R::FIVE), c(R::TEN), c(R::SIX), c(R::SEVEN), c(R::TEN)});
    g.placeBet(100);
    g.startRound();
    g.playerDoubleDown();
    check(g.getCurrentBet() == 200, "la apuesta se duplica");
    check(g.getPlayer().getHandSize() == 3, "se recibe exactamente una carta");
    check(g.getState() == GameState::DEALER_TURN, "despues de doblar le toca al dealer");
    g.playDealerTurn();
    RoundResult r = g.finishRound();
    check(r.outcome == O::PLAYER_WIN && r.moneyChange == 200, "21 contra 17 gana el doble (+200)");
    check(g.getTotalBet() == 200, "el total apostado incluye el doble");
}

void testDoblarSoloConDosCartas() {
    seccion("doblar: solo con las dos primeras cartas");
    Game g("T", 1000);
    g.stackDeck({c(R::TWO), c(R::TEN), c(R::THREE), c(R::SEVEN), c(R::FOUR)});
    g.placeBet(100);
    g.startRound();
    g.playerHit();
    g.playerDoubleDown();
    check(g.getCurrentBet() == 100, "con 3 cartas no se puede doblar");
}

void testDoblarNecesitaDinero() {
    seccion("doblar: hace falta dinero para la apuesta doble");
    Game g("T", 150);
    g.stackDeck({c(R::FIVE), c(R::TEN), c(R::SIX), c(R::SEVEN), c(R::TWO)});
    g.placeBet(100);
    g.startRound();
    check(!g.canDoubleDown(), "canDoubleDown dice que no");
    g.playerDoubleDown();
    check(g.getCurrentBet() == 100, "con 150 $ y 100 apostados no se puede doblar a 200");

    // con seguro: doblar deja apostado 200 + 50 de seguro = 250
    Game justo("T", 250);
    justo.stackDeck({c(R::FIVE), c(R::SEVEN), c(R::SIX), c(R::ACE), c(R::TEN)});
    justo.placeBet(100);
    justo.startRound();
    justo.takeInsurance(true);
    check(justo.canDoubleDown(), "con 250 $, apuesta 100 y seguro 50 se puede doblar (justo 250)");

    Game corto("T", 240);
    corto.stackDeck({c(R::FIVE), c(R::SEVEN), c(R::SIX), c(R::ACE), c(R::TEN)});
    corto.placeBet(100);
    corto.startRound();
    corto.takeInsurance(true);
    corto.playerDoubleDown();
    check(corto.getCurrentBet() == 100, "con 240 $, apuesta 100 y seguro 50 no se puede doblar (harian falta 250)");
}

// ---------------------------------------------------------------- estadisticas y rondas seguidas

void testEstadisticasYRachas() {
    seccion("estadisticas y rachas");
    Game g("T", 1000);
    const std::vector<std::vector<Card>> rondas = {
        {c(R::TEN), c(R::TEN), c(R::NINE), c(R::SEVEN)},  // 19 contra 17: gana
        {c(R::TEN), c(R::TEN), c(R::NINE), c(R::SEVEN)},  // gana
        {c(R::TEN), c(R::TEN), c(R::SIX), c(R::EIGHT)},   // 16 contra 18: pierde
    };
    for (const auto& cartas : rondas) {
        g.stackDeck(cartas);
        g.placeBet(100);
        g.startRound();
        plantarseYTerminar(g);
        g.resetForNewRound();
    }
    check(g.getWins() == 2 && g.getLosses() == 1, "2 victorias y 1 derrota");
    check(g.getBestStreak() == 2 && g.getCurrentStreak() == 0, "mejor racha 2, racha actual 0");
    check(g.getMaxMoney() == 1200, "dinero maximo 1200");
    check(g.getPlayerMoney() == 1100, "dinero final 1100");
    check(g.getWinRate() > 66.6 && g.getWinRate() < 66.7, "win rate 66.7 %");
    check(g.getTotalBet() == 300, "total apostado 300");
}

void testResetForNewRound() {
    seccion("resetForNewRound");
    Game g("T", 1000);
    g.stackDeck({c(R::TEN), c(R::KING), c(R::SEVEN), c(R::ACE)});
    g.placeBet(100);
    g.startRound();
    g.takeInsurance(true);
    plantarseYTerminar(g);
    g.resetForNewRound();
    check(g.getPlayer().getHandSize() == 0 && g.getDealer().getHandSize() == 0, "las manos quedan vacias");
    check(g.getCurrentBet() == 0 && g.getState() == GameState::BETTING, "apuesta a 0 y estado BETTING");
    // si el seguro no se limpiara, contaria en la ronda siguiente
    g.stackDeck({c(R::TEN), c(R::TEN), c(R::NINE), c(R::SEVEN)});
    g.placeBet(100);
    g.startRound();
    RoundResult r = plantarseYTerminar(g);
    check(r.insuranceChange == 0 && r.moneyChange == 100, "el seguro no pasa a la ronda siguiente");
}

// ---------------------------------------------------------------- propiedades (rondas al azar)

void testPropiedadesEnRondasAlAzar() {
    seccion("propiedades en 2000 partidas al azar");
    std::mt19937 rng(12345);
    int rondas = 0, dineroNoCuadra = 0, dineroNegativo = 0, segurosSinAs = 0, estadoFinalMal = 0;
    int statsNoCuadran = 0, excepciones = 0;

    for (int partida = 0; partida < 2000; ++partida) {
        Game g("T", 1000);
        int jugadas = 0;
        while (g.canContinue() && jugadas < 100) {
            jugadas++;
            try {
                int apuestas[] = {10, 50, 100, g.getPlayerMoney()};
                int apuesta = apuestas[rng() % 4];
                if (apuesta > g.getPlayerMoney()) apuesta = g.getPlayerMoney();
                int antes = g.getPlayerMoney();

                g.placeBet(apuesta);
                g.startRound();

                if (g.canOfferInsurance()) {
                    const auto& d = g.getDealer().getHand();
                    if (d.size() < 2 || d[1].getrank() != Rank::ACE) segurosSinAs++;
                    g.takeInsurance(rng() % 2);
                }

                bool sigue = true;
                while (sigue && g.getState() == GameState::PLAYER_TURN) {
                    switch (rng() % 3) {
                        case 0: {
                            PlayerActionResult a = g.playerHit();
                            if (a == PlayerActionResult::BUST) sigue = false;
                            else if (a == PlayerActionResult::BLACKJACK) g.playerStand();  // con 21 se planta
                            break;
                        }
                        case 1:
                            g.playerStand();
                            break;
                        default:
                            g.playerDoubleDown();
                            if (g.getState() == GameState::PLAYER_TURN) g.playerStand();
                            break;
                    }
                }
                g.playDealerTurn();
                RoundResult r = g.finishRound();
                rondas++;

                if (g.getPlayerMoney() - antes != r.moneyChange) dineroNoCuadra++;
                if (g.getPlayerMoney() < 0) dineroNegativo++;
                if (g.getState() != GameState::ROUND_END) estadoFinalMal++;
                g.resetForNewRound();
            } catch (...) {
                excepciones++;
                break;
            }
        }
        if (g.getWins() + g.getLosses() + g.getTies() != jugadas && excepciones == 0) statsNoCuadran++;
    }

    check(excepciones == 0, "ninguna ronda lanza excepciones (" + std::to_string(excepciones) + ")");
    check(dineroNoCuadra == 0, "moneyChange coincide siempre con el dinero que se mueve (" + std::to_string(dineroNoCuadra) + " rondas mal)");
    check(dineroNegativo == 0, "el dinero nunca es negativo (" + std::to_string(dineroNegativo) + " rondas mal)");
    check(segurosSinAs == 0, "el seguro solo se ofrece con un As visible");
    check(estadoFinalMal == 0, "finishRound siempre deja la ronda en ROUND_END");
    check(statsNoCuadran == 0, "victorias + derrotas + empates = rondas jugadas");
    out << "  (" << rondas << " rondas jugadas)" << std::endl;
}

}  // namespace

int main() {
    std::ostringstream descartado;
    std::streambuf* original = std::cout.rdbuf(descartado.rdbuf());

    out << "Tests de Game" << std::endl;

    testCanContinue();
    testPlaceBet();
    testBlackjackNatural();
    testAmbosBlackjack();
    testPeekConDiezVisible();
    testSeguroSeOfreceYEspera();
    testSeguroYesConBlackjack();
    testSeguroNoConBlackjack();
    testSeguroYesSinBlackjack();
    testSeguroNoSinBlackjack();
    testSeguroCasosSinOferta();
    testDealerPideCon16();
    testDealerSePlantaConBlando17();
    testDealerSePasa();
    testJugadorSePasa();
    testEmpate();
    testAsQueCambiaDeValor();
    testDoblar();
    testDoblarSoloConDosCartas();
    testDoblarNecesitaDinero();
    testEstadisticasYRachas();
    testResetForNewRound();
    testPropiedadesEnRondasAlAzar();

    out << std::endl << (checks - fallos) << " de " << checks << " comprobaciones correctas";
    if (fallos > 0) {
        out << ", " << fallos << " FALLOS";
    }
    out << std::endl;

    std::cout.rdbuf(original);  // devolver std::cout a su sitio antes de salir
    return fallos > 0 ? 1 : 0;
}
