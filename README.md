# Blackjack

**[English](#english) · [Español](#español)**

---

## English

A Blackjack game written in C++20 with a graphical interface built on **SFML 3**. The project also includes the original console version.

### Features

- Graphical interface with five screens: menu, betting, game, round result and game over.
- Betting with **+$10**, **+$50** and **+$100** chips, plus a **CLEAR** button. You start with **$1000**.
- Player actions: **HIT**, **STAND** and **DOUBLE**.
- **Natural blackjack pays 3:2**.
- **Dealer peek**: if the dealer is dealt a blackjack, the round ends straight away and you only lose your original bet.
- Result message and color based on the actual outcome of the round (green for a win, red for a loss, yellow for a push).
- Automatic reshuffle when 10 or fewer cards are left in the deck.
- **Game over** screen when you can no longer cover the minimum bet, showing your wins, losses and best win streak, with a **PLAY AGAIN** button to start a new game with $1000.
- Console version (`Blackjack_Console`) with the same game rules and a statistics menu.

### Rules in this version

| Rule | Value |
|---|---|
| Decks | 1 deck of 52 cards |
| Starting money | $1000 |
| Minimum bet | $10 |
| Blackjack payout | 3:2 |
| Dealer | Hits below 17 and stands on any 17 |
| Double down | Only with your first two cards, and only if you can cover the bet |
| Dealer blackjack | Checked right after the deal (peek) |
| Not available yet | Split, insurance |

### Controls

- **Mouse (left click):** every button in the game.
- **ESC:** quit.

### Requirements

- Tested on **Windows 11** with MinGW-w64 (see [Known limitations](#known-limitations)).
- A C++20 compiler (MinGW-w64 or MSVC).
- CMake 3.23 or newer.
- SFML 3 (the Graphics, Window and System modules). The easiest way to get it is [vcpkg](https://github.com/microsoft/vcpkg).

### Building

1. Install SFML 3 with vcpkg. Pick the triplet that matches your compiler (`x64-mingw-dynamic` for MinGW, `x64-windows` for MSVC):

   ```bash
   vcpkg install sfml:x64-mingw-dynamic
   ```

2. Configure the project, pointing CMake at the vcpkg toolchain file:

   ```bash
   cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
   ```

   Always set `CMAKE_BUILD_TYPE` (`Release` or `Debug`). Without it, vcpkg may copy the wrong SFML DLLs and the game will not start.

3. Build the version you want:

   ```bash
   cmake --build build --target Blackjack_GUI
   cmake --build build --target Blackjack_Console
   ```

4. Run `build/Blackjack_GUI.exe` (or `build/Blackjack_Console.exe`). After building, the vcpkg toolchain copies the SFML DLLs next to the executable, and CMake copies the `assets/` folder there too.

**With CLion:** open the folder as a CMake project and add `-DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake` to the CMake options (*Settings → Build, Execution, Deployment → CMake*). Then choose the `Blackjack_GUI` run configuration.

**Moving the game to another folder:** copy the executable together with its DLLs and the `assets/` folder. The game looks for `assets/fonts/DejaVuSans.ttf` relative to the folder it is run from. On a computer without MinGW installed, it also needs MinGW's runtime DLLs (`libstdc++-6.dll`, `libgcc_s_seh-1.dll`).

### Known limitations

- **Only tested on Windows.** The code no longer depends on Windows: the font ships with the project in `assets/`, and `<windows.h>` is only used under `#ifdef _WIN32` to enable UTF-8 console output. It should build on Linux and macOS with SFML 3, but this has not been tested yet.
- Split and insurance are not implemented yet.

### Project structure

```
Blackjack/
├── CMakeLists.txt     # Targets: Blackjack_GUI and Blackjack_Console
├── main.cpp           # Console version
├── main_gui.cpp       # Graphical version
├── assets/
│   └── fonts/         # DejaVu Sans and its license (copied next to the executable when building)
├── include/           # Headers
│   ├── BlackjackGUI.h # SFML interface (screens, buttons, state machine)
│   ├── Game.h         # Game logic: bets, rounds, payouts, statistics
│   ├── Player.h       # Player hand and its value (aces count as 1 or 11)
│   ├── Dealer.h       # Dealer (inherits from Player)
│   ├── Deck.h         # 52-card deck
│   ├── Card.h         # Card: suit and rank
│   └── Comun.h        # Shared includes
└── src/               # Implementations
```

The game logic (`Game`) does no input or output on its own; the console version and the graphical version are two different front ends on top of it.

### Credits

- Font: [DejaVu Sans](https://github.com/dejavu-fonts/dejavu-fonts), distributed under its own license (Bitstream Vera and Arev). The full text is in [`assets/fonts/LICENSE`](assets/fonts/LICENSE).

### Versions

| Version | Contents |
|---|---|
| *In development* | Phase 1: portable font (DejaVu Sans in `assets/`) and Windows-only code isolated behind `#ifdef _WIN32` |
| **V2.0** | Complete graphical version: game over, dealer peek, 3:2 payout, result colors and a fixed result screen layout |
| V1.1 | First graphical game screen (work in progress) |
| V1.0 | Console version |

### Roadmap

Phase 1, in progress: ~~portable font~~ (done), statistics screen, split, an ALL IN or custom bet, insurance and automated tests.

---

## Español

Un juego de Blackjack escrito en C++20 con interfaz gráfica hecha con **SFML 3**. El proyecto incluye también la versión original de consola.

### Características

- Interfaz gráfica con cinco pantallas: menú, apuesta, partida, resultado de la ronda y fin de partida.
- Apuestas con fichas de **+$10**, **+$50** y **+$100**, y un botón **CLEAR** para borrarla. Empiezas con **1000 $**.
- Acciones del jugador: **HIT** (pedir carta), **STAND** (plantarse) y **DOUBLE** (doblar).
- **El blackjack natural paga 3:2**.
- **Dealer peek**: si al dealer le sale blackjack en el reparto, la ronda termina en ese momento y solo pierdes tu apuesta inicial.
- Mensaje y color del resultado según lo que ha pasado de verdad en la ronda (verde si ganas, rojo si pierdes, amarillo si hay empate).
- El mazo se vuelve a barajar solo cuando quedan 10 cartas o menos.
- Pantalla de **Game Over** cuando ya no te llega para la apuesta mínima, con tus victorias, derrotas y mejor racha, y un botón **PLAY AGAIN** para empezar otra partida con 1000 $.
- Versión de consola (`Blackjack_Console`) con las mismas reglas y un menú de estadísticas.

### Reglas de esta versión

| Regla | Valor |
|---|---|
| Mazos | 1 mazo de 52 cartas |
| Dinero inicial | 1000 $ |
| Apuesta mínima | 10 $ |
| Pago del blackjack | 3:2 |
| Dealer | Pide carta por debajo de 17 y se planta con cualquier 17 |
| Doblar | Solo con las dos primeras cartas y si te llega el dinero |
| Blackjack del dealer | Se comprueba justo después del reparto (peek) |
| Todavía no disponible | Split, seguro |

### Controles

- **Ratón (clic izquierdo):** todos los botones del juego.
- **ESC:** salir.

### Requisitos

- Probado en **Windows 11** con MinGW-w64 (mira [Limitaciones conocidas](#limitaciones-conocidas)).
- Un compilador de C++20 (MinGW-w64 o MSVC).
- CMake 3.23 o superior.
- SFML 3 (los módulos Graphics, Window y System). La forma más fácil de instalarlo es con [vcpkg](https://github.com/microsoft/vcpkg).

### Compilar

1. Instala SFML 3 con vcpkg. Elige el triplet que corresponda a tu compilador (`x64-mingw-dynamic` para MinGW, `x64-windows` para MSVC):

   ```bash
   vcpkg install sfml:x64-mingw-dynamic
   ```

2. Configura el proyecto indicándole a CMake el toolchain de vcpkg:

   ```bash
   cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=<raiz-de-vcpkg>/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
   ```

   Pon siempre `CMAKE_BUILD_TYPE` (`Release` o `Debug`). Sin él, vcpkg puede copiar las DLL de SFML equivocadas y el juego no arrancará.

3. Compila la versión que quieras:

   ```bash
   cmake --build build --target Blackjack_GUI
   cmake --build build --target Blackjack_Console
   ```

4. Ejecuta `build/Blackjack_GUI.exe` (o `build/Blackjack_Console.exe`). Al compilar, el toolchain de vcpkg copia las DLL de SFML junto al ejecutable, y CMake copia también la carpeta `assets/`.

**Con CLion:** abre la carpeta como proyecto CMake y añade `-DCMAKE_TOOLCHAIN_FILE=<raiz-de-vcpkg>/scripts/buildsystems/vcpkg.cmake` a las opciones de CMake (*Settings → Build, Execution, Deployment → CMake*). Después elige la configuración de ejecución `Blackjack_GUI`.

**Para llevar el juego a otra carpeta:** copia el ejecutable junto con sus DLL y la carpeta `assets/`. El juego busca `assets/fonts/DejaVuSans.ttf` a partir de la carpeta desde la que se ejecuta. En un ordenador sin MinGW instalado necesita además las DLL de MinGW (`libstdc++-6.dll`, `libgcc_s_seh-1.dll`).

### Limitaciones conocidas

- **Solo está probado en Windows.** El código ya no depende de Windows: la fuente viene con el proyecto en `assets/`, y `<windows.h>` solo se usa dentro de `#ifdef _WIN32` para activar la salida UTF-8 en la consola. Debería compilar en Linux y macOS con SFML 3, pero todavía no se ha probado.
- El split y el seguro aún no están implementados.

### Estructura del proyecto

```
Blackjack/
├── CMakeLists.txt     # Targets: Blackjack_GUI y Blackjack_Console
├── main.cpp           # Versión de consola
├── main_gui.cpp       # Versión gráfica
├── assets/
│   └── fonts/         # DejaVu Sans y su licencia (se copian junto al ejecutable al compilar)
├── include/           # Cabeceras
│   ├── BlackjackGUI.h # Interfaz SFML (pantallas, botones, máquina de estados)
│   ├── Game.h         # Lógica del juego: apuestas, rondas, pagos, estadísticas
│   ├── Player.h       # Mano del jugador y su valor (el As vale 1 u 11)
│   ├── Dealer.h       # Dealer (hereda de Player)
│   ├── Deck.h         # Mazo de 52 cartas
│   ├── Card.h         # Carta: palo y valor
│   └── Comun.h        # Includes comunes
└── src/               # Implementaciones
```

La lógica del juego (`Game`) no lee ni escribe nada por su cuenta: la versión de consola y la gráfica son dos interfaces distintas sobre la misma lógica.

### Créditos

- Fuente: [DejaVu Sans](https://github.com/dejavu-fonts/dejavu-fonts), distribuida con su propia licencia (Bitstream Vera y Arev). El texto completo está en [`assets/fonts/LICENSE`](assets/fonts/LICENSE).

### Versiones

| Versión | Contenido |
|---|---|
| *En desarrollo* | Fase 1: fuente portátil (DejaVu Sans en `assets/`) y el código exclusivo de Windows aislado con `#ifdef _WIN32` |
| **V2.0** | Versión gráfica completa: Game Over, dealer peek, pago 3:2, colores del resultado y pantalla de resultado corregida |
| V1.1 | Primera pantalla de juego gráfica (en desarrollo) |
| V1.0 | Versión de consola |

### Próximos pasos

Fase 1, en curso: ~~fuente portátil~~ (hecha), pantalla de estadísticas, split, apuesta ALL IN o personalizada, seguro y tests automáticos.
