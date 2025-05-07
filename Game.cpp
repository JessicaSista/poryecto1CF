#include "game.h"
#include <windows.h> 
#include <GL/gl.h>
#include <GL/glu.h>
#include <algorithm>

Game::Game()
    : worm(0.0f, 0.5f, 0.0f), state(MENU), portalX(4.0f), portalZ(0.0f) {
    for (int i = -5; i <= 5; ++i) {
        blocks.emplace_back(i, 0.0f, 0);
    }
    apples.emplace_back(1.0f, 0.5f, 0.0f);
    apples.emplace_back(2.0f, 0.5f, 0.0f);
}

void Game::update() {
    if (state != PLAYING) return;

    auto it = std::remove_if(apples.begin(), apples.end(), [&](const Apple& apple) {
        float dx = apple.getX() - worm.getHeadX();
        float dz = apple.getZ() - worm.getHeadZ();
        if (dx * dx + dz * dz < 0.25f) {
            worm.grow();
            return true;
        }
        return false;
        });
    apples.erase(it, apples.end());

    // Check portal
    float dx = portalX - worm.getHeadX();
    float dz = portalZ - worm.getHeadZ();
    if (dx * dx + dz * dz < 0.25f) {
        state = WON;
    }
}

void Game::render() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (state == MENU) {
        renderMenu();
        return;
    }

    if (state == WON) {
        renderWinScreen();
        return;
    }

    camera.apply();

    for (const auto& block : blocks) block.draw();
    for (const auto& apple : apples) apple.draw();
    worm.draw();

    // Dibujar el portal (cubo azul)
    glPushMatrix();
    glTranslatef(portalX, 0.5f, portalZ);
    glColor3f(0.2f, 0.2f, 1.0f);
    float s = 0.5f;
    glBegin(GL_QUADS);
    // caras igual que el cubo de bloques...
    // frente
    glVertex3f(-s, -s, s); glVertex3f(s, -s, s);
    glVertex3f(s, s, s); glVertex3f(-s, s, s);
    // atrás
    glVertex3f(-s, -s, -s); glVertex3f(-s, s, -s);
    glVertex3f(s, s, -s); glVertex3f(s, -s, -s);
    // izq
    glVertex3f(-s, -s, -s); glVertex3f(-s, -s, s);
    glVertex3f(-s, s, s); glVertex3f(-s, s, -s);
    // der
    glVertex3f(s, -s, -s); glVertex3f(s, s, -s);
    glVertex3f(s, s, s); glVertex3f(s, -s, s);
    // arriba
    glVertex3f(-s, s, -s); glVertex3f(-s, s, s);
    glVertex3f(s, s, s); glVertex3f(s, s, -s);
    // abajo
    glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s);
    glVertex3f(s, -s, s); glVertex3f(-s, -s, s);
    glEnd();
    glPopMatrix();
}

void Game::handleInput(const Uint8* keystate) {
    if (state == MENU && keystate[SDL_SCANCODE_RETURN]) {
        state = PLAYING;
        return;
    }

    if (state != PLAYING) return;

    if (keystate[SDL_SCANCODE_RIGHT]) worm.move(0.1f, 0.0f);
    if (keystate[SDL_SCANCODE_LEFT]) worm.move(-0.1f, 0.0f);
    if (keystate[SDL_SCANCODE_UP]) worm.move(0.0f, -0.1f);
    if (keystate[SDL_SCANCODE_DOWN]) worm.move(0.0f, 0.1f);
}

void Game::renderMenu() const {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Podrías dibujar un botón o texto aquí con un quad
    // o usar SDL_ttf si agregás fuentes
}

void Game::renderWinScreen() const {
    glClearColor(0.0f, 0.4f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Podés dibujar un mensaje tipo "¡Ganaste!" como un fondo verde
}
