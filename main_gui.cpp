#include <SFML/Graphics.hpp>
#include <iostream>

// Función helper para detectar si el mouse está sobre un rectángulo
bool mouseOver(const sf::RectangleShape& rect, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect bounds = rect.getGlobalBounds();

    return bounds.contains(sf::Vector2f(static_cast<float>(mousePos.x),
                                        static_cast<float>(mousePos.y)));
}

int main() {
    std::cout << "Iniciando SFML 3.0..." << std::endl;

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Test SFML - Blackjack");
    window.setFramerateLimit(60);

    std::cout << "Ventana creada correctamente" << std::endl;
    std::cout << "Presiona ESC para cerrar" << std::endl;

    sf::Color BackgroundColor(0, 100, 0);

    // Cargar fuente
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Error: No se pudo cargar arial.ttf" << std::endl;
        return -1;
    }

    // NUEVO: Crear botón
    sf::RectangleShape boton(sf::Vector2f(150, 60));
    boton.setPosition({325, 450});
    boton.setFillColor(sf::Color(100, 100, 200)); // Azul

    // Texto del botón
    sf::Text textoBoton(font);
    textoBoton.setString("HIT");
    textoBoton.setCharacterSize(30);
    textoBoton.setPosition({365, 460});
    textoBoton.setFillColor(sf::Color::White);

    // Contador de clicks
    int clicks = 0;
    sf::Text contadorTexto(font);
    contadorTexto.setCharacterSize(25);
    contadorTexto.setPosition({350, 520});
    contadorTexto.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPress->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
            }

            // Detectar click en el botón
            if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseClick->button == sf::Mouse::Button::Left) {
                    if (mouseOver(boton, window)) {
                        clicks++;
                        contadorTexto.setString("Clicks: " + std::to_string(clicks));
                        std::cout << "¡Botón clickeado! Total: " << clicks << std::endl;
                    }
                }
            }
        }

        // Cambiar color del botón si el mouse está encima
        if (mouseOver(boton, window)) {
            boton.setFillColor(sf::Color(150, 150, 255)); // Azul claro (hover)
        } else {
            boton.setFillColor(sf::Color(100, 100, 200)); // Azul normal
        }

        window.clear(BackgroundColor);

        // Dibujar carta
        sf::RectangleShape carta(sf::Vector2f(100, 140));
        carta.setPosition({350, 230});
        carta.setFillColor(sf::Color::White);
        carta.setOutlineColor(sf::Color::Black);
        carta.setOutlineThickness(3);
        window.draw(carta);

        // Texto de la carta
        sf::Text textoRango(font);
        textoRango.setString("K");
        textoRango.setCharacterSize(40);
        textoRango.setPosition({365, 240});
        textoRango.setFillColor(sf::Color::Black);
        window.draw(textoRango);

        sf::Text textoPalo(font);
        textoPalo.setString(L"\u2660");  // ♠
        textoPalo.setCharacterSize(40);
        textoPalo.setPosition({365, 340});
        textoPalo.setFillColor(sf::Color::Black);
        window.draw(textoPalo);

        // Dibujar botón y textos
        window.draw(boton);
        window.draw(textoBoton);
        window.draw(contadorTexto);

        window.display();
    }

    std::cout << "¡SFML 3.0 funcionó correctamente!" << std::endl;
    return 0;
}