//
// Created by tomesh on 1/9/19.
//

#include "shootergame.h"


CShooterGame::CShooterGame(const char *title, unsigned width, unsigned height, unsigned short pixelsize,
                           bool fullscreen, unsigned short renderingThreadCount, const char *mapName)
        : tpge::CEngine(), m_MapName(mapName) {
    construct(title, width, height, pixelsize, fullscreen, renderingThreadCount);
}

CShooterGame::CShooterGame(const tpge::CEngine & other, const char *mapName)
        : tpge::CEngine(), m_MapName(mapName) {
    inWindowOf(other);
}

void CShooterGame::onUserCreate() {
    m_FoV = 1.3f; // 75 deg
    m_UIScale = getPixelSize() > 4 ? (unsigned short) 1 : (unsigned short) (4 / getPixelSize());

    // default value
    /* 00                */ m_Textures.push_back(std::make_shared<CTexture>("../textures/default.bmp"));
    // texture values
    /* 01 - red wall     */ m_Textures.push_back(std::make_shared<CTexture>("../textures/red_wall.bmp"));
    /* 02 - gray wall    */ m_Textures.push_back(std::make_shared<CTexture>("../textures/gray_wall.bmp"));
    /* 03 - door         */ m_Textures.push_back(std::make_shared<CTexture>("../textures/door.bmp"));
    /* 04 - portal       */ m_Textures.push_back(std::make_shared<CTexture>("../textures/portal.bmp"));
    /* 05 - shot0        */ m_Textures.push_back(std::make_shared<CTexture>("../textures/shot0.bmp"));
    /* 06 - shot1        */ m_Textures.push_back(std::make_shared<CTexture>("../textures/shot1.bmp"));
    /* 07 - shot2        */ m_Textures.push_back(std::make_shared<CTexture>("../textures/shot2.bmp"));
    /* 08 - shot3        */ m_Textures.push_back(std::make_shared<CTexture>("../textures/shot3.bmp"));
    /* 09 - enemy0       */ m_Textures.push_back(std::make_shared<CTexture>("../textures/enemy0.bmp"));
    /* 10 - enemy1       */ m_Textures.push_back(std::make_shared<CTexture>("../textures/enemy1.bmp"));
    /* 11 - 10 hp        */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_life1.bmp"));
    /* 12 - 25 hp        */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_life2.bmp"));
    /* 13 - 50 hp        */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_life3.bmp"));
    /* 14 - full health  */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_life_full.bmp"));
    /* 15 - gun0         */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_gun0.bmp"));
    /* 16 - gun1         */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_gun1.bmp"));
    /* 17 - gun2         */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_gun2.bmp"));
    /* 18 - gun3         */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_gun3.bmp"));
    /* 19 - key0         */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_key0.bmp"));
    /* 20 - key1         */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_key1.bmp"));
    /* 21 - key2         */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_key2.bmp"));
    /* 22 - key3         */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_key3.bmp"));
    /* 23 - key4         */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_key4.bmp"));
    /* 24 - key5         */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_key5.bmp"));
    /* 25 - end          */ m_Textures.push_back(std::make_shared<CTexture>("../textures/end.bmp"));
    /* 26 - end open     */ m_Textures.push_back(std::make_shared<CTexture>("../textures/end_open.bmp"));
    /* 27 - moss wall    */ m_Textures.push_back(std::make_shared<CTexture>("../textures/moss_wall.bmp"));
    /* 28 - shot4        */ m_Textures.push_back(std::make_shared<CTexture>("../textures/shot4.bmp"));
    /* 29 - enemy2       */ m_Textures.push_back(std::make_shared<CTexture>("../textures/enemy2.bmp"));
    /* 30 - gun4         */ m_Textures.push_back(std::make_shared<CTexture>("../textures/pu_gun4.bmp"));

    std::ifstream mapFile;
    char mapPath[256];
    sprintf(mapPath, "../maps/%s.tmap", m_MapName);
    mapFile.open(mapPath);

    float playerX, playerY;
    Uint32 sky, ground, ceiling, floor;
    char buffer[0xFF];
    std::stringstream lineStream;

    // load level configuration
    mapFile >> buffer >> m_MapWidth >>
            buffer >> m_MapHeight >>
            buffer >> playerX >>
            buffer >> playerY >>
            std::hex >>
            buffer >> sky >>
            buffer >> ground >>
            buffer >> ceiling >>
            buffer >> floor >>
            buffer >> m_RenderDistance >>
            std::dec;

    // load objects
    while (true) {
        mapFile.getline(buffer, 0xFF);

        // blank line
        if (buffer[0] == '\0') continue;

        // comment
        if (strncmp(buffer, "//", 2) == 0) continue;

        lineStream << buffer;
        lineStream >> buffer;

        // enemy
        if (strncmp(buffer, "enemy:", 7) == 0) {
            short type;
            float x, y;

            lineStream >> type;

            lineStream >> buffer[0];   // read '{'

            // first point (mandatory)
            lineStream >> buffer[0];   // read '['
            lineStream >> x;
            lineStream >> buffer[0]; // read ','
            lineStream >> y;
            lineStream >> buffer[0] >> buffer[0];   // read "]," (or "]}")

            CPath path(x, m_MapHeight - y);

            // other points
            while (buffer[0] != '}') {
                lineStream >> buffer[0];   // read '['
                lineStream >> x;
                lineStream >> buffer[0]; // read ','
                lineStream >> y;
                lineStream >> buffer[0] >> buffer[0];   // read "]," (or "]}")

                path.addPoint(x, m_MapHeight - y);
            }

            m_Objects.addEnemy(type, path, m_Textures);
        }

        // powerup
        if (strncmp(buffer, "powerup:", 9) == 0) {
            short type;
            float x, y;

            lineStream >> type;

            lineStream >> buffer[0];   // read '['
            lineStream >> x;
            lineStream >> buffer[0];   // read ','
            lineStream >> y;
            lineStream >> buffer[0];   // read ']'

            m_Objects.addPowerUp(type, x, m_MapHeight - y, m_Textures);
        }

        if (strncmp(buffer, "map:", 5) == 0) {
            break;
        }
    }

    m_Map = new ETile[m_MapWidth * m_MapHeight];
    char c;
    for (int y = m_MapHeight - 1; y >= 0; y--) {
        for (int x = 0; x < m_MapWidth; x++) {
            mapFile >> c;
            switch (c) {
                case '#':
                    m_Map[getMapIndex(x, y)] = ETile::WALL;
                    break;
                case '@':
                    m_Map[getMapIndex(x, y)] = ETile::RED_WALL;
                    break;
                case '&':
                    m_Map[getMapIndex(x, y)] = ETile::MOSS_WALL;
                    break;
                case 'D':
                    m_Map[getMapIndex(x, y)] = ETile::DOOR;
                    break;
                case '+':
                    m_Map[getMapIndex(x, y)] = ETile::ROOM;
                    break;
                case 'F':
                    m_EndX = (short) x;
                    m_EndY = (short) y;
                    m_Map[getMapIndex(x, y)] = ETile::END;
                    break;
                default:
                    m_Map[getMapIndex(x, y)] = ETile::EMPTY;
                    break;
            }
        }
    }

    mapFile.close();

    m_Player = CPlayer(playerX, m_MapHeight - playerY, [&](float x, float y){ return validPosition(x, y); });

    // default value
    /* 00                */ m_Palette.push_back(0xFF000000);
    // palette values
    /* 01 - sky          */ m_Palette.push_back(sky);
    /* 02 - ground       */ m_Palette.push_back(ground);
    /* 03 - wall_shade   */ m_Palette.push_back(blendColor(sky, TPGEColor::BLACK, 0.75f));
    /* 04 - ground_shade */ m_Palette.push_back(blendColor(sky, TPGEColor::BLACK, 0.75f -
                                                                                  powf(m_RenderDistance < 20.0f ? 0 :
                                                                                       (m_RenderDistance - 20) /
                                                                                       m_RenderDistance, 2)));
    /* 05 - GUI color    */ m_Palette.push_back(TPGEColor::LIME);
    /* 06 - ceiling      */ m_Palette.push_back(ceiling);
    /* 07 - floor        */ m_Palette.push_back(floor);

    m_DistanceBuffer = new float[getScreenWidth()];

    int screenH = getScreenHeight();
    m_WorldCacheOutside = new Uint32[screenH];
    m_WorldCacheInside = new Uint32[screenH];
    for (int y = 0; y < screenH / 2; ++y) {
        float shade = cbrtf(((screenH - y) - screenH * 0.5f - screenH / m_RenderDistance) /
                            (screenH * 0.5f));

        // ceiling
        m_WorldCacheOutside[y] = m_Palette[1];
        m_WorldCacheInside[y] = blendColor(m_Palette[6], TPGEColor::BLACK, shade);

        // floor
        m_WorldCacheOutside[screenH - 1 - y] = blendColor(m_Palette[2], m_Palette[4], shade);
        m_WorldCacheInside[screenH - 1 - y] = blendColor(m_Palette[7], TPGEColor::BLACK, shade);
    }

    m_HoldingPrevGun = false;
    m_HoldingNextGun = false;
    m_HurtTime = 0.0f;
}

