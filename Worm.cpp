#include "Worm.h"
#include <windows.h>   
#include <GL/gl.h>
#include <GL/glu.h>

Worm::Worm(float startX, float startY, float startZ) : y(startY) {
    segments.emplace_back(startX, startZ);
}

void Worm::draw() const {
    glColor3f(0.0f, 1.0f, 0.0f);
    for (const auto& segment : segments) {
        glPushMatrix();
        glTranslatef(segment.first, y, segment.second);
        GLUquadric* quad = gluNewQuadric();
        gluSphere(quad, 0.2, 16, 16);
        gluDeleteQuadric(quad);
        glPopMatrix();
    }
}

void Worm::move(float dx, float dz) {
    float newX = segments.front().first + dx;
    float newZ = segments.front().second + dz;
    segments.insert(segments.begin(), { newX, newZ });
    segments.pop_back();
}

void Worm::grow() {
    segments.push_back(segments.back());
}

float Worm::getHeadX() const { return segments.front().first; }
float Worm::getHeadZ() const { return segments.front().second; }