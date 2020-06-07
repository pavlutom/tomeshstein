//
// Created by tomesh on 1/7/19.
//

#ifndef TOMESHSTEIN_PATH_H
#define TOMESHSTEIN_PATH_H


#include <vector>

class CPath {
public:
    CPath(float startX, float startY);
    float getX(int i);
    float getY(int i);
    float getDistanceFrom(int i, float x, float y);
    CPath & addPoint(float startX, float startY);

    size_t getLenght();

private:
    struct TPoint {
        float m_X;
        float m_Y;
    };

    std::vector<TPoint> m_Points;
};


#endif //TOMESHSTEIN_PATH_H
