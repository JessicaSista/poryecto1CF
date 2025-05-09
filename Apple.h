#ifndef APPLE_H
#define APPLE_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Vertice.h"

class Apple {
public:
    Apple(float x, float y, float z);
    void draw();
    float getX() const;
    float getY() const;
    float getZ() const;

private:
    struct SubMesh {
        GLuint texBaseColor;
        std::vector<Vertice> vertices;
    };

    float x, y, z;
    std::vector<SubMesh> subMeshes;

    void cargarModelo();
    void procesarNodo(const aiNode* node, const aiScene* scene, const std::string& baseDir);
    GLuint cargarTextura(const aiMaterial* material, aiTextureType type, const std::string& baseDir);
};

#endif // APPLE_H