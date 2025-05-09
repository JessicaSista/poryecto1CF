#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <SDL.h>
#include "worm.h"
#include "apple.h"
#include "block.h"
#include "camera.h"

enum GameState {
    MENU,
    PLAYING,
    WON
};

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Game {
public:
    Game();
    void cargarNivel(const std::string& nombreNivel);
    void dibujarNivel();
    void moveWorm(Direction dir);
    bool isValidMovement(Direction dir);
    void update();
    void applyGravity();
    void render();
    void renderMenu() const;
    void renderWinScreen() const;
    void handleInput(SDL_Keycode key);
    GameState getState() const { return state; }
    void setState(GameState newState) { state = newState; }

private:
    Worm worm;
    std::vector<Apple> apples;
    std::vector<Block> blocks;
    Camera camera;
    GameState state;
    float portalX, portalY;
};

#endif // GAME_H
