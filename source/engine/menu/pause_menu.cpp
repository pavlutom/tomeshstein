//
// Created by tomesh on 6/18/20.
//

#include "pause_menu.h"

CPauseMenu::CPauseMenu(const tpge::CEngine &other) : CMenu(other) {}

void CPauseMenu::onUserCreate() {
    m_UIScale = getPixelSize() > 4 ? (unsigned short) 1 : (unsigned short) (4 / getPixelSize());

    m_Menus.emplace("pause", std::make_shared<TMenu>(TMenu({"Continue", "Quit"}, [&](int selected) {
        switch (selected) {
            case 0: // continue
            case 1: // quit game
                return true;
            default:
                return false;
        }
    })));

    setMenu("pause");
    m_Selected = 0;
    m_HoldingKey = false;
}

bool CPauseMenu::onUserUpdate(float elapsedTime, int &signal) {
    bool result = CMenu::onUserUpdate(elapsedTime, signal);
    if (!result) {
        signal = m_Selected;
    }
    return result;
}
