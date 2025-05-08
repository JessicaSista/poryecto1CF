#include "Camera.h"
#include <windows.h>   
#include <GL/gl.h>
#include <GL/glu.h>


void Camera::apply() const {
    gluLookAt(
        0.0, 5.0, 40.0,  // posici�n de la c�mara
        0.0, 0.0, 0.0,   // hacia d�nde mira
        0.0, 1.0, 0.0    // up vector
    );
}
