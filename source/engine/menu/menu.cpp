//
// Created by tomesh on 1/27/19.
//

#include "menu.h"

CMenu::CMenu(const char *title, unsigned width, unsigned height, unsigned short pixelsize, bool fullscreen)
        : m_Header("../textures/menu_header.bmp"),
        m_CurrentMenu({}, [](int){ return false; }) {
    construct(title, width, height, pixelsize, fullscreen);

}

void CMenu::onUserCreate() {
    m_UIScale = getPixelSize() > 4 ? (unsigned short) 1 : (unsigned short) (4 / getPixelSize());

    m_Menus.emplace("main", std::make_shared<TMenu>(TMenu({"New game", "Load game", "Options", "Quit game"}, [&](int selected){
        std::shared_ptr<CShooterGame> g;
        std::ofstream save;
        switch (selected) {
            case 0: // new game
                // request name from player
                drawRectangle(0, 100 * m_UIScale, getScreenWidth(), 20 * m_CurrentMenu.size() * m_UIScale, TPGEColor::BLACK);
                drawString((getScreenWidth() - 264) / 2, 100 * m_UIScale, TPGEColor::WHITE, m_UIScale, "Name: ");
                printFrame();
                char buffer[0xFF];
                if (!readText(buffer, (getScreenWidth() - 264) / 2 + 72, 100 * m_UIScale, TPGEColor::WHITE, m_UIScale)) {
                    // window closed
                    return true;
                }

                if (*buffer != 0) {
                    // create savefile
                    char name[0xFF];
                    strcpy(name, buffer);
                    sprintf(buffer, "../savegames/%s.tsave", name);
                    save.open(buffer);
                    save << "1" << std::endl << "1" << std::endl;
                    save.close();
                    m_Current = m_Unlocked = 1;
                    m_Name = name;

                    // go into game menu
                    setMenu("game");
                }

                return false;
            case 1: // load game
                getSaves();
                setMenu("load");

                return false;
            case 2: // options
                setMenu("options");
                return false;
            case 3: // quit game
                return true;
            default:
                return false;
        }
    })));

    m_Menus.emplace("load", std::make_shared<TMenu>(TMenu({}, [&](int selected){
        if (selected < m_Menus["load"]->m_Items.size() - 1) {
            m_Name = m_CurrentMenu[selected];
            loadSave(m_Name.c_str());
            setMenu("game");
        } else if (selected == m_Menus["load"]->m_Items.size() - 1) {
            setMenu("main");
        }

        return false;
    })));

    m_Menus.emplace("options", std::make_shared<TMenu>(TMenu({"TBA", "Back to Main menu"}, [&](int selected){
        switch (selected) {
            case 0:
                return false;
            case 1: // back to main menu
                setMenu("main");
                return false;
            default:
                return false;
        }
    })));

    m_Menus.emplace("game", std::make_shared<TMenu>(TMenu({"Continue", "Select level", "Back to Main menu"}, [&](int selected){
        char buffer[0xFF];
        std::shared_ptr<CShooterGame> g;
        std::ofstream save;
        switch (selected) {
            case 0: // continue
                sprintf(buffer, "Level %02d", m_Current);
                g = std::make_shared<CShooterGame>(*this, buffer);

                if (g->run()) {
                    if (m_Current == m_Unlocked) {
                        m_Unlocked++;
                    }
                    m_Current++;
                }

                sprintf(buffer, "../savegames/%s.tsave", m_Name.c_str());
                save.open(buffer);
                save << m_Current << std::endl << m_Unlocked << std::endl;
                save.close();

                return false;
            case 1: // select level
                getMaps();
                setMenu("level");
                return false;
            case 2: // back to main menu
                setMenu("main");
                return false;
            default:
                return false;
        }
    })));

    m_Menus.emplace("level", std::make_shared<TMenu>(TMenu({}, [&](int selected){
        if (selected < m_Menus["level"]->m_Items.size() - 1) {
            // run game
            auto map = m_CurrentMenu[selected];
            std::shared_ptr<CShooterGame> g = std::make_shared<CShooterGame>(*this, map.c_str());
            if (map.length() == 8 && map.substr(0, 6) == "Level ") {    // level
                m_Current = std::stoi(map.substr(6, 2));

                if (g->run()) {
                    if (m_Current == m_Unlocked) {
                        m_Unlocked++;
                    }
                    m_Current++;
                }

                std::ofstream save;
                char buffer[0xFF];
                sprintf(buffer, "../savegames/%s.tsave", m_Name.c_str());
                save.open(buffer);
                save << m_Current << std::endl << m_Unlocked << std::endl;
                save.close();
            } else {
                g->run();
            }
        }
        setMenu("game");
        return false;
    })));

    setMenu("main");
    m_Selected = 0;
    m_HoldingKey = false;
}

