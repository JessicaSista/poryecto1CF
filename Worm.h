#ifndef WORM_H
#define WORM_H

#include <vector>

class Worm {
public:
    Worm(float startX, float startY, float startZ);
    void draw() const;
    void drop();
    void move(float dx, float dy);
    void grow();
    float getHeadX() const;
    float getHeadY() const;
    std::vector< std::pair<float, float>> getSegments();

private:
    std::vector<std::pair<float, float>> segments;
    float z;
};

#endif // WORM_H
