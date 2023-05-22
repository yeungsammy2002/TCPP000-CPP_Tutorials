#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "game.h"

Game::Game(sf::RenderWindow& window) : window { window } {
    std::srand(std::time(0));

    font.loadFromFile("fonts/arcadeclassic.ttf");

    game_over.setFont(font);
    game_over.setPosition(36, 180);
    game_over.setString("Game Over!");
    game_over.setCharacterSize(30);
    game_over.setFillColor(sf::Color::Red);
    game_over.setStyle(sf::Text::Bold);

    score.setFont(font);
    score.setPosition(140, 430);
    score.setString("Score ");
    score.setCharacterSize(30);
    score.setFillColor(sf::Color::Black);
    score.setStyle(sf::Text::Bold);

    tiles_texture.loadFromFile("images/tiles.png");
    bg_texture.loadFromFile("images/background.png");
    frame_texture.loadFromFile("images/frame.png");

    tiles.setTexture(tiles_texture);
    bg.setTexture(bg_texture);
    frame.setTexture(frame_texture);

    state.t_type = std::rand() % 7;
    state.colorNum = 1 + std::rand() % 7;

    for (int i = 0; i < 4; ++i) {
        state.tetromino[i].x = tetrominoes[state.t_type][i].x + DROP_LOCATION;
        state.tetromino[i].y = tetrominoes[state.t_type][i].y;
    }
}

void Game::start() {
    if (state.overflow) return;
    state.time = clock.getElapsedTime().asSeconds();
    clock.restart();
    state.timer += state.time;
}

void Game::move(int& move) {
    for (int i = 0; i < 4; ++i) {
        state.prev_tetromino[i] = state.tetromino[i];
        state.tetromino[i].x += move;
    }

    move = 0;
    bound();
}

void Game::down() {
    state.delay = 0.05;
}

void Game::rotate(int& rotate) {
    if (!rotate || state.t_type == 0) return;
    if (state.rotate == 4 && rotate == 1)
        state.rotate = 1;
    else if (state.rotate == 1 && rotate == -1)
        state.rotate = 4;
    else
        state.rotate += rotate;

    switch (state.t_type) {
        case 0:
            return;
        case 1:
            for (int i = 0; i < 4; ++i) {
                if (rotate == 1) {
                    state.tetromino[i].x += i_rotate[state.rotate - 1][i].x;
                    state.tetromino[i].y += i_rotate[state.rotate - 1][i].y;
                } else if (rotate == -1) {
                    int temp = state.rotate;
                    if (temp == 4) temp = 0;
                    state.tetromino[i].x -= i_rotate[temp][i].x;
                    state.tetromino[i].y -= i_rotate[temp][i].y;
                }
            }
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            for (int i = 0; i < 4; ++i) {
                int x = state.tetromino[i].x - state.tetromino[0].x;
                int y = state.tetromino[i].y - state.tetromino[0].y;
                if (rotate == 1) {
                    state.tetromino[i].x = state.tetromino[0].x - y;
                    state.tetromino[i].y = state.tetromino[0].y + x;
                } else if (rotate == -1) {
                    state.tetromino[i].x = state.tetromino[0].x + y;
                    state.tetromino[i].y = state.tetromino[0].y - x;
                }
            }
            break;
    }
    bound();

    rotate = 0;
}

void Game::bound() {
    int x = 0;
    bool is_overlap = false;
    for (int i = 0; i < 4; ++i) {
        if (state.tetromino[i].x < 0 && state.tetromino[i].x < x) {
            x = state.tetromino[i].x;
        }
        if (state.tetromino[i].x > (HMAX - 1) && state.tetromino[i].x > x) {
            x = state.tetromino[i].x - HMAX + 1;
        }
        if (state.field[state.tetromino[i].y][state.tetromino[i].x])
            is_overlap = true;
    }

    if (x != 0) {
        for (int i = 0; i < 4; ++i) {
            state.tetromino[i].x -= x;
        }
    }

    if (is_overlap) {
        for (int i = 0; i < 4; ++i)
            state.tetromino[i] = state.prev_tetromino[i];
    }
}

void Game::fall() {
    if (state.timer > state.delay) {
        for (int i = 0; i < 4; ++i) {
            state.prev_tetromino[i] = state.tetromino[i];
            state.tetromino[i].y += 1;
        }
        state.timer = 0;
        land();
    }
}

void Game::land() {
    bool is_land = false;

    for (int i = 0; i < 4; ++i) {
        if (state.tetromino[i].y >= VMAX ||
            state.field[state.tetromino[i].y][state.tetromino[i].x]) {
            is_land = true;
            break;
        }
    }

    if (is_land) {
        for (int i = 0; i < 4; ++i) {
            state.field[state.prev_tetromino[i].y][state.prev_tetromino[i].x] = state.colorNum;
        }
        count();
        is_overflow();
        next();
    }

}

void Game::count() {
    int k = VMAX - 1;
    for (int i = VMAX - 1; i > 0; --i) {
        int tiles = 0;
        for (int j = 0; j < HMAX; ++j) {
            if (state.field[i][j]) tiles++;
            state.field[k][j] = state.field[i][j];
        }
        if (tiles != 10) k--;
        else state.score += 1;
    }
}

void Game::is_overflow() {
    for (int i = 0; i < HMAX; ++i)
        if (state.field[0][i]) {
            state.overflow = true;
            std::cout << "overflow, game over" << std::endl;
            break;
        }
}

void Game::next() {
    state.t_type = std::rand() % 7;
    state.rotate = 1;
    state.colorNum = 1 + std::rand() % 7;
    for (int i = 0; i < 4; ++i) {
        state.tetromino[i].x = tetrominoes[state.t_type][i].x + DROP_LOCATION;
        state.tetromino[i].y = tetrominoes[state.t_type][i].y;
    }
}

void Game::refresh() {
    state.delay = SPEED;

    window.clear(sf::Color::Black);

    // draw demo background
//    sf::Texture demo_texture;
//    demo_texture.loadFromFile("images/demo.png");
//    sf::Sprite demo;
//    demo.setTexture(demo_texture);
//    demo.setTextureRect(sf::IntRect(0, 0, 180, 360));
//    demo.setPosition(0, 18);
//    window.draw(demo);

    window.draw(bg);

    // draw current tetromino
    for (int i = 0; i < 4; ++i) {
        tiles.setTextureRect(sf::IntRect(state.colorNum * 18, 0, 18, 18));
        tiles.setPosition(offset_x + state.tetromino[i].x * 18, offset_y + state.tetromino[i].y * 18);
        window.draw(tiles);
    }

    // draw current field state
    for (int i = 0; i < VMAX; ++i)
        for (int j = 0; j < HMAX; ++j) {
            if (state.field[i][j] == 0) continue;
            tiles.setTextureRect(sf::IntRect(state.field[i][j] * 18, 0, 18, 18));
            tiles.setPosition(offset_x + j * 18, offset_y + i * 18);
            window.draw(tiles);
        }

    // print score
    std::string score_str = "Score " + std::to_string(state.score);
    score.setString(score_str);
    window.draw(score);

    // print game over
    if (state.overflow) {
        window.draw(game_over);
    }

    window.display();
}

