//
// Created by tomesh on 1/11/19.
//

#ifndef TOMESHSTEIN_OBJECTCONTAINER_H
#define TOMESHSTEIN_OBJECTCONTAINER_H


#include <memory>
#include <algorithm>
#include <vector>
#include "../../misc/texture/texture.h"
#include "../gameobject.h"
#include "../moving/enemy/enemy.h"
#include "../moving/projectile/projectile.h"
#include "../static/powerup/powerup.h"

class CObjectContainer {
protected:
    std::vector<std::shared_ptr<CGameObject>>   m_All;
    std::vector<std::shared_ptr<CEnemy>>        m_Enemies;
    std::vector<std::shared_ptr<CProjectile>>   m_Projectiles;
    std::vector<std::shared_ptr<CPowerUp>>      m_PowerUps;

public:
    CObjectContainer() = default;

    void sort();

    template<typename ...Args>
    void addEnemy(Args && ...args) {
        std::shared_ptr<CEnemy> enemy = std::make_shared<CEnemy>(args...);

        m_All.insert(std::upper_bound(m_All.begin(), m_All.end(), enemy), enemy);
        m_Enemies.insert(std::upper_bound(m_Enemies.begin(), m_Enemies.end(), enemy), enemy);
    }

    template<typename ...Args>
    void addProjectile(Args && ...args) {
        std::shared_ptr<CProjectile> projectile = std::make_shared<CProjectile>(args...);

        m_All.insert(std::upper_bound(m_All.begin(), m_All.end(), projectile), projectile);
        m_Projectiles.insert(std::upper_bound(m_Projectiles.begin(), m_Projectiles.end(), projectile), projectile);
    }

    template<typename ...Args>
    void addPowerUp(Args && ...args) {
        std::shared_ptr<CPowerUp> powerup = std::make_shared<CPowerUp>(args...);

        m_All.insert(std::upper_bound(m_All.begin(), m_All.end(), powerup), powerup);
        m_PowerUps.insert(std::upper_bound(m_PowerUps.begin(), m_PowerUps.end(), powerup), powerup);
    }

    void removeEnemy(std::shared_ptr<CEnemy> enemy);
    void removeProjectile(std::shared_ptr<CProjectile> enemy);
    void removePowerUp(std::shared_ptr<CPowerUp> powerup);

    const std::vector<std::shared_ptr<CGameObject>> & getAll() const;
    const std::vector<std::shared_ptr<CEnemy>> & getEnemies() const;
    const std::vector<std::shared_ptr<CProjectile>> & getProjectiles() const;
    const std::vector<std::shared_ptr<CPowerUp>> & getPowerUps() const;
};


#endif //TOMESHSTEIN_OBJECTCONTAINER_H
