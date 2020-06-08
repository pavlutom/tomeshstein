//
// Created by tomesh on 1/9/19.
//

#ifndef TOMESHSTEIN_SHOOTERGAME_H
#define TOMESHSTEIN_SHOOTERGAME_H

#include <vector>
#include <map>
#include <algorithm>
#include <thread>

#include "../tpge/TPGE.h"
#include "../../objects/gameobject.h"
#include "../../player/player.h"
#include "../../objects/moving/enemy/enemy.h"
#include "../../objects/container/objectcontainer.h"

class CShooterGame : public tpge::CEngine {
public:
    CShooterGame(const char * title, unsigned width, unsigned height, unsigned short pixelsize, bool fullscreen, unsigned short renderingThreadCount, const char * mapName);
    CShooterGame(const tpge::CEngine & other, const char * mapName);

protected:
    enum ETile {
        EMPTY, ROOM, WALL, RED_WALL, DOOR, END, END_OPEN, MOSS_WALL
    };

    void onUserCreate() override;
    bool onUserUpdate(float elapsedTime, int & signal) override;

    bool manageProjectiles(int & signal); /* returns true if the player is killed by a projectile */
    void manageEnemies();
    void managePowerups();

    bool manageInput();

    bool validPosition(float x, float y, bool wallsOnly = true);
    int getMapIndex(int x, int y);
    bool clearPath(float x1, float y1, float x2, float y2, float precision);
    void openEnd();

    Uint32 blendColor(Uint32 color1, Uint32 color2, float blend);
    std::shared_ptr<CTexture> getTileTexture(ETile tile);

    void printWorld();
    void printWorldPart(int xFrom, int xTo, float distanceToScreen, bool isInside);
    void printWorldColumn(int x, float distanceToScreen, bool isInside);
    void printHurt();
    void printGUI();
    void printObjects();
    void printObjectsPart(float *viewAngles, int xFrom, int xTo, float distanceToScreen, bool isInside);
    void printMap();
    void printCrosshair();
    void printHealthBar();
    void printGun();
    void printKeys();

    const char * m_MapName;

    short m_MapWidth;
    short m_MapHeight;

    short m_EndX;
    short m_EndY;

    short m_UIScale;

    float m_RenderDistance;
    float m_FoV;

    CPlayer m_Player;

    ETile *m_Map;

    std::vector<std::shared_ptr<CTexture>> m_Textures;
    std::vector<Uint32> m_Palette;

    CObjectContainer m_Objects;

    float *m_DistanceBuffer;

    int m_CeilingY;
    Uint32 *m_WorldCacheOutside;
    Uint32 *m_WorldCacheInside;

    bool m_HoldingPrevGun;
    bool m_HoldingNextGun;

    float m_HurtTime;
};


#endif //TOMESHSTEIN_SHOOTERGAME_H
