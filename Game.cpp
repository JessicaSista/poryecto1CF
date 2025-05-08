#include "SDL.h"
#include "SDL_opengl.h"
#include "game.h"
#include <windows.h> 
#include <GL/gl.h>
#include <GL/glu.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "FreeImage.h"

using namespace std;

const int cantText = 1;
GLuint textures[cantText];

void Game::cargarTexturas() {
    const char* archivos[] = {
        "../texturas/manzana.png"
    };

    for (int i = 0; i < cantText; i++) {
        const char* rutaArchivo = archivos[i];
        FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(rutaArchivo);
        FIBITMAP* bitmap = FreeImage_Load(fif, rutaArchivo);
        bitmap = FreeImage_ConvertTo24Bits(bitmap);
        int w = FreeImage_GetWidth(bitmap);
        int h = FreeImage_GetHeight(bitmap);
        void* datos = FreeImage_GetBits(bitmap);

        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, datos);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
}

Game::Game()
    : worm(0.0f, 0.5f, 0.0f), state(MENU), portalX(4.0f), portalY(0.0f) {
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
    for (Apple apple : apples) {
        apple.draw(textures[0]);
    }
    worm.draw();

    // Dibujar el portal (cubo azul)
    glPushMatrix();
    glTranslatef(portalX, 0.5f, portalY);
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
