//
// Created by tomesh on 1/14/19.
//

#include "powerup.h"

CPowerUp::CPowerUp(short type, float x, float y, const std::vector<std::shared_ptr<CTexture>> & textures)
: CGameObject(x, y, textures[0]) {
    m_Scale = 0.5;

    switch (type) {
        case 0: // 10 hp
            m_Texture = textures[11];
            m_AffectPlayer = [](CPlayer & player){
                player.heal(10);
                return false;
            };
            break;
        case 1: // 25 hp
            m_Texture = textures[12];
            m_AffectPlayer = [](CPlayer & player){
                player.heal(25);
                return false;
            };
            break;
        case 2: // 50 hp
            m_Texture = textures[13];
            m_AffectPlayer = [](CPlayer & player){
                player.heal(50);
                return false;
            };
            break;
        case 3: // full health
            m_Texture = textures[14];
            m_AffectPlayer = [](CPlayer & player){
                player.heal(0);
                return false;
            };
            break;
        case 4: // gun 0
            m_Texture = textures[15];
            m_AffectPlayer = [t = textures[5]](CPlayer & player){
                player.addGun(0.2f, 5, 25.0f, 0.04f, t);
                return false;
            };
            break;
        case 5: // gun 1
            m_Texture = textures[16];
            m_AffectPlayer = [t = textures[6]](CPlayer & player){
                player.addGun(0.1f, 3, 30.0f, 0.065f, t);
                return false;
            };
            break;
        case 6: // gun 2
            m_Texture = textures[17];
            m_AffectPlayer = [t = textures[7]](CPlayer & player){
                player.addGun(0.35f, 12, 15.0f, 0.1f, t);
                return false;
            };
            break;
        case 7: // gun 3
            m_Texture = textures[18];
            m_AffectPlayer = [t = textures[8]](CPlayer & player){
                player.addGun(0.4f, 15, 50.0f, 0.025f, t);
                return false;
            };
            break;
        case 8: // key 0
            m_Texture = textures[19];
            m_AffectPlayer = [](CPlayer & player){
                player.findKey(0);
                return false;
            };
            break;
        case 9: // key 1
            m_Texture = textures[20];
            m_AffectPlayer = [](CPlayer & player){
                player.findKey(1);
                return false;
            };
            break;
        case 10: // key 2
            m_Texture = textures[21];
            m_AffectPlayer = [](CPlayer & player){
                player.findKey(2);
                return false;
            };
            break;
        case 11: // key 3
            m_Texture = textures[22];
            m_AffectPlayer = [](CPlayer & player){
                player.findKey(3);
                return false;
            };
            break;
        case 12: // key 4
            m_Texture = textures[23];
            m_AffectPlayer = [](CPlayer & player){
                player.findKey(4);
                return false;
            };
            break;
        case 13: // key 5
            m_Texture = textures[24];
            m_AffectPlayer = [](CPlayer & player){
                player.findKey(5);
                return false;
            };
            break;
        case 14: // gun 4
            m_Texture = textures[30];
            m_AffectPlayer = [t = textures[28]](CPlayer & player){
                player.addGun(0.4f, 15, 50.0f, 0.025f, t);  // needs tuning
                return false;
            };
            break;
        default:
            break;
    }
}

bool CPowerUp::affectPlayer(CPlayer & player) {
    return m_AffectPlayer(player);
}
