#ifndef APPLE_H
#define APPLE_H
#include <windows.h>
#include "Vertice.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <vector>
#include <conio.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class Apple {
public:
    Apple(float x, float y, float z);
    void draw();
    float getX() const;
    float getY() const;
    float getZ() const;
    void cargarModelo();

private:
    float x, y, z;
    vector<Vertice> modelo;
    GLuint texturaId;
    GLuint cargarTexturaDesdeMaterial(const aiMaterial* material, const string& directorioBase);
};

#endif // APPLE_H