bool CMenu::onUserUpdate(float elapsedTime, int & signal) {
    if (manageInput()) {
        if (getMenuResult()) {
            return false;
        }
    }

    drawRectangle(0, 0, getScreenWidth(), getScreenHeight(), TPGEColor::BLACK);
    drawHeader();
    drawCurrentMenu();
    printFrame();
    return true;
}

void CMenu::setMenu(const char *name) {
    m_CurrentMenu = *m_Menus[name];
    m_Selected = 0;
    m_Top = 0;
}

bool CMenu::manageInput() {
    if (isKeyPressed(SDL_SCANCODE_UP) || isKeyPressed(SDL_SCANCODE_W)) {
        if (!m_HoldingKey) {
            m_Selected--;
            if (m_Selected == -1) {
                m_Selected++;
            }
            if (m_Top >= m_Selected && m_Selected != 0) {
                m_Top--;
            }
            m_HoldingKey = true;
        }
        return false;
    }
    if (isKeyPressed(SDL_SCANCODE_DOWN) || isKeyPressed(SDL_SCANCODE_S)) {
        if (!m_HoldingKey) {
            m_Selected++;
            if (m_Selected == m_CurrentMenu.size()) {
                m_Selected--;
            }
            if (m_Selected - m_Top >= 3 && m_Selected != m_CurrentMenu.size() - 1) {
                m_Top++;
            }
            m_HoldingKey = true;
        }
        return false;
    }
    if (isKeyPressed(SDL_SCANCODE_RETURN) || isKeyPressed(SDL_SCANCODE_K)) {
        if (!m_HoldingKey) {
            m_HoldingKey = true;
            return true;
        }
        return false;
    }

    m_HoldingKey = false;
    return false;
}

bool CMenu::getMenuResult() {
    return m_CurrentMenu.m_Execute(m_Selected);
}

std::vector<std::string> CMenu::scanDirectory(const char * path, bool stripExtension) {
    std::vector<std::string> files;
    DIR *dirp = opendir(path);
    struct dirent *dp;
    while ((dp = readdir(dirp)) != nullptr)
    {
        std::string f (dp->d_name);
        if (f[0] != '.') {
            if (stripExtension) {
                for (size_t i = 0; i < f.length(); i++) {
                    if (f[i] == '.') {
                        f = f.substr(0, i);
                        break;
                    }
                }
            }
            files.push_back(f);
        }
    }
    std::sort(files.begin(), files.end(), [](const std::string & a, const std::string & b){
        std::string a1 = a, b1 = b;
        std::transform(a1.begin(), a1.end(), a1.begin(), ::tolower);
        std::transform(b1.begin(), b1.end(), b1.begin(), ::tolower);
        return a1 < b1;
    });
    closedir(dirp);
    return files;
}

void CMenu::getMaps() {
    m_Menus["level"]->m_Items.clear();
    auto items = scanDirectory("../maps");
    std::vector<std::string> other;
    for (auto &i : items) {
        if (i.length() == 8 && i.substr(0, 6) == "Level ") {
            if (std::stoi(i.substr(6, 2)) <= m_Unlocked) {
                m_Menus["level"]->m_Items.push_back(i);
            }
        } else {
            other.push_back(i);
        }
    }
    m_Menus["level"]->m_Items.insert(m_Menus["level"]->m_Items.end(), other.begin(), other.end());
    m_Menus["level"]->m_Items.emplace_back("Back to Game menu");
}

