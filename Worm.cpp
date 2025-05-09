#include "Worm.h"
#include <windows.h>   
#include <GL/gl.h>
#include <GL/glu.h>

Worm::Worm(float startX, float startY, float startZ) : z(startZ) {
    segments.emplace_back(startX, startY);
}

void Worm::draw() const {
    //Dibujar cabeza de otro color
    glColor3f(0.75f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(segments.front().first, segments.front().second, z);
    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, 0.2, 16, 16);
    gluDeleteQuadric(quad);
    glPopMatrix();

    glColor3f(0.0f, 1.0f, 0.0f);
    for(int i=1; i<segments.size(); i++){
        std::pair<float, float> segment = segments.at(i);
        glPushMatrix();
        glTranslatef(segment.first, segment.second, z);
        GLUquadric* quad = gluNewQuadric();
        gluSphere(quad, 0.2, 16, 16);
        gluDeleteQuadric(quad);
        glPopMatrix();
    }
}

void Worm::move(float dx, float dy) {
    float newX = segments.front().first + dx;
    float newY = segments.front().second + dy;
    segments.insert(segments.begin(), { newX, newY });
    segments.pop_back();
}

void Worm::drop() {
    for (int i = 0; i < segments.size(); i++) {
        segments.at(i).second -= 1;
    }
}

void Worm::grow() {
    segments.push_back(segments.back());
}

std::vector< std::pair<float, float>> Worm::getSegments() {
    return segments;
}

float Worm::getHeadX() const { return segments.front().first; }
float Worm::getHeadY() const { return segments.front().second; }