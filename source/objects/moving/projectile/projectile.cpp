//
// Created by tomesh on 1/5/19.
//

#include "projectile.h"



CProjectile::CProjectile(float x, float y, std::shared_ptr<CTexture> texture, float v, float angle, int m_Damage)
: CMovingGameObject(x, y, std::move(texture), v, angle), m_Damage(m_Damage), m_DistanceFromStart(0.0f) {
    m_Scale = 0.05f;
}

void CProjectile::move() {
    float x = m_X, y = m_Y;
    CMovingGameObject::move();
    x = m_X - x;
    y = m_Y - y;

    m_DistanceFromStart += sqrtf(x * x + y * y);
}

int CProjectile::getDamage() {
    return m_Damage;
}

bool CProjectile::active() {
    return m_DistanceFromStart > 1.0f;
}