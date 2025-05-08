#ifndef GAME_H
#define GAME_H
#include <windows.h> 
#include <SDL.h>
#include <vector>
#include "worm.h"
#include "apple.h"
#include "block.h"
#include "camera.h"
#include <assimp/Importer.hpp>      
#include <assimp/scene.h> 
#include <assimp/postprocess.h>

enum GameState {
    MENU,
    PLAYING,
    WON
};

class Game {
public:
    Game();
    void cargarTexturas();
    void update();
    void render() const;
    void handleInput(const Uint8* keystate);

private:
    Worm worm;
    std::vector<Apple> apples;
    std::vector<Block> blocks;
    Camera camera;
    GameState state;
    float portalX, portalY;

    void renderMenu() const;
    void renderWinScreen() const;
};

#endif
