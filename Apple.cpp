#include "apple.h"
#include "FreeImage.h"
#include <windows.h> 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <fstream>

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

Apple::Apple(float x, float y, float z)
    : x(x), y(y), z(z) {
    cargarModelo();
}

void Apple::cargarModelo() {
    Assimp::Importer importer;
    const std::string modelPath = "../modelo/scene.gltf";
    const std::string baseDir = "../modelo/";

    const aiScene* scene = importer.ReadFile(modelPath,
        aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || !scene->HasMeshes()) {
        std::cerr << "Error cargando modelo: "
            << importer.GetErrorString() << std::endl;
        return;
    }

    subMeshes.clear();
    procesarNodo(scene->mRootNode, scene, baseDir);
}

void Apple::procesarNodo(const aiNode* node, const aiScene* scene, const std::string& baseDir) {
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

        // Cargar texture baseColor
        GLuint texID = cargarTextura(mat, aiTextureType_DIFFUSE, baseDir);

        // Extraer vértices de esta mesh
        std::vector<Vertice> verts;
        verts.reserve(mesh->mNumFaces * 3);
        for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
            const aiFace& face = mesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                unsigned int idx = face.mIndices[j];
                Vertice v;
                v.posX = mesh->mVertices[idx].x;
                v.posY = mesh->mVertices[idx].y;
                v.posZ = mesh->mVertices[idx].z;
                v.normX = mesh->mNormals[idx].x;
                v.normY = mesh->mNormals[idx].y;
                v.normZ = mesh->mNormals[idx].z;
                if (mesh->HasTextureCoords(0)) {
                    v.textX = mesh->mTextureCoords[0][idx].x;
                    v.textY = mesh->mTextureCoords[0][idx].y;
                }
                else {
                    v.textX = v.textY = 0.0f;
                }
                verts.push_back(v);
            }
        }

        subMeshes.push_back({ texID, std::move(verts) });
    }

    // Recursion para hijos
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        procesarNodo(node->mChildren[i], scene, baseDir);
    }
}

GLuint Apple::cargarTextura(const aiMaterial* material, aiTextureType type, const std::string& baseDir) {
    if (material->GetTextureCount(type) == 0)
        return 0;

    aiString path;
    material->GetTexture(type, 0, &path);
    if (path.length == 0) {
        return 0;
    }

    std::string fullPath = baseDir + path.C_Str();
    std::ifstream file(fullPath);
    if (!file.good()) {
        std::cerr << "No encontrado: " << fullPath << std::endl;
        return 0;
    }

    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fullPath.c_str(), 0);
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(fullPath.c_str());

    FIBITMAP* bmp = FreeImage_Load(fif, fullPath.c_str());
    if (!bmp) return 0;
    FIBITMAP* dib = FreeImage_ConvertTo24Bits(bmp);
    FreeImage_Unload(bmp);

    int w = FreeImage_GetWidth(dib);
    int h = FreeImage_GetHeight(dib);
    void* data = FreeImage_GetBits(dib);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
        GL_BGR, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FreeImage_Unload(dib);
    return tex;
}

void Apple::draw() {
    glPushMatrix();
    glTranslatef(x, y, z);

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    for (const auto& sub : subMeshes) {
        glBindTexture(GL_TEXTURE_2D, sub.texBaseColor);
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 1.0f, 1.0f);
        for (const auto& v : sub.vertices) {
            glTexCoord2f(v.textX, v.textY);
            glNormal3f(v.normX, v.normY, v.normZ);
            glVertex3f(v.posX, v.posY, v.posZ);
        }
        glEnd();
    }

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

float Apple::getX() const { return x; }
float Apple::getY() const { return y; }
float Apple::getZ() const { return z; }
