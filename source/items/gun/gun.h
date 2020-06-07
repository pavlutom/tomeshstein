//
// Created by tomesh on 1/5/19.
//

#ifndef TOMESHSTEIN_GUN_H
#define TOMESHSTEIN_GUN_H

#include <memory>
#include "../../misc/texture/texture.h"
#include "../../engine/utils/rng/rng.h"

class CObjectContainer;

class CGun {
public:
    CGun();
    CGun(float cooldown, short power, float projV, float noise, const std::shared_ptr<CTexture> &texture);

    void update(float elapsedTime);

    void shoot(CObjectContainer & objects, float x, float y, float a);

    std::shared_ptr<CTexture> getTexture();
private:
    float m_SinceLastShot;
    float m_Cooldown;
    short m_Power;
    float m_ProjV;
    float m_Noise;

    std::shared_ptr<CTexture> m_Texture;
};


#endif //TOMESHSTEIN_GUN_H
