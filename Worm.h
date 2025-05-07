#ifndef WORM_H
#define WORM_H

#include <vector>

class Worm {
public:
    Worm(float startX, float startY, float startZ);
    void draw() const;
    void move(float dx, float dz);
    void grow();
    float getHeadX() const;
    float getHeadZ() const;

private:
    std::vector<std::pair<float, float>> segments;
    float y;
};

#endif // WORM_H
