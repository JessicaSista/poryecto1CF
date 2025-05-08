#include "Camera.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

float cameraX = 0.0f, cameraY = 5.0f, cameraZ = 40.0f;  // Posici�n inicial de la c�mara
float cameraAngleX = 0.0f, cameraAngleY = 0.0f;  // �ngulos de rotaci�n de la c�mara
float zoomFactor = 40.0f;  // Zoom inicial (distancia de la c�mara)

bool mouseDragging = false;
int lastMouseX, lastMouseY;

void Camera::apply(float x, float y, float z) const {
    gluLookAt(
        cameraX, cameraY, cameraZ,  // posici�n de la c�mara
        0.0f, 0.0f, 0.0f,          // hacia d�nde mira
        0.0f, 1.0f, 0.0f           // up vector
    );
}

void Camera::update(SDL_Event& event) {
    // Detectar si se est� arrastrando el mouse
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            mouseDragging = true;
            lastMouseX = event.motion.x;
            lastMouseY = event.motion.y;
        }
    }
    if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            mouseDragging = false;
        }
    }

    if (event.type == SDL_MOUSEMOTION && mouseDragging) {
        // Cambiar los �ngulos de la c�mara basados en el movimiento del mouse
        int deltaX = event.motion.x - lastMouseX;
        int deltaY = event.motion.y - lastMouseY;

        cameraAngleX += deltaX * 0.1f;  // Movimiento en el eje X
        cameraAngleY += deltaY * 0.1f;  // Movimiento en el eje Y

        lastMouseX = event.motion.x;
        lastMouseY = event.motion.y;
    }

    if (event.type == SDL_MOUSEWHEEL) {
        // Zoom con la rueda del mouse
        if (event.wheel.y > 0) {  // Zoom in
            zoomFactor -= 2.0f;
        }
        else if (event.wheel.y < 0) {  // Zoom out
            zoomFactor += 2.0f;
        }
    }

    // Ajustamos la posici�n de la c�mara
    cameraX = zoomFactor * sin(cameraAngleX) * cos(cameraAngleY);
    cameraY = zoomFactor * sin(cameraAngleY);
    cameraZ = zoomFactor * cos(cameraAngleX) * cos(cameraAngleY);
}
