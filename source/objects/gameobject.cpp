#include <utility>

//
// Created by tomesh on 1/9/19.
//

#include "gameobject.h"

CGameObject::CGameObject(float x, float y, std::shared_ptr<CTexture> texture)
: m_X(x), m_Y(y), m_Texture(std::move(texture)),
m_dX(0.0f), m_dY(0.0f), m_DistanceFromPlayer(0.0f), m_ElapsedTime(0.0f),
m_Scale(1.0f) {}

void CGameObject::update(float elapsedTime) {
    m_ElapsedTime = elapsedTime;
}

void CGameObject::updateDistanceFromPlayer(float playerX, float playerY) {
    m_dX = m_X - playerX;
    m_dY = m_Y - playerY;
    m_DistanceFromPlayer = sqrtf(m_dX * m_dX + m_dY * m_dY);
}

float CGameObject::getX() const {
    return m_X;
}

float CGameObject::getY() const {
    return m_Y;
}

float CGameObject::getDistanceFromPlayer() const {
    return m_DistanceFromPlayer;
}

float CGameObject::getPlayerViewAngle() const {
    return atan2f(m_dY, m_dX);
}

std::shared_ptr<CTexture> CGameObject::getTexture() const {
    return m_Texture;
}

float CGameObject::getScale() const {
    return m_Scale;
}

bool operator<(const std::shared_ptr<CGameObject> & lhs, const std::shared_ptr<CGameObject> & rhs) {   // inverted to render the nearest object as the last
    return   lhs->m_DistanceFromPlayer > rhs->m_DistanceFromPlayer ||
           ((lhs->m_DistanceFromPlayer == rhs->m_DistanceFromPlayer && lhs->m_X > rhs->m_Y) ||
            (lhs->m_X == rhs->m_X && lhs->m_Y > rhs->m_Y));
}