bool CShooterGame::onUserUpdate(float elapsedTime, int & signal) {
    m_Player.update(elapsedTime);

    if (!manageInput()) {
        signal = 0;
        return false;
    }

    // end
    if ((int) m_Player.getX() == m_EndX && (int) m_Player.getY() == m_EndY) {
        signal = 1;
        return false;
    }

    // manage all objects
    for (const auto &o : m_Objects.getAll()) {
        o->updateDistanceFromPlayer(m_Player.getX(), m_Player.getY());
        o->update(elapsedTime);
    }

    // sort objects (because locations changed)
    m_Objects.sort();

    // manage collisions etc.
    if (manageProjectiles(signal)) return false;
    manageEnemies();
    managePowerups();

    printWorld();

    printObjects();

    if (m_HurtTime > 0) {
        m_HurtTime -= elapsedTime;
        printHurt();
    }

    printGUI();

    printFrame();

    setTitle("%s (%.f FPS)", getTitle(), 1 / elapsedTime);

    return true;
}

bool CShooterGame::manageProjectiles(int & signal) {
    for (auto p = m_Objects.getProjectiles().rbegin(); p != m_Objects.getProjectiles().rend(); ++p) {
        if ((*p)->active() && (*p)->getDistanceFromPlayer() < 0.5f) {   // hit player
            if (m_Player.hurt((*p)->getDamage())) {  // player dead
                signal = 0;
                return true;   // game over
            }
            m_Objects.removeProjectile(*p);
            m_HurtTime = 0.25f;
        } else {
            (*p)->move();
            if (!validPosition((*p)->getX(), (*p)->getY(), false)) {
                m_Objects.removeProjectile(*p);
            }
        }
    }
    return false;
}

