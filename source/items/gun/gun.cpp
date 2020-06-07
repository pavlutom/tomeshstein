//
// Created by tomesh on 1/5/19.
//

#include "gun.h"
#include "../../objects/container/objectcontainer.h"

CGun::CGun() : m_Cooldown(0.0f), m_Power(0), m_ProjV(0.0f), m_Noise(0.0f) {}

CGun::CGun(float cooldown, short power, float projV, float noise, const std::shared_ptr<CTexture> &texture)
: m_SinceLastShot(MAXFLOAT), m_Cooldown(cooldown), m_Power(power), m_ProjV(projV), m_Noise(noise), m_Texture(texture) {}

void CGun::update(float elapsedTime) {
    m_SinceLastShot += elapsedTime;
}

void CGun::shoot(CObjectContainer & objects, float x, float y, float a) {
    if (m_SinceLastShot > m_Cooldown) {
        objects.addProjectile(x, y, m_Texture, m_ProjV, a + (m_SinceLastShot < m_Cooldown * 1.5 ? rng::uniform_real<float>(-m_Noise, m_Noise) : 0.0f), m_Power);
        m_SinceLastShot = 0.0f;
    }
}

std::shared_ptr<CTexture> CGun::getTexture() {
    return m_Texture;
}
