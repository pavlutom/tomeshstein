//
// Created by tomesh on 1/5/19.
//

#ifndef TOMESHSTEIN_PROJECTILE_H
#define TOMESHSTEIN_PROJECTILE_H

#include "../../../misc/texture/texture.h"
#include "../movinggameobject.h"


class CProjectile : public CMovingGameObject {
public:
    CProjectile(float x, float y, std::shared_ptr<CTexture> texture, float v, float angle, int m_Damage);

    void move() override;
    int getDamage();
    bool active();

protected:
    int m_Damage;
    float m_DistanceFromStart;
};


#endif //TOMESHSTEIN_PROJECTILE_H