void CShooterGame::manageEnemies() {
    for (auto e = m_Objects.getEnemies().rbegin(); e != m_Objects.getEnemies().rend(); ++e) {
        if ((*e)->hurt(m_Objects)) {    // enemy dies
            auto loot = (*e)->getLoot();
            for (auto &item : loot) {
                CPowerUp p(item,
                           (*e)->getX() + rng::uniform_real<float>(-0.5f, 0.5f),
                           (*e)->getY() + rng::uniform_real<float>(-0.5f, 0.5f),
                           m_Textures);
                p.updateDistanceFromPlayer(m_Player.getX(), m_Player.getY());
                m_Objects.addPowerUp(p);
            }
            m_Objects.removeEnemy(*e);
            continue;
        }

        if ((*e)->playerInRadius() &&
            clearPath((*e)->getX(), (*e)->getY(), m_Player.getX(), m_Player.getY(), 0.1f)) {  // enemy can see player
            (*e)->shoot(m_Objects, m_Player.getX(), m_Player.getY());   // don't move and shoot in player's direction
        } else {
            (*e)->move();   // move along defined path
        }
    }
}

void CShooterGame::managePowerups() {
    for (auto u = m_Objects.getPowerUps().rbegin(); u != m_Objects.getPowerUps().rend(); ++u) {
        if ((*u)->getDistanceFromPlayer() < 0.5f) {     // picked up by player
            (*u)->affectPlayer(m_Player);
            if (m_Player.hasAllKeys()) {
                openEnd();
            }
            m_Objects.removePowerUp(*u);
        }
    }
}

