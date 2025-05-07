#include "TextureLoader.h"
#include <windows.h> 
#include <FreeImage.h>
#include <iostream>

GLuint TextureLoader::loadTexture(const std::string& path) {
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);
    FIBITMAP* image = FreeImage_ConvertTo32Bits(FreeImage_Load(format, path.c_str()));
    if (!image) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return 0;
    }

    int width = FreeImage_GetWidth(image);
    int height = FreeImage_GetHeight(image);
    BYTE* bits = FreeImage_GetBits(image);

    for (int i = 0; i < width * height; ++i) {
        BYTE* pixel = bits + i * 4;
        std::swap(pixel[0], pixel[2]); // B <--> R
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
  

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bits);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FreeImage_Unload(image);
    return texID;
}
