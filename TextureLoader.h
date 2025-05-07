#pragma once
#include <windows.h>   
#include <GL/gl.h>
#include <string>

class TextureLoader {
public:
    static GLuint loadTexture(const std::string& path);
};
