//
// Created by tomesh on 1/27/19.
//

#ifndef TOMESHSTEIN_MAINMENU_H
#define TOMESHSTEIN_MAINMENU_H


#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <dirent.h>
#include "../game/shootergame.h"
#include "../tpge/TPGE.h"
#include "../../misc/texture/texture.h"

class CMenu : public tpge::CEngine {
public:
    CMenu(const char * title, unsigned width, unsigned height, unsigned short pixelsize, bool fullscreen, unsigned short renderingThreadCount);

protected:
    struct TMenu {
        TMenu(std::vector<std::string> items, std::function<bool(int)> execute)
        : m_Items(std::move(items)), m_Execute(std::move(execute)) {}

        inline int size() { return (int) m_Items.size(); }
        inline std::string operator[] (int i) { return m_Items[i]; }

        std::vector<std::string> m_Items;
        std::function<bool(int)> m_Execute;
    };

    void onUserCreate() override;
    bool onUserUpdate(float elapsedTime, int & signal) override;

    void setMenu(const char * name);

    bool manageInput();
    bool getMenuResult();

    std::vector<std::string> scanDirectory(const char * path, bool stripExtension=true);
    void getMaps();
    void getSaves();
    void loadSave(const char * name);

    void drawHeader();
    void drawCurrentMenu();

    short m_UIScale;
    CTexture m_Header;
    TMenu m_CurrentMenu;
    std::map<const char *, std::shared_ptr<TMenu>> m_Menus;
    int m_Selected;
    int m_Top;
    bool m_HoldingKey;

    std::string m_Name;
    int m_Current;
    int m_Unlocked;
};


#endif //TOMESHSTEIN_MAINMENU_H
