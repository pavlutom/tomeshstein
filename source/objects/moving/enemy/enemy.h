//
// Created by tomesh on 1/7/19.
//

#ifndef TOMESHSTEIN_ENEMY_H
#define TOMESHSTEIN_ENEMY_H

#include <functional>
#include "../../../items/gun/gun.h"
#include "../../../misc/path/path.h"
#include "../movinggameobject.h"


class CEnemy : public CMovingGameObject {
public:
    CEnemy(std::shared_ptr<CTexture> texture, float v, short health, CGun gun, CPath path, float radius);
    CEnemy(short type, CPath path, const std::vector<std::shared_ptr<CTexture>> & textures);

    void update(float elapsedTime) override;

    void move() override;
    void shoot(CObjectContainer & objects, float targetX, float targetY);
    bool hurt(CObjectContainer & objects);
    bool playerInRadius();

    std::vector<short> getLoot() const;

private:
    float getAngleToTarget(float x, float y);

    short m_Health;
    CGun m_Gun;
    CPath m_Path;
    float m_Radius;
    unsigned m_CurrentPathPoint;

    std::function<std::vector<short>(double roll)> m_Loot;
};


#endif //TOMESHSTEIN_ENEMY_H