bool CShooterGame::manageInput() {
    // quit
    if (isKeyPressed(SDL_SCANCODE_ESCAPE)) {
        return false;
    }

    bool focus = isKeyPressed(SDL_SCANCODE_L);
    // move
    if (isKeyPressed(SDL_SCANCODE_W)) {
        m_Player.moveForward(focus);
    }
    if (isKeyPressed(SDL_SCANCODE_S)) {
        m_Player.moveBackward(focus);
    }
    if (isKeyPressed(SDL_SCANCODE_A)) {
        if (isKeyPressed(SDL_SCANCODE_LSHIFT) || isKeyPressed(SDL_SCANCODE_RSHIFT)) {
            m_Player.moveLeft(focus);
        } else {
            m_Player.rotateLeft(focus);
        }
    }
    if (isKeyPressed(SDL_SCANCODE_D)) {
        if (isKeyPressed(SDL_SCANCODE_LSHIFT) || isKeyPressed(SDL_SCANCODE_RSHIFT)) {
            m_Player.moveRight(focus);
        } else {
            m_Player.rotateRight(focus);
        }
    }

    // switch previous gun
    if (isKeyPressed(SDL_SCANCODE_J)) {
        if (!m_HoldingPrevGun) {
            m_Player.previousGun();
            m_HoldingPrevGun = true;
        }
    } else {
        m_HoldingPrevGun = false;
    }

    // switch next gun
    if (isKeyPressed(SDL_SCANCODE_I)) {
        if (!m_HoldingNextGun) {
            m_Player.nextGun();
            m_HoldingNextGun = true;
        }
    } else {
        m_HoldingNextGun = false;
    }

    // shoot
    if (isKeyPressed(SDL_SCANCODE_K)) {
        m_Player.shoot(m_Objects);
    }

    // debug
    if (isKeyPressed(SDL_SCANCODE_SPACE)) {
        std::cout << "X: " << m_Player.getX() << " Y: " << m_MapHeight - m_Player.getY() << std::endl;
    }
    if (isKeyPressed(SDL_SCANCODE_M)) {
        std::cout << "next" << std::endl;
    }

    return true;
}

bool CShooterGame::validPosition(float x, float y, bool wallsOnly) {
    return x >= 0 && x < m_MapWidth && y >= 0 && y < m_MapHeight
           && ((wallsOnly && m_Map[getMapIndex((int) x, (int) y)] != ETile::WALL &&
                m_Map[getMapIndex((int) x, (int) y)] != ETile::RED_WALL &&
                m_Map[getMapIndex((int) x, (int) y)] != ETile::MOSS_WALL &&
                m_Map[getMapIndex((int) x, (int) y)] != ETile::END)
               || m_Map[getMapIndex((int) x, (int) y)] == ETile::EMPTY
               || m_Map[getMapIndex((int) x, (int) y)] == ETile::ROOM);
}

int CShooterGame::getMapIndex(int x, int y) {
    return y * m_MapWidth + x;
}

bool CShooterGame::clearPath(float x1, float y1, float x2, float y2, float precision) {
    float angle = atan2f(x2 - x1, y2 - y1);
    float dX = sinf(angle) * precision;
    float dY = cosf(angle) * precision;

    while ((m_Map[getMapIndex((int) x1, (int) y1)] == ETile::EMPTY || m_Map[getMapIndex((int) x1, (int) y1)] == ETile::ROOM) &&
           (fabs(x1 - x2) > precision || fabs(y1 - y2) > precision)) {
        x1 += dX;
        y1 += dY;
    }

    return fabs(x1 - x2) <= precision && fabs(y1 - y2) <= precision;
}

