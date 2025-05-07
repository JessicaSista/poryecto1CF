#include "apple.h"

Apple::Apple(float x, float y, float z) : x(x), y(y), z(z) {}

void Apple::draw() const {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0f, 0.0f, 0.0f); // rojo
    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, 0.2, 16, 16);
    gluDeleteQuadric(quad);
    glPopMatrix();
}

float Apple::getX() const { return x; }
float Apple::getY() const { return y; }
float Apple::getZ() const { return z; }
