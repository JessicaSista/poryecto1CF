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

const int TAM_NIVEL = 15;
const float TAM_CUBO = 1;

const int cantText = 1;

void Game::cargarNivel(string nombreNivel) {
    ifstream nivelFile("../niveles/" + nombreNivel);
    string linea;
    int i = 0;
    while (getline(nivelFile, linea)) {
        for (int j = 0; j < TAM_NIVEL; j++) {
            switch (linea.at(j)) {
            case 'A':
                break;
            case 'T':
                blocks.emplace_back(j,-i,0,TIERRA);
                break;
            case 'P':
                blocks.emplace_back(j, -i, 0, PINCHO);
                break;
            case 'M':
                apples.emplace_back(j, -i, 0);
                break;
            case 'G':
                //nivel[i][j] = GUSANO;
                break;
            case 'C':
                worm = Worm(-j, i, 0);
                break;
            case 'F':
                //nivel[i][j] = FIN;
                break;
            }
        }
        i++;
    }
    nivelFile.close();
}

void Game::dibujarNivel() {
    for (Apple apple : apples) {
        apple.draw(); 
    }
    for (Block const& block : blocks) block.draw();
}



Game::Game()
    :worm(0,0,0), state(MENU), portalX(4.0f), portalY(0.0f) {
    cargarNivel("nivel1.txt");
}

void Game::update() {
    if (state != PLAYING) return;

    auto it = std::remove_if(apples.begin(), apples.end(), [&](const Apple& apple) {
        float dx = apple.getX() - worm.getHeadX();
        float dz = apple.getZ() - worm.getHeadY();  // o HeadZ si corresponde
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

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat light_pos[] = { 1.0f, 2.0f, 1.0f, 0.0f };
    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specular[] = { 0.4f, 0.4f, 0.4f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);


    this->dibujarNivel();
    worm.draw();

    // Dibujar el portal (cubo azul)
    glPushMatrix();
    glTranslatef(portalX, 0.5f, portalY);
    glColor3f(0.2f, 0.2f, 1.0f);
    float s = 0.5f;
    glBegin(GL_QUADS);
    // (Caras del cubo igual que antes...)
    glVertex3f(-s, -s, s); glVertex3f(s, -s, s);
    glVertex3f(s, s, s); glVertex3f(-s, s, s);

    glVertex3f(-s, -s, -s); glVertex3f(-s, s, -s);
    glVertex3f(s, s, -s); glVertex3f(s, -s, -s);

    glVertex3f(-s, -s, -s); glVertex3f(-s, -s, s);
    glVertex3f(-s, s, s); glVertex3f(-s, s, -s);

    glVertex3f(s, -s, -s); glVertex3f(s, s, -s);
    glVertex3f(s, s, s); glVertex3f(s, -s, s);

    glVertex3f(-s, s, -s); glVertex3f(-s, s, s);
    glVertex3f(s, s, s); glVertex3f(s, s, -s);

    glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s);
    glVertex3f(s, -s, s); glVertex3f(-s, -s, s);
    glEnd();
    glPopMatrix();

    // Deshabilitar iluminaci�n
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
}


void Game::handleInput(const Uint8* keystate) {

    if (state != PLAYING) return;

    if (keystate[SDL_SCANCODE_RIGHT]) worm.move(0.1f, 0.0f);
    if (keystate[SDL_SCANCODE_LEFT]) worm.move(-0.1f, 0.0f);
    if (keystate[SDL_SCANCODE_UP]) worm.move(0.0f, -0.1f);
    if (keystate[SDL_SCANCODE_DOWN]) worm.move(0.0f, 0.1f);
}

void Game::moveWorm() {

}

void Game::renderMenu() const {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);  // rojo intenso
    glClear(GL_COLOR_BUFFER_BIT);
    // Podr�as dibujar un bot�n o texto aqu� con un quad
    // o usar SDL_ttf si agreg�s fuentes
}

void Game::renderWinScreen() const {
    glClearColor(0.0f, 0.4f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Pod�s dibujar un mensaje tipo "�Ganaste!" como un fondo verde
}