void CShooterGame::openEnd() {
    m_Map[getMapIndex(m_EndX, m_EndY)] = ETile::END_OPEN;
}

Uint32 CShooterGame::blendColor(Uint32 color1, Uint32 color2, float blend) {
    Uint32 res = 0xFF000000;

    *((Uint8 *) &res + 2) = (Uint8) (*((Uint8 *) &color1 + 2) * blend + *((Uint8 *) &color2 + 2) * (1 - blend));
    *((Uint8 *) &res + 1) = (Uint8) (*((Uint8 *) &color1 + 1) * blend + *((Uint8 *) &color2 + 1) * (1 - blend));
    *((Uint8 *) &res) = (Uint8) (*((Uint8 *) &color1) * blend + *((Uint8 *) &color2) * (1 - blend));

    return res;
}

std::shared_ptr<CTexture> CShooterGame::getTileTexture(ETile tile) {
    switch (tile) {
        case ETile::WALL:
            return m_Textures[2];
        case ETile::RED_WALL:
            return m_Textures[1];
        case ETile::DOOR:
            return m_Textures[3];
        case ETile::END:
            return m_Textures[25];
        case ETile::END_OPEN:
            return m_Textures[26];
        case ETile::MOSS_WALL:
            return m_Textures[27];
        default:
            return m_Textures[0];
    }
}

void CShooterGame::printWorld() {
    float distanceToScreen = (getScreenWidth() * 0.5f) / tan(m_FoV * 0.5f);
    bool isInside = m_Map[getMapIndex((int) m_Player.getX(), (int) m_Player.getY())] == ETile::ROOM;

    unsigned short numThreads = getRenderingThreadCount();
    int xPart = getScreenWidth() / numThreads;
    std::thread threads[numThreads];
    for (int t = 0; t < numThreads; ++t) {
        int from = t * xPart;
        int to = t + 1 == numThreads ? getScreenWidth() : (t + 1) * xPart;
        threads[t] = std::thread(&CShooterGame::printWorldPart, this, from, to, distanceToScreen, isInside);
    }
    for (int t = 0; t < numThreads; ++t) {
        threads[t].join();
    }
}
void CShooterGame::printWorldPart(int xFrom, int xTo, float distanceToScreen, bool isInside) {
    for (int x = xFrom; x < xTo; ++x) {
        printWorldColumn(x, distanceToScreen, isInside);
    }
}

void CShooterGame::printWorldColumn(int x, float distanceToScreen, bool isInside) {
    float angle = atan2f(x - getScreenWidth() * 0.5f, distanceToScreen);

    float step = 0.005f;
    float distance = 0.0f;
    float dX = sinf(m_Player.getAngle() + angle) * step;
    float dY = cosf(m_Player.getAngle() + angle) * step;
    float beamX = m_Player.getX();
    float beamY = m_Player.getY();

    while ((m_Map[getMapIndex((int) beamX, (int) beamY)] == ETile::EMPTY || m_Map[getMapIndex((int) beamX, (int) beamY)] == ETile::ROOM) && distance <= m_RenderDistance) {
        distance += step;
        beamX += dX;
        beamY += dY;
    }
    m_DistanceBuffer[x] = distance;

    ETile tile = m_Map[getMapIndex((int) beamX, (int) beamY)];
    std::shared_ptr<CTexture> tex = getTileTexture(tile);



    int ceiling = (int) (getScreenHeight() * 0.5f - getScreenHeight() / distance / cosf(angle));

    float tX = beamY;
    if (abs(beamX - round(beamX)) > abs(beamY - round(beamY))) {
        tX = beamX;
    }

    for (int y = 0; y < getScreenHeight(); y++) {
        if (y < ceiling || y > getScreenHeight() - ceiling) {
            drawPixel(x, y, isInside ? m_WorldCacheInside[y] : m_WorldCacheOutside[y]);
        } else {
            float shade = 1 - (distance / m_RenderDistance);
            float tY = (float) (y - ceiling) / (getScreenHeight() - 2 * ceiling);
            Uint32 col = tex->get(tX, tY);
            drawPixel(x, y, blendColor(col, (isInside ? TPGEColor::BLACK : m_Palette[3]), shade));
        }
    }
}

