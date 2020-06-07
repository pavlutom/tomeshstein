//
// Created by tomesh on 1/7/19.
//

#include "path.h"
#include <ctgmath>


CPath::CPath(float startX, float startY) : m_Points ({{startX, startY}}) {}

float CPath::getX(int i) {
    return m_Points[i%m_Points.size()].m_X;
}

float CPath::getY(int i) {
    return m_Points[i%m_Points.size()].m_Y;
}

float CPath::getDistanceFrom(int i, float x, float y) {
    return sqrtf((m_Points[i%m_Points.size()].m_X-x)*(m_Points[i%m_Points.size()].m_X-x) + (m_Points[i%m_Points.size()].m_Y-y)*(m_Points[i%m_Points.size()].m_Y-y));
}

CPath &CPath::addPoint(float startX, float startY) {
    m_Points.push_back({startX, startY});
    return *this;
}

size_t CPath::getLenght() {
    return m_Points.size();
}
