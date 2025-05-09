#include "game.h"
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "FreeImage.h"

const int TAM_NIVEL = 15;
const float TAM_CUBO = 1.0f;

void Game::cargarNivel(const std::string& nombreNivel) {
    std::ifstream nivelFile("../niveles/" + nombreNivel);
    if (!nivelFile.is_open()) {
        std::cerr << "No se pudo abrir nivel: " << nombreNivel << std::endl;
        return;
    }
    std::string linea;
    int i = 0;
    blocks.clear();
    apples.clear();
    while (std::getline(nivelFile, linea) && i < TAM_NIVEL) {
        for (int j = 0; j < TAM_NIVEL && j < (int)linea.size(); ++j) {
            char c = linea[j];
            switch (c) {
            case 'T': blocks.emplace_back(j, -i, 0, TIERRA); break;
            case 'P': blocks.emplace_back(j, -i, 0, PINCHO); break;
            case 'M': apples.emplace_back(j, -i, 0); break;
            case 'C': worm = Worm(j, -i, 0); break;
            default: break;
            }
        }
        ++i;
    }
}

void Game::dibujarNivel() {
    for (auto& apple : apples)
        apple.draw();
    for (auto& block : blocks)
        block.draw();
}

Game::Game()
    : worm(0, 0, 0), state(MENU), portalX(4.0f), portalY(0.0f) {
    cargarNivel("nivel1.txt");
}

void Game::update() {
    if (state != PLAYING) return;

    auto it = std::remove_if(apples.begin(), apples.end(), [this](const Apple& apple) {
        float dx = apple.getX() - worm.getHeadX();
        float dz = apple.getY() - worm.getHeadY();
        if (dx * dx + dz * dz < 0.25f) {
            worm.grow();
            return true;
        }
        return false;
    });
    apples.erase(it, apples.end());

    // Check portal
    float dx = portalX - worm.getHeadX();
    float dz = portalY - worm.getHeadY();
    if (dx * dx + dz * dz < 0.25f) {
        state = WON;
    }

    applyGravity();
}

void Game::applyGravity() {
    for (std::pair<float, float> segment : worm.getSegments()) {
        for (Block const& block : blocks) {
            if (block.getY() == segment.second - 1 && block.getX() == segment.first) {
                return;
            } 
        }
        for (Apple const& apple : apples) {
            if (apple.getY() == segment.second - 1 && apple.getX() == segment.first) {
                return;
            }
        }
    }
    worm.drop();
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Habilitar iluminación y normalización de normales
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    // Luz 0: direccional
    glEnable(GL_LIGHT0);
    GLfloat light0Pos[] = { 0.0f, 5.0f, 0.0f, 0.0f };
    GLfloat light0Ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat light0Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light0Spec[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);

    // Luz 1: puntual suave
    glEnable(GL_LIGHT1);
    GLfloat light1Pos[] = { TAM_NIVEL * 0.5f, 3.0f, TAM_NIVEL * 0.5f, 1.0f };
    GLfloat light1Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light1Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1Ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01f);

    // Luz 2: spotlight frontal intenso
    glEnable(GL_LIGHT2);
    GLfloat light2Pos[] = { 0.0f, 5.0f, 5.0f, 1.0f };
    GLfloat light2Dir[] = { 0.0f, -1.0f, -1.0f };
    GLfloat light2Ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat light2Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light2Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT2, GL_POSITION, light2Pos);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2Dir);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25.0f);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 15.0f);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light2Ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2Diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light2Specular);

    // Material brillante
    GLfloat matSpec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShine[] = { 64.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    // Renderizado de nivel y gusano
    dibujarNivel();
    worm.draw();

    // Desactivar luces
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT0);
    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHTING);
}


void Game::handleInput(SDL_Keycode key) {

    if (state != PLAYING) return;
    switch (key) {
        case SDLK_UP:
            moveWorm(UP);
            break;
        case SDLK_DOWN:
            moveWorm(DOWN);
            break;
        case SDLK_LEFT:
            moveWorm(LEFT);
            break;
        case SDLK_RIGHT:
            moveWorm(RIGHT);
            break;
    }
}

bool Game::isValidMovement(Direction dir) {
    int x = worm.getHeadX();
    int y = worm.getHeadY();

    for (Block const& block : blocks) {
        switch (dir) {
            case UP:
                if (block.getX() == x && block.getY() == y + 1) return false;
                break;
            case DOWN:
                if (block.getX() == x && block.getY() == y - 1) return false;
                break;
            case LEFT:
                if (block.getX() == x - 1 && block.getY() == y ) return false;
                break;
            case RIGHT:
                if (block.getX() == x + 1 && block.getY() == y) return false;
                break;
        }
    }
    for (std::pair<float,float> const& segment : worm.getSegments()) {
        switch (dir) {
        case UP:
            if (segment.first == x && segment.second == y + 1) return false;
            break;
        case DOWN:
            if (segment.first == x && segment.second == y - 1) return false;
            break;
        case LEFT:
            if (segment.first == x - 1 && segment.second == y) return false;
            break;
        case RIGHT:
            if (segment.first == x + 1 && segment.second == y) return false;
            break;
        }
    }
}

void Game::moveWorm(Direction dir) {
    if (isValidMovement(dir)) {
        switch (dir) {
        case UP:
            worm.move(0, 1);
            break;
        case DOWN:
            worm.move(0, -1);
            break;
        case LEFT:
            worm.move(-1, 0);
            break;
        case RIGHT:
            worm.move(1, 0);
            break;
        }
    }
}

void Game::renderMenu() const {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Game::renderWinScreen() const {
    glClearColor(0.0f, 0.4f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}