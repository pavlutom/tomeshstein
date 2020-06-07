//
// Created by tomesh on 1/9/19.
//

#ifndef TOMESHSTEIN_GAMEOBJECT_H
#define TOMESHSTEIN_GAMEOBJECT_H

#include <memory>
#include <ctgmath>

#include "../misc/texture/texture.h"

class CGameObject {
public:
    CGameObject(float x, float y, std::shared_ptr<CTexture> texture);

    virtual void update(float elapsedTime);

    void updateDistanceFromPlayer(float playerX, float playerY);

    float getX() const;
    float getY() const;
    float getDistanceFromPlayer() const;
    float getPlayerViewAngle() const;
    std::shared_ptr<CTexture> getTexture() const;
    float getScale() const;
    friend bool operator<(const std::shared_ptr<CGameObject> & lhs, const std::shared_ptr<CGameObject> & rhs);

protected:
    float m_X;
    float m_Y;
    float m_dX;
    float m_dY;
    float m_DistanceFromPlayer;
    float m_ElapsedTime;
    float m_Scale;
    std::shared_ptr<CTexture> m_Texture;
};


#endif //TOMESHSTEIN_GAMEOBJECT_H
