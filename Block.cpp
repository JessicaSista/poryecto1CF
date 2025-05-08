#include "block.h"
#include <windows.h> 
#include <GL/gl.h>

Block::Block(float x, float y, float z, Tile tipo) : x(x), y(y), z(z), tipo(tipo) {}

void Block::draw() const {
    float r, g, b;
    switch (tipo) {
        case TIERRA:
            r = 0.2;
            g = 0.1;
            b = 0;
            break;
        case PINCHO:
            r = 0.25;
            g = 0.25;
            b = 0.25;
            break;
    }
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);
    glColor3f(r, g, b);

    float s = 0.5f; // medio tamaño del cubo
    // Frente
    glVertex3f(-s, -s, s);
    glVertex3f(s, -s, s);
    glVertex3f(s, s, s);
    glVertex3f(-s, s, s);
    // Atrás
    glVertex3f(-s, -s, -s);
    glVertex3f(-s, s, -s);
    glVertex3f(s, s, -s);
    glVertex3f(s, -s, -s);
    // Izquierda
    glVertex3f(-s, -s, -s);
    glVertex3f(-s, -s, s);
    glVertex3f(-s, s, s);
    glVertex3f(-s, s, -s);
    // Derecha
    glVertex3f(s, -s, -s);
    glVertex3f(s, s, -s);
    glVertex3f(s, s, s);
    glVertex3f(s, -s, s);
    // Arriba
    glVertex3f(-s, s, -s);
    glVertex3f(-s, s, s);
    glVertex3f(s, s, s);
    glVertex3f(s, s, -s);
    // Abajo
    glVertex3f(-s, -s, -s);
    glVertex3f(s, -s, -s);
    glVertex3f(s, -s, s);
    glVertex3f(-s, -s, s);
    glEnd();

    glPopMatrix();
}


float Block::getX() const { return x; }
float Block::getY() const { return y; }
float Block::getZ() const { return z; }
Tile Block::getTipo() const { return tipo; }