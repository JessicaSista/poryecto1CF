#ifndef APPLE_H
#define APPLE_H
#include <windows.h>  
#include <GL/gl.h>
#include <GL/glu.h>

class Apple {
public:
    Apple(float x, float y, float z);
    void draw() const;
    float getX() const;
    float getY() const;
    float getZ() const;

private:
    float x, y, z;
};

#endif // APPLE_H
