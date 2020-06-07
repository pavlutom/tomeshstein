//
// Created by tomesh on 1/7/19.
//

#include <iostream>
#include "enemy.h"
#include "../../container/objectcontainer.h"


CEnemy::CEnemy(std::shared_ptr<CTexture> texture, float v, short health, CGun gun, CPath path, float radius)
: CMovingGameObject(path.getX(0), path.getY(0), std::move(texture), v, 0.0f),
m_Health(health), m_Gun(std::move(gun)), m_Path(std::move(path)), m_Radius(radius), m_CurrentPathPoint(0) {}

void CEnemy::update(float elapsedTime) {
    CMovingGameObject::update(elapsedTime);
    m_Gun.update(elapsedTime);
}

void CEnemy::move() {
    if (m_Path.getLenght() > 1) {
        if (m_Path.getDistanceFrom(m_CurrentPathPoint, m_X, m_Y) < 0.1f) {
            m_CurrentPathPoint++;
        }
        m_Angle = getAngleToTarget(m_Path.getX(m_CurrentPathPoint), m_Path.getY(m_CurrentPathPoint));
        CMovingGameObject::move();
    }
}

void CEnemy::shoot(CObjectContainer & objects, float targetX, float targetY) {
    m_Gun.shoot(objects, m_X, m_Y, getAngleToTarget(targetX, targetY));
}

bool CEnemy::hurt(CObjectContainer & objects) {
    for (auto p = objects.getProjectiles().rbegin(); p != objects.getProjectiles().rend(); ++p) {
        if ((*p)->active() && sqrtf((m_X - (*p)->getX()) * (m_X - (*p)->getX()) + (m_Y - (*p)->getY()) * (m_Y - (*p)->getY())) < 0.5f) {
            m_Health -= (*p)->getDamage();
            objects.removeProjectile(*p);
            break;
        }
    }
    return m_Health <= 0;
}

float CEnemy::getAngleToTarget(float x, float y) {
    return atan2f(x - m_X, y - m_Y);
}

bool CEnemy::playerInRadius() {
//    return false;
    return m_DistanceFromPlayer <= m_Radius;
}

std::vector<short> CEnemy::getLoot() const {
    return m_Loot(rng::uniform_real<double>(0.0, 100.0));
}

CEnemy::CEnemy(short type, CPath path, const std::vector<std::shared_ptr<CTexture>> & textures)
: CMovingGameObject(path.getX(0), path.getY(0), textures[0], 0.0f, 0.0f),
m_Path(std::move(path)), m_CurrentPathPoint(0) {
    switch (type) {
        case 0:     // green - normal shooting
            m_Health = 30;
            m_V = 3.0f;
            m_Gun = CGun(1.0f, 15, 10.0f, 0.05f, textures[8]);
            m_Radius = 10.0f;
            m_Texture = textures[9];
            m_Loot = [](double roll) {
                std::vector<short> loot;
                if (roll <= 10.0) { loot.push_back(0); }
                if (roll <= 5.0) { loot.push_back(0); }
                return loot;
            };
            break;
        case 1:     // red - "AOE"
            m_Health = 40;
            m_V = 2.0f;
            m_Gun = CGun(0.05f, 5, 3.0f, 1.57f, textures[6]);
            m_Radius = 12.5f;
            m_Texture = textures[10];
            m_Loot = [](double roll) {
                std::vector<short> loot;
                if (roll <= 15.0) { loot.push_back(0); }
                if (roll <= 7.5) { loot.push_back(0); }
                if (roll <= 1.0) { loot.push_back(0); }
                return loot;
            };
            break;
        case 2:     // blue - "sniper"
            m_Health = 50;
            m_V = 1.0f;
            m_Gun = CGun(3.5f, 25, 15.0f, 0.0f, textures[28]);
            m_Radius = 20.0f;
            m_Texture = textures[29];
            m_Loot = [](double roll) {
                std::vector<short> loot;
                if (roll <= 5.0) { loot.push_back(1); }
                return loot;
            };
            break;

        default:    // default trash bullshit (dies instantly)
            m_Health = 0;
            m_V = 0;
            m_Gun = CGun(MAXFLOAT, 0, 0.0f, MAXFLOAT, textures[0]);
            m_Radius = 0.0f;
            m_Texture = textures[0];
            m_Loot = [](double roll) { return std::vector<short>(); };
            break;
    }
}