void CMenu::getSaves() {
    m_Menus["load"]->m_Items = scanDirectory("../savegames");
    m_Menus["load"]->m_Items.emplace_back("Back to Main menu");
}

void CMenu::loadSave(const char * name) {
    char buffer[0xFF];
    sprintf(buffer, "../savegames/%s.tsave", name);
    std::ifstream save;
    save.open(buffer);
    save >> m_Current >> m_Unlocked;
    save.close();
}

void CMenu::drawHeader() {
    int sizeX = 100;
    int sizeY = 50;
    int scale = 2 * m_UIScale;
    int offsetX = (getScreenWidth() - sizeX * scale) / 2;
    int offsetY = 0;

    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            for (int i = 0; i < scale; i++) {
                for (int j = 0; j < scale; j++) {
                    drawPixel(offsetX + x * scale + i, offsetY + y * scale + j,
                              m_Header.get((float) x / (sizeX + 1), (float) y / (sizeY + 1)));
                }
            }
        }
    }
}

void CMenu::drawCurrentMenu() {
    int offsetX;
    int offsetY = 100;

    if (m_CurrentMenu.size() < 5) {
        for (int i = 0; i < m_CurrentMenu.size(); i++) {
            offsetX = (getScreenWidth() - m_UIScale * 12 * (int) m_CurrentMenu[i].length()) / 2;
            drawString(offsetX, (offsetY + 20 * i) * m_UIScale, (i == m_Selected ? TPGEColor::RED : TPGEColor::WHITE), m_UIScale,
                       m_CurrentMenu[i].c_str());
        }
    } else {
        if (m_CurrentMenu.size() - m_Top == 4) {
            drawString((getScreenWidth() - m_UIScale * 36) / 2, offsetY * m_UIScale, TPGEColor::WHITE, m_UIScale, "...");
            for (int i = 1; i < 4; i++) {
                offsetX = (getScreenWidth() - m_UIScale * 12 * (int) m_CurrentMenu[i+m_Top].length()) / 2;
                drawString(offsetX, (offsetY + 20 * i) * m_UIScale, (i+m_Top == m_Selected ? TPGEColor::RED : TPGEColor::WHITE), m_UIScale,
                           m_CurrentMenu[i+m_Top].c_str());
            }
        } else if (m_Top == 0) {
            for (int i = 0; i < 3; i++) {
                offsetX = (getScreenWidth() - m_UIScale * 12 * (int) m_CurrentMenu[i].length()) / 2;
                drawString(offsetX, (offsetY + 20 * i) * m_UIScale, (i == m_Selected ? TPGEColor::RED : TPGEColor::WHITE), m_UIScale,
                           m_CurrentMenu[i].c_str());
            }
            drawString((getScreenWidth() - m_UIScale * 36) / 2, (offsetY + 60) * m_UIScale, TPGEColor::WHITE, m_UIScale, "...");
        } else {
            drawString((getScreenWidth() - m_UIScale * 36) / 2, offsetY * m_UIScale, TPGEColor::WHITE, m_UIScale, "...");
            for (int i = 1; i < 3; i++) {
                offsetX = (getScreenWidth() - m_UIScale * 12 * (int) m_CurrentMenu[i+m_Top].length()) / 2;
                drawString(offsetX, (offsetY + 20 * i) * m_UIScale, (i+m_Top == m_Selected ? TPGEColor::RED : TPGEColor::WHITE), m_UIScale,
                           m_CurrentMenu[i+m_Top].c_str());
            }
            drawString((getScreenWidth() - m_UIScale * 36) / 2, (offsetY + 60) * m_UIScale, TPGEColor::WHITE, m_UIScale, "...");
        }
    }

}
