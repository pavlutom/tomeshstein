//
// Created by tomesh on 1/11/19.
//

#include "objectcontainer.h"


void CObjectContainer::sort() {
    std::sort(m_All.begin(), m_All.end());
    std::sort(m_Enemies.begin(), m_Enemies.end());
    std::sort(m_Projectiles.begin(), m_Projectiles.end());
    std::sort(m_PowerUps.begin(), m_PowerUps.end());
}

void CObjectContainer::removeEnemy(std::shared_ptr<CEnemy> enemy) {
    m_All.erase(std::find(m_All.begin(), m_All.end(), enemy));
    m_Enemies.erase(std::find(m_Enemies.begin(), m_Enemies.end(), enemy));
}

void CObjectContainer::removeProjectile(std::shared_ptr<CProjectile> projectile) {
    m_All.erase(std::find(m_All.begin(), m_All.end(), projectile));
    m_Projectiles.erase(std::find(m_Projectiles.begin(), m_Projectiles.end(), projectile));
}

void CObjectContainer::removePowerUp(std::shared_ptr<CPowerUp> powerup) {
    m_All.erase(std::find(m_All.begin(), m_All.end(), powerup));
    m_PowerUps.erase(std::find(m_PowerUps.begin(), m_PowerUps.end(), powerup));
}

const std::vector<std::shared_ptr<CGameObject>> & CObjectContainer::getAll() const {
    return m_All;
}

const std::vector<std::shared_ptr<CEnemy>> & CObjectContainer::getEnemies() const {
    return m_Enemies;
}

const std::vector<std::shared_ptr<CProjectile>> & CObjectContainer::getProjectiles() const {
    return m_Projectiles;
}

const std::vector<std::shared_ptr<CPowerUp>> & CObjectContainer::getPowerUps() const {
    return m_PowerUps;
}