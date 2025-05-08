#ifndef CAMERA_H
#define CAMERA_H
#include <windows.h> 
#include <SDL.h>

class Camera {
public:
    Camera() = default;

    // M�todo que aplica la transformaci�n de la c�mara usando gluLookAt
    void apply() const;

    // M�todo que actualiza la posici�n y los �ngulos de la c�mara basado en los eventos del mouse
    void update(SDL_Event& event);

private:
    // Posici�n de la c�mara
    float cameraX = 0.0f, cameraY = 5.0f, cameraZ = 40.0f;

    // �ngulos de rotaci�n de la c�mara
    float cameraAngleX = 0.0f, cameraAngleY = 0.0f;

    // Factor de zoom (distancia desde la c�mara)
    float zoomFactor = 40.0f;

    // Variables para manejar el arrastre del mouse
    bool mouseDragging = false;
    int lastMouseX, lastMouseY;
};

#endif