void CShooterGame::printHurt() {
    blendScreen(0.3f, TPGEColor::RED);
}

void CShooterGame::printGUI() {
    printMap();
    printCrosshair();
    printHealthBar();
    printGun();
    printKeys();
}

void CShooterGame::printObjects() {
    float distanceToScreen = (getScreenWidth() * 0.5f) / tan(m_FoV * 0.5f);
    bool inside = m_Map[getMapIndex((int) m_Player.getX(), (int) m_Player.getY())] == ETile::ROOM;

    for (auto &o : m_Objects.getAll()) {
        float objectViewAngle =
                atan2f(cosf(m_Player.getAngle()), sinf(m_Player.getAngle())) - o->getPlayerViewAngle();
        objectViewAngle += (objectViewAngle < -3.14f ? 6.28f : (objectViewAngle > 3.14 ? -6.28 : 0.0f));

        if (o->getDistanceFromPlayer() > 0.5f && o->getDistanceFromPlayer() <= m_RenderDistance &&
            fabsf(objectViewAngle) <=
            m_FoV * 0.65f) { // visible (0.5f would trigger when middle of the enemy is out of screen bounds)
            float height = (getScreenHeight() -
                            2 * (getScreenHeight() * 0.5f -
                                 getScreenHeight() / o->getDistanceFromPlayer() / cosf(objectViewAngle))) *
                           o->getScale();
            float width = o->getTexture()->getAspectRatio() * height;
            int oY = (int) (getScreenHeight() * 0.5f -
                            getScreenHeight() / o->getDistanceFromPlayer() / cosf(objectViewAngle) +
                            ((1 - o->getScale()) * 0.5f * height / o->getScale()));
            int oX = (int) (distanceToScreen * tanf(objectViewAngle) + (getScreenWidth() - width) * 0.5f);
            for (int x = 0; x < width; x++) {
                if (o->getDistanceFromPlayer() <= m_DistanceBuffer[oX + x]) {
                    for (int y = 0; y < height; y++) {
                        Uint32 col = o->getTexture()->get(x / width, y / height);
                        if ((col & 0x00FFFFFF) != 0) { // black = transparent
                            int scX = oX + x;
                            int scY = oY + y;
                            if (scX >= 0 && scX < getScreenWidth() && scY >= 0 && scY < getScreenHeight()) {
                                float shade = 1 - (o->getDistanceFromPlayer() / m_RenderDistance);
                                drawPixel(scX, scY, blendColor(col, (inside ? TPGEColor::BLACK : m_Palette[3]), sqrt(shade)));
                            }
                        }
                    }
                }
            }
        }
    }
}

