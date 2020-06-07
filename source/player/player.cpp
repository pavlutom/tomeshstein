//
// Created by tomesh on 1/5/19.
//

#include "player.h"

CPlayer::CPlayer(float x, float y, std::function<bool(float, float)> positionValidator)
        : m_PositionValidator(std::move(positionValidator)), m_MaxHealth(100), m_Health(m_MaxHealth), m_X(x), m_Y(y),
          m_Angle(0.0f), m_V(5.0f), m_VRot(2.75f), m_ElapsedTime(0),
          m_CurrentGun(0) {
    for (bool &key : m_Keys) {
        key = false;
    }
}

void CPlayer::update(float elapsedTime) {
    for (auto &i : m_Guns) {
        i.update(elapsedTime);
    }
    m_ElapsedTime = elapsedTime;
}

void CPlayer::moveForward(bool focus) {
    float dX = sinf(m_Angle) * m_V * m_ElapsedTime * (focus ? 0.5f : 1.0f);
    float dY = cosf(m_Angle) * m_V * m_ElapsedTime * (focus ? 0.5f : 1.0f);

    if (m_PositionValidator(m_X + dX, m_Y)) m_X += dX;
    if (m_PositionValidator(m_X, m_Y + dY)) m_Y += dY;
}

void CPlayer::moveBackward(bool focus) {
    float dX = sinf(m_Angle) * m_V * m_ElapsedTime * (focus ? 0.5f : 1.0f);
    float dY = cosf(m_Angle) * m_V * m_ElapsedTime * (focus ? 0.5f : 1.0f);

    if (m_PositionValidator(m_X - dX, m_Y)) m_X -= dX;
    if (m_PositionValidator(m_X, m_Y - dY)) m_Y -= dY;
}

void CPlayer::moveLeft(bool focus) {
    float dX = sinf(m_Angle - 1.57f) * m_V * m_ElapsedTime * (focus ? 0.5f : 1.0f);
    float dY = cosf(m_Angle - 1.57f) * m_V * m_ElapsedTime * (focus ? 0.5f : 1.0f);

    if (m_PositionValidator(m_X + dX, m_Y)) m_X += dX;
    if (m_PositionValidator(m_X, m_Y + dY)) m_Y += dY;
}

void CPlayer::moveRight(bool focus) {
    float dX = sinf(m_Angle - 1.57f) * m_V * m_ElapsedTime * (focus ? 0.5f : 1.0f);
    float dY = cosf(m_Angle - 1.57f) * m_V * m_ElapsedTime * (focus ? 0.5f : 1.0f);

    if (m_PositionValidator(m_X - dX, m_Y)) m_X -= dX;
    if (m_PositionValidator(m_X, m_Y - dY)) m_Y -= dY;
}

void CPlayer::rotateLeft(bool focus) {
    m_Angle -= m_VRot * m_ElapsedTime * (focus ? 0.33f : 1.0f);
    if (m_Angle <= -3.14159f) {
        m_Angle += 2 * 3.14159f;
    }
}

void CPlayer::rotateRight(bool focus) {
    m_Angle += m_VRot * m_ElapsedTime * (focus ? 0.33f : 1.0f);
    if (m_Angle >= 3.14159f) {
        m_Angle -= 2 * 3.14159f;
    }
}

void CPlayer::shoot(CObjectContainer &objects) {
    if (!m_Guns.empty()) {
        m_Guns[m_CurrentGun].shoot(objects, m_X, m_Y, m_Angle);
    }
}

void CPlayer::previousGun() {
    if (!m_Guns.empty()) {
        m_CurrentGun = (short) ((m_CurrentGun + m_Guns.size() - 1) % m_Guns.size());
    }
}

void CPlayer::nextGun() {
    if (!m_Guns.empty()) {
        m_CurrentGun = (short) ((m_CurrentGun + 1) % m_Guns.size());
    }
}

bool CPlayer::hurt(int dmg) {
    m_Health -= dmg;
    return m_Health <= 0;
}

void CPlayer::heal(int health) {
    m_Health += health;
    if (health == 0 || m_Health > m_MaxHealth) {
        m_Health = m_MaxHealth;
    }
}

void CPlayer::findKey(short key) {
    if (key >= 0 && key < 6) {
        m_Keys[key] = true;
    }
}

bool CPlayer::hasKey(short key) {
    if (key >= 0 && key < 6) {
        return m_Keys[key];
    }
    return false;
}

bool CPlayer::hasAllKeys() {
    bool res = true;
    for (auto &key : m_Keys) {
        res &= key;
    }
    return res;
}

float CPlayer::getX() {
    return m_X;
}

float CPlayer::getY() {
    return m_Y;
}

float CPlayer::getAngle() {
    return m_Angle;
}

int CPlayer::getHP() {
    return m_Health;
}

std::shared_ptr<CTexture> CPlayer::getGunTexture() {
    if (!m_Guns.empty()) {
        return m_Guns[m_CurrentGun].getTexture();
    }
    return nullptr;
}

std::vector<std::shared_ptr<CTexture>> CPlayer::getGunTextures() {
    std::vector<std::shared_ptr<CTexture>> tex;
    for (auto &g : m_Guns) {
        tex.push_back(g.getTexture());
    }
    return tex;
}
