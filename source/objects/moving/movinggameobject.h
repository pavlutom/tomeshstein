//
// Created by tomesh on 1/9/19.
//

#ifndef TOMESHSTEIN_MOVINGGAMEOBJECT_H
#define TOMESHSTEIN_MOVINGGAMEOBJECT_H


#include "../gameobject.h"

class CMovingGameObject : public CGameObject {
public:
    CMovingGameObject(float x, float y, std::shared_ptr<CTexture> texture, float v, float angle);

    virtual void move();

protected:
    float m_V;
    float m_Angle;
};


#endif //TOMESHSTEIN_MOVINGGAMEOBJECT_H
