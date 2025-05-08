#ifndef CAMERA_H
#define CAMERA_H
#include <windows.h> 
#include <SDL.h>

class Camera {
public:
    Camera() = default;

    // Método que aplica la transformación de la cámara usando gluLookAt
    void apply() const;

    // Método que actualiza la posición y los ángulos de la cámara basado en los eventos del mouse
    void update(SDL_Event& event);

private:
    // Posición de la cámara
    float cameraX = 0.0f, cameraY = 5.0f, cameraZ = 40.0f;

    // Ángulos de rotación de la cámara
    float cameraAngleX = 0.0f, cameraAngleY = 0.0f;

    // Factor de zoom (distancia desde la cámara)
    float zoomFactor = 40.0f;

    // Variables para manejar el arrastre del mouse
    bool mouseDragging = false;
    int lastMouseX, lastMouseY;
};

#endif
