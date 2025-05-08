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
    void cargarNivel(string nombreNivel);
    void dibujarNivel();
    void moveWorm();
    void update();
    void render();
    void renderMenu() const;
    void renderWinScreen() const;
    void handleInput(const Uint8* keystate);
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

#endif