void CShooterGame::printMap() {
    for (int y = 0; y < m_MapHeight; y++) {
        for (int x = 0; x < m_MapWidth; x++) {
            float blend = 0.5f;
            Uint32 tileColor = TPGEColor::BLACK;
            if (m_Map[getMapIndex(x, y)] == ETile::WALL ||
                m_Map[getMapIndex(x, y)] == ETile::RED_WALL ||
                m_Map[getMapIndex(x, y)] == ETile::MOSS_WALL) {
                blend = 1.0f;
                tileColor = m_Palette[5];
            }if (m_Map[getMapIndex(x, y)] == ETile::END || m_Map[getMapIndex(x, y)] == ETile::END_OPEN) {
                blend = 1.0f;
                tileColor = TPGEColor::CYAN;
            }
            if (m_Map[getMapIndex(x, y)] == ETile::DOOR) {
                blend = 1.0f;
                tileColor = TPGEColor::BLUE;
            }
            if (x == (int) m_Player.getX() && y == (int) m_Player.getY()) {
                blend = 1.0f;
                tileColor = TPGEColor::YELLOW;
            }
            for (int i = 0; i < m_UIScale; i++) {
                for (int j = 0; j < m_UIScale; j++) {
                    int scX = x * m_UIScale + i;
                    int scY = (m_MapHeight - 1 - y) * m_UIScale + j;
                    if (scX < getScreenWidth() && scY < getScreenHeight()) {
                        blendPixel(scX, scY, blend, tileColor);
                    }
                }
            }
        }
    }

    // projectiles
    for (auto &p : m_Objects.getProjectiles()) {
        for (int i = 0; i < m_UIScale; i++) {
            for (int j = 0; j < m_UIScale; j++) {
                int scX = (int) p->getX() * m_UIScale + i;
                int scY = (m_MapHeight - 1 - (int) p->getY()) * m_UIScale + j;
                if (scX < getScreenWidth() && scY < getScreenHeight()) {
                    drawPixel(scX, scY, TPGEColor::WHITE);
                }
            }
        }
    }

    // enemies
    for (auto &e : m_Objects.getEnemies()) {
        for (int i = 0; i < m_UIScale; i++) {
            for (int j = 0; j < m_UIScale; j++) {
                int scX = (int) e->getX() * m_UIScale + i;
                int scY = (m_MapHeight - 1 - (int) e->getY()) * m_UIScale + j;
                if (scX < getScreenWidth() && scY < getScreenHeight()) {
                    drawPixel(scX, scY, TPGEColor::RED);
                }
            }
        }
    }

    // powerups
    for (auto &u : m_Objects.getPowerUps()) {
        // hide keys
        bool isKey = false;
        for (int key = 0; key < 6; key++) {
            isKey |= u->getTexture() == m_Textures[key + 19];
        }
        if (isKey) {
            continue;
        }

        // print powerups
        for (int i = 0; i < m_UIScale; i++) {
            for (int j = 0; j < m_UIScale; j++) {
                int scX = (int) u->getX() * m_UIScale + i;
                int scY = (m_MapHeight - 1 - (int) u->getY()) * m_UIScale + j;
                if (scX < getScreenWidth() && scY < getScreenHeight()) {
                    drawPixel(scX, scY, TPGEColor::CORAL);
                }
            }
        }
    }
}

void CShooterGame::printCrosshair() {
    for (int i = 0; i < 8 + m_UIScale - ((getPixelSize() <= 4) ? getPixelSize() : 0); i++) {

        drawPixel(getScreenWidth() / 2 - (i + 4) + 1, getScreenHeight() / 2 - (i + 4) + 1, m_Palette[5]);
        drawPixel(getScreenWidth() / 2 + (i + 4) + 1, getScreenHeight() / 2 - (i + 4) + 1, m_Palette[5]);
        drawPixel(getScreenWidth() / 2 - (i + 4) + 1, getScreenHeight() / 2 + (i + 4) + 1, m_Palette[5]);
        drawPixel(getScreenWidth() / 2 + (i + 4) + 1, getScreenHeight() / 2 + (i + 4) + 1, m_Palette[5]);

        drawPixel(getScreenWidth() / 2 - (i + 4) - 1, getScreenHeight() / 2 - (i + 4) - 1, m_Palette[5]);
        drawPixel(getScreenWidth() / 2 + (i + 4) - 1, getScreenHeight() / 2 - (i + 4) - 1, m_Palette[5]);
        drawPixel(getScreenWidth() / 2 - (i + 4) - 1, getScreenHeight() / 2 + (i + 4) - 1, m_Palette[5]);
        drawPixel(getScreenWidth() / 2 + (i + 4) - 1, getScreenHeight() / 2 + (i + 4) - 1, m_Palette[5]);

        drawPixel(getScreenWidth() / 2 - (i + 4) + 1, getScreenHeight() / 2 - (i + 4) - 1, m_Palette[5]);
        drawPixel(getScreenWidth() / 2 + (i + 4) + 1, getScreenHeight() / 2 - (i + 4) - 1, m_Palette[5]);
        drawPixel(getScreenWidth() / 2 - (i + 4) + 1, getScreenHeight() / 2 + (i + 4) - 1, m_Palette[5]);
        drawPixel(getScreenWidth() / 2 + (i + 4) + 1, getScreenHeight() / 2 + (i + 4) - 1, m_Palette[5]);

        drawPixel(getScreenWidth() / 2 - (i + 4) - 1, getScreenHeight() / 2 - (i + 4) + 1, m_Palette[5]);
        drawPixel(getScreenWidth() / 2 + (i + 4) - 1, getScreenHeight() / 2 - (i + 4) + 1, m_Palette[5]);
        drawPixel(getScreenWidth() / 2 - (i + 4) - 1, getScreenHeight() / 2 + (i + 4) + 1, m_Palette[5]);
        drawPixel(getScreenWidth() / 2 + (i + 4) - 1, getScreenHeight() / 2 + (i + 4) + 1, m_Palette[5]);
    }
}

