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
    const aiMaterial* material = escena->mMaterials[malla->mMaterialIndex];

    texturaId = cargarTexturaDesdeMaterial(material, directorioBase);

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

GLuint Apple::cargarTexturaDesdeMaterial(const aiMaterial* material, const string& directorioBase) {
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString texPath;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);

        string rutaCompleta = directorioBase + string(texPath.C_Str());

        FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(rutaCompleta.c_str());
        if (fif == FIF_UNKNOWN) {
            std::cerr << "Formato de imagen no reconocido: " << rutaCompleta << std::endl;
            return 0;
        }

        FIBITMAP* bitmap = FreeImage_Load(fif, rutaCompleta.c_str());
        if (!bitmap) {
            std::cerr << "No se pudo cargar la textura: " << rutaCompleta << std::endl;
            return 0;
        }

        bitmap = FreeImage_ConvertTo24Bits(bitmap);
        int w = FreeImage_GetWidth(bitmap);
        int h = FreeImage_GetHeight(bitmap);
        void* datos = FreeImage_GetBits(bitmap);

        GLuint texId;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, datos);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        FreeImage_Unload(bitmap);

        return texId;
    }

    return 0;
}

void Apple::draw() {
    glPushMatrix();
    glTranslatef(x, y, z);

    // Rotar 90 grados alrededor del eje X para que quede "parada"
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    glDisable(GL_LIGHTING);  

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaId);
    glBegin(GL_TRIANGLES);
    for (auto& vertice : modelo) {
        glTexCoord2f(vertice.textX, vertice.textY);
        glNormal3f(vertice.normX, vertice.normY, vertice.normZ);
        glVertex3f(vertice.posX, vertice.posY, vertice.posZ);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);  

    glPopMatrix();
}



float Apple::getX() const { return x; }
float Apple::getY() const { return y; }
float Apple::getZ() const { return z; }
