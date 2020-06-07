//
// Created by tomesh on 1/5/19.
//

#ifndef TOMESHSTEIN_PLAYER_H
#define TOMESHSTEIN_PLAYER_H

#include "../items/gun/gun.h"
#include <vector>
#include <ctgmath>
#include <functional>


class CPlayer {
public:
    CPlayer() = default;
    CPlayer(float x, float y, std::function<bool(float, float)> positionValidator);

    void update         (float elapsedTime);

    void moveForward    (bool focus = false);
    void moveBackward   (bool focus = false);
    void moveLeft       (bool focus = false);
    void moveRight      (bool focus = false);
    void rotateLeft     (bool focus = false);
    void rotateRight    (bool focus = false);

    void shoot          (CObjectContainer & objects);
    void previousGun();
    void nextGun();
    bool hurt(int health);
    void heal(int health);
    void findKey(short key);
    bool hasKey(short key);
    bool hasAllKeys();

    float getX();
    float getY();
    float getAngle();
    int getHP();
    std::shared_ptr<CTexture> getGunTexture();
    std::vector<std::shared_ptr<CTexture>> getGunTextures();

    template<typename ...Args>
    void addGun(Args && ...args) {
        m_Guns.emplace_back(args...);
    }

private:
    std::function<bool(float, float)> m_PositionValidator;

    int m_MaxHealth;
    int m_Health;
    float m_V;
    float m_VRot;
    float m_X;
    float m_Y;
    float m_Angle;

    short m_CurrentGun;
    std::vector<CGun> m_Guns;

    bool m_Keys[6];

    float m_ElapsedTime;
};


#endif //TOMESHSTEIN_PLAYER_H
