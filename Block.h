#ifndef BLOCK_H
#define BLOCK_H
#include "Tile.h"

class Block {
public:
    Block(float x, float y, float z, Tile tipo);
    void draw() const;
    float getX() const;
    float getY() const;
    float getZ() const;
    Tile getTipo() const;

private:
    float x, y, z;
    Tile tipo;
};

#endif // BLOCK_H