void CShooterGame::printHealthBar() {
    for (int i = 0; i < m_UIScale; i++) {
        for (int j = 0; j < m_UIScale; j++) {
            for (int x = 0; x < 16; x++) {
                for (int y = 0; y < 14; y++) {
                    if (!((x + y < 7) || (15 - x + y < 7) || (15 - x + (15 - y) < 4) || (x + (15 - y) < 4) ||
                          ((15 - x + (15 - y) < 12) && (x + (15 - y) < 12)))) { // <3
                        drawPixel((x + 2) * m_UIScale + j, getScreenHeight() - (y + 3) * m_UIScale + i, TPGEColor::RED);
                    }
                }
            }
        }
    }
    int hp = m_Player.getHP();
    drawString(19 * m_UIScale, getScreenHeight() - 16 * m_UIScale, (hp > 25 ? m_Palette[5]: TPGEColor::RED), m_UIScale, "%d", hp);
}

void CShooterGame::printGun() {
    auto tex = m_Player.getGunTextures();
    int size = 15;

    for (int i = 0; i < m_UIScale; i++) {
        for (int j = 0; j < m_UIScale; j++) {
            for (int x = 0; x < size + 1; x++) {
                for (int y = 0; y < size + 1; y++) {
                    for (int t = 0; t < tex.size(); t++) {
                        Uint32 col = tex[t]->get((float) x / size, (float) y / size);
                        if ((col & 0x00FFFFFF) != 0) {
                            drawPixel(
                                    getScreenWidth() - (((int) tex.size() - 1 - t) * (size + 2) + (x + 2)) * m_UIScale +
                                    j,
                                    getScreenHeight() - (y + 2) * m_UIScale + i, col);
                        } else if (tex[t] == m_Player.getGunTexture()) {
                            drawPixel(
                                    getScreenWidth() - (((int) tex.size() - 1 - t) * (size + 2) + (x + 2)) * m_UIScale +
                                    j,
                                    getScreenHeight() - (y + 2) * m_UIScale + i, m_Palette[5]);
                        }
                    }
                }
            }
        }
    }
}

void CShooterGame::printKeys() {
    int size = 24;
    int idxOffset = 19;
    int keyCnt = 6;

    for (int i = 0; i < m_UIScale; i++) {
        for (int j = 0; j < m_UIScale; j++) {
            for (int x = 0; x <= size; x++) {
                for (int y = 0; y <= size; y++) {
                    for (short t = 0; t < keyCnt; t++) {
                        Uint32 col = m_Textures[t + idxOffset]->get((float) x / size, (float) y / size);
                        if ((col & 0x00FFFFFF) != 0) {
                            if (m_Player.hasKey(t)) {
                                drawPixel(
                                        (getScreenWidth() - 8 - (keyCnt - t) * (size / 2) + x) * m_UIScale + j,
                                        (y + 1) * m_UIScale + i, col);
                            } else {
                                blendPixel(
                                        (getScreenWidth() - 8 - (keyCnt - t) * (size / 2) + x) * m_UIScale + j,
                                        (y + 1) * m_UIScale + i, 0.25f, TPGEColor::BLACK);
                            }
                        }
                    }
                }
            }
        }
    }
}
