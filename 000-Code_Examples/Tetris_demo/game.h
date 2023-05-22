#ifndef HELLO_SFML_GAME_H
#define HELLO_SFML_GAME_H

#include <SFML/Graphics.hpp>
#include <cstdlib>

class Game {
public:
    Game(sf::RenderWindow& window);

    void start();

    void move(int& move);

    void down();

    void rotate(int& rotate);

    void fall();

    void refresh();


private:
    sf::RenderWindow& window;
    sf::Clock clock;

    sf::Font font;
    sf::Text score;
    sf::Text game_over;

    sf::Texture tiles_texture;
    sf::Texture bg_texture;
    sf::Texture frame_texture;

    sf::Sprite tiles;
    sf::Sprite bg;
    sf::Sprite frame;

    struct Tile {
        int x = 0, y = 0;
    };

    Tile tetrominoes[7][4] {
            {       // O
                    { 0,  0 },
                    { 1,  0 },
                    { 0,  1 },
                    { 1, 1 },
            },
            {       // I
                    { -1, 0 },
                    { 0,  0 },
                    { 1,  0 },
                    { 2, 0 },
            },
            {       // T
                    { 0,  0 },
                    { -1, 0 },
                    { 1,  0 },
                    { 0, -1 },
            },
            {       // S
                    { 0,  0 },
                    { -1, 0 },
                    { 0,  -1 },
                    { 1, -1 }
            },
            {       // Z
                    { 0,  0 },
                    { 0,  -1 },
                    { -1, -1 },
                    { 1, 0 }
            },
            {       // J
                    { 0,  0 },
                    { -1, 0 },
                    { -1, -1 },
                    { 1, 0 }
            },
            {       // L
                    { 0,  0 },
                    { -1, 0 },
                    { 1,  0 },
                    { 1, -1 }
            }
    };

    Tile i_rotate[4][4] {
            {
                    { -1, -2 },
                    { 0,  -1 },
                    { 1,  0 },
                    { 2,  1 },
            },
            {
                    { 2,  -1 },
                    { 1,  0 },
                    { 0,  1 },
                    { -1, 2 },
            },
            {
                    { 1,  2 },
                    { 0,  1 },
                    { -1, 0 },
                    { -2, -1 },
            },
            {
                    { -2, 1 },
                    { -1, 0 },
                    { 0,  -1 },
                    { 1,  -2 },
            },
    };


    void bound();

    void land();

    void count();

    void is_overflow();

    void next();

    static constexpr float SPEED = 0.2f;
    static constexpr int DROP_LOCATION = 4;
    static constexpr int offset_x = 28;
    static constexpr int offset_y = 13;
    static constexpr int VMAX = 21;
    static constexpr int HMAX = 10;

    struct {
        int t_type;
        int rotate = 1;
        int colorNum = 1 + std::rand() % 7;
        float timer = 0.f;
        float delay = SPEED;
        float time = 0.f;
        Tile tetromino[4];
        Tile prev_tetromino[4];
        int field[VMAX][HMAX] { 0 };
        bool overflow = false;
        int score = 0;
    } state;
};


#endif //HELLO_SFML_GAME_H
