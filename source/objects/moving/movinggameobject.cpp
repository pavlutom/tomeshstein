//
// Created by tomesh on 1/9/19.
//

#include "movinggameobject.h"

CMovingGameObject::CMovingGameObject(float x, float y, std::shared_ptr<CTexture> texture, float v, float angle)
: CGameObject(x, y, std::move(texture)), m_V(v), m_Angle(angle) {}

void CMovingGameObject::move() {
    m_X += sinf(m_Angle) * m_V * m_ElapsedTime;
    m_Y += cosf(m_Angle) * m_V * m_ElapsedTime;
}
