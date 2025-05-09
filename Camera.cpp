#include "Camera.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <cmath>  // Para las funciones trigonométricas

float cameraX = 0.0f, cameraY = 5.0f, cameraZ = 40.0f;  // Posición inicial de la cámara
float cameraAngleX = 0.0f, cameraAngleY = 0.0f;  // Ángulos de rotación de la cámara
float zoomFactor = 40.0f;  // Zoom inicial (distancia de la cámara)

bool mouseDragging = false;
int lastMouseX, lastMouseY;

void Camera::apply(float x, float y, float z) const {
    gluLookAt(
        cameraX, cameraY, cameraZ,  // Posición de la cámara
        x, y, z,                    // Hacia dónde mira (la cámara se dirige hacia el centro)
        0.0f, 1.0f, 0.0f           // Vectores de orientación (arriba)
    );
}

void Camera::update(SDL_Event& event) {
    // Detectar si se está arrastrando el mouse
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
        // Cambiar los ángulos de la cámara basados en el movimiento del mouse
        int deltaX = event.motion.x - lastMouseX;
        int deltaY = event.motion.y - lastMouseY;

        cameraAngleX += deltaX * 0.1f;  // Movimiento en el eje X
        cameraAngleY += deltaY * 0.1f;  // Movimiento en el eje Y

        lastMouseX = event.motion.x;
        lastMouseY = event.motion.y;

        // Limitamos el ángulo Y para evitar que la cámara se voltee completamente
        if (cameraAngleY > 89.0f) cameraAngleY = 89.0f;
        if (cameraAngleY < -89.0f) cameraAngleY = -89.0f;
    }

    if (event.type == SDL_MOUSEWHEEL) {
        // Zoom con la rueda del mouse
        if (event.wheel.y > 0) {  // Zoom in
            zoomFactor -= 2.0f;
        }
        else if (event.wheel.y < 0) {  // Zoom out
            zoomFactor += 2.0f;
        }

        if (zoomFactor < 10.0f) zoomFactor = 10.0f;  // Limitar el zoom mínimo
        if (zoomFactor > 100.0f) zoomFactor = 100.0f;  // Limitar el zoom máximo
    }

    // Actualizamos la posición de la cámara en base a los ángulos de la cámara
    cameraX = zoomFactor * cos(cameraAngleY * 3.14159f / 180.0f) * sin(cameraAngleX * 3.14159f / 180.0f);
    cameraY = zoomFactor * sin(cameraAngleY * 3.14159f / 180.0f);
    cameraZ = zoomFactor * cos(cameraAngleY * 3.14159f / 180.0f) * cos(cameraAngleX * 3.14159f / 180.0f);
}
