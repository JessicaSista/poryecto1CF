#include "apple.h"
#include "iostream"

Apple::Apple(float x, float y, float z) : x(x), y(y), z(z) {
    cargarModelo();
}

void Apple::cargarModelo() {
    Assimp::Importer importador;
    const aiScene* manzana = importador.ReadFile("../modelo/manzana.gltf", aiProcess_Triangulate);
    const aiMesh* malla = manzana->mMeshes[0];
    std::vector<Vertice> arregloVertices;
    for (unsigned int i = 0; i < malla->mNumFaces; i++) {
        for (unsigned int j = 0; j < 3; j++) {
            Vertice vert;
            vert.posX = malla->mVertices[malla->mFaces[i].mIndices[j]].x;
            vert.posY = malla->mVertices[malla->mFaces[i].mIndices[j]].y;
            vert.posZ = malla->mVertices[malla->mFaces[i].mIndices[j]].z;
            vert.normX = malla->mNormals[malla->mFaces[i].mIndices[j]].x;
            vert.normY = malla->mNormals[malla->mFaces[i].mIndices[j]].y;
            vert.normZ = malla->mNormals[malla->mFaces[i].mIndices[j]].z;
            vert.textX = malla->mTextureCoords[0][malla->mFaces[i].mIndices[j]].x;
            vert.textY = malla->mTextureCoords[0][malla->mFaces[i].mIndices[j]].y;
            arregloVertices.emplace_back(vert);
        }
    }
    this->modelo = arregloVertices;
}

void Apple::draw(GLuint texture) {
    glPushMatrix();
    glTranslatef(x, y, z);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
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
