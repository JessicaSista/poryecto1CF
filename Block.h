#ifndef BLOCK_H
#define BLOCK_H

class Block {
public:
    Block(float x, float y, float z);
    void draw() const;
    float getX() const;
    float getY() const;
    float getZ() const;

private:
    float x, y, z;
};

#endif // BLOCK_H