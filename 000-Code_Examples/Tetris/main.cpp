#include <SFML/Graphics.hpp>
#include <ctime>

constexpr int M = 20;
constexpr int N = 10;

int field[M][N] = {0};

struct Point {
    int x = 0, y = 0;
} a[4], b[4];

int figures[7][4] = {
        {1, 3, 5, 7},   // I
        {2, 4, 5, 7},   // Z
        {3, 5, 4, 6},   // S
        {3, 5, 4, 7},   // T
        {2, 3, 5, 7},   // L
        {3, 5, 7, 6},   // J
        {2, 3, 4, 5},   // O
};

bool check() {
    for (int i = 0; i < 4; ++i)
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return false;
        else if (field[a[i].y][a[i].x]) return false;

    return true;
}

int main() {
    std::srand(std::time(0));

    sf::RenderWindow window(sf::VideoMode(320, 480), "My Tetris");

    sf::Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");

    sf::Sprite s(t1), background(t2), frame(t3);

    int dx = 0;
    bool rotate = false;
    int colorNum = 1;

    float timer = 0, delay = 0.3;

    sf::Clock clock;


    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        sf::Event e;

        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::KeyPressed)
                if (e.key.code == sf::Keyboard::Up) rotate = true;
                else if (e.key.code == sf::Keyboard::Left) dx = -1;
                else if (e.key.code == sf::Keyboard::Right) dx = 1;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delay = 0.05;

        // <- Move ->
        for (int i = 0; i < 4; ++i) {
            b[i] = a[i];
            a[i].x += dx;
        }
        if (!check()) for (int i = 0; i < 4; ++i) a[i] = b[i];

        // Rotate
        if (rotate) {
            Point p = a[1];         // center of rotation
            for (int i = 0; i < 4; ++i) {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check()) for (int i = 0; i < 4; ++i) a[i] = b[i];
        }

        // Tick
        if (timer > delay) {
            for (int i = 0; i < 4; ++i) {
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!check()) {
                for (int i = 0; i < 4; ++i)
                    field[b[i].y][b[i].x] = colorNum;

                colorNum = 1 + std::rand() % 7;
                int n = std::rand() % 7;

                for (int i = 0; i < 4; ++i) {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            }

            timer = 0;
        }

        // check lines
        int k = M - 1;
        for (int i = M - 1; i > 0; i--) {
            int count = 0;
            for (int j = 0; j < N; j++) {
                if (field[i][j]) count++;
                field[k][j] = field[i][j];
            }
            if (count < N) k--;
        }

        dx = 0;
        rotate = 0;
        delay = 0.3;

        // draw
        window.clear(sf::Color::White);
        window.draw(background);

        for (int i = 0; i < M; ++i)
            for (int j = 0; j < N; ++j) {
                if (field[i][j] == 0) continue;
                s.setTextureRect(sf::IntRect(field[i][j] * 18, 0, 18, 18));
                s.setPosition(j * 18, i * 18);
                s.move(28, 31); // offset
                window.draw(s);
            }

        for (int i = 0; i < 4; i++) {
            s.setTextureRect(sf::IntRect(colorNum * 18, 0, 18, 18));
            s.setPosition(a[i].x * 18, a[i].y * 18);
            s.move(28, 31); // offset
            window.draw(s);
        }
        window.draw(frame);
        window.display();
    }
}