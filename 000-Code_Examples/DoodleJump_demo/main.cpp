#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>

struct Point { int x, y; };

int main() {
    std::srand(time(0));

    sf::RenderWindow app(sf::VideoMode(400,533), "Doodle Jump Demo");
    app.setFramerateLimit(60);

    sf::Texture t1, t2, t3;
    t1.loadFromFile("images/background.png");
    t2.loadFromFile("images/platform.png");
    t3.loadFromFile("images/doodle.png");

    sf::Sprite sBackground(t1), sPlat(t2), sPers(t3);

    Point plat[20];

    for(int i = 0; i < 10; ++i) {
        plat[i].x = std::rand() % 400;
        plat[i].y = std::rand() % 533;
    }

    int x = 100, y = 100, h = 200;
    float dx = 0, dy = 0;

    while(app.isOpen()) {
        sf::Event e;

        while(app.pollEvent(e)) {
            if(e.type == sf::Event::Closed) {
                app.close();
            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) x += 3;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) x -= 3;

        dy += 0.2;
        y += dy;
        if(y > 500) dy = -10;

        if(y < h)
            for(int i = 0; i < 10; ++i) {
                y = h;
                plat[i].y = plat[i].y - dy;
                if(plat[i].y > 533) {
                    plat[i].y = 0;
                    plat[i].x = std::rand() % 400;
                }
            }

        for(int i = 0; i < 10; ++i)
            if((x + 50 > plat[i].x)
            && (x + 20 < plat[i].x + 68)
            && (y + 70 > plat[i].y)
            && (y + 70 < plat[i].y + 14)
            && (dy > 0))
                dy = -10;

        sPers.setPosition(x, y);

        app.draw(sBackground);
        app.draw(sPers);

        for(int i = 0; i < 10; ++i) {
            sPlat.setPosition(plat[i].x, plat[i].y);
            app.draw(sPlat);
        }

        app.display();
    }
}
