#include <SFML/Graphics.hpp>
#include <iostream>

#include "game.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(320, 480), "Tetris Refactoring");

    Game game(window);

    int move = 0;
    int rotate = 0;

    while (window.isOpen()) {
        game.start();

        sf::Event e;

        while (window.pollEvent(e)) {
            switch (e.type) {
                case sf::Event::KeyPressed:
                    if (e.key.code == sf::Keyboard::Left) move = -1;
                    if (e.key.code == sf::Keyboard::Right) move = 1;
                    if (e.key.code == sf::Keyboard::R) rotate = 1;
                    if (e.key.code == sf::Keyboard::E) rotate = -1;
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) game.down();

        game.move(move);
        game.rotate(rotate);
        game.fall();

        game.refresh();
    }
}
