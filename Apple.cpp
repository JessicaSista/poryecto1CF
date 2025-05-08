#include "apple.h"
#include "FreeImage.h"
#include <iostream>
#include <windows.h> 
#include <GL/gl.h>
#include <GL/glu.h>
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

Apple::Apple(float x, float y, float z) : x(x), y(y), z(z), texturaId(0) {
    cargarModelo();
}

void Apple::cargarModelo() {
    Assimp::Importer importador;
    const string rutaModelo = "../modelo/scene.gltf";
    const string directorioBase = "../modelo/";

    const aiScene* escena = importador.ReadFile(rutaModelo, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!escena || !escena->HasMeshes()) {
        std::cerr << "Error cargando modelo: " << importador.GetErrorString() << std::endl;
        return;
    }

    const aiMesh* malla = escena->mMeshes[0];

    std::vector<Vertice> arregloVertices;
    for (unsigned int i = 0; i < malla->mNumFaces; i++) {
        for (unsigned int j = 0; j < 3; j++) {
            unsigned int index = malla->mFaces[i].mIndices[j];
            Vertice vert;
            vert.posX = malla->mVertices[index].x;
            vert.posY = malla->mVertices[index].y;
            vert.posZ = malla->mVertices[index].z;
            vert.normX = malla->mNormals[index].x;
            vert.normY = malla->mNormals[index].y;
            vert.normZ = malla->mNormals[index].z;

            if (malla->HasTextureCoords(0)) {
                vert.textX = malla->mTextureCoords[0][index].x;
                vert.textY = malla->mTextureCoords[0][index].y;
            }
            else {
                vert.textX = vert.textY = 0.0f;
            }

            arregloVertices.emplace_back(vert);
        }
    }

    this->modelo = arregloVertices;
}

void Apple::draw() {
    glPushMatrix();
    glTranslatef(x, y, z);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaId);

    // No aplicar ningún material ni color, solo la textura original

    glBegin(GL_TRIANGLES);
    for (auto& vertice : modelo) {
        glTexCoord2f(vertice.textX, vertice.textY);
        glNormal3f(vertice.normX, vertice.normY, vertice.normZ);
        glVertex3f(vertice.posX, vertice.posY, vertice.posZ);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

float Apple::getX() const { return x; }
float Apple::getY() const { return y; }
float Apple::getZ() const { return z; }
