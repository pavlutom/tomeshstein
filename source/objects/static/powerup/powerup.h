//
// Created by tomesh on 1/14/19.
//

#ifndef TOMESHSTEIN_POWERUP_H
#define TOMESHSTEIN_POWERUP_H


#include <functional>
#include "../../gameobject.h"
#include "../../../player/player.h"

class CPowerUp : public CGameObject {
public:
    CPowerUp(short type, float x, float y, const std::vector<std::shared_ptr<CTexture>> & textures);

    bool affectPlayer(CPlayer & player);

protected:
    std::function<bool(CPlayer & player)> m_AffectPlayer;
};


#endif //TOMESHSTEIN_POWERUP_H
