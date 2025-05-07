#include "block.h"
#include <windows.h> 
#include <GL/gl.h>

Block::Block(float x, float y, float z) : x(x), y(y), z(z) {}

void Block::draw() const {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.3f, 0.3f, 0.3f);

    float s = 0.5f; // medio tamaño del cubo

    glBegin(GL_QUADS);
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
