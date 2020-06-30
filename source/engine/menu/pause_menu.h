//
// Created by tomesh on 6/18/20.
//

#ifndef TOMESHSTEIN_PAUSE_MENU_H
#define TOMESHSTEIN_PAUSE_MENU_H

#include "menu.h"

class CPauseMenu : public CMenu {
public:
    CPauseMenu(const tpge::CEngine &other);
protected:
    void onUserCreate() override;
    bool onUserUpdate(float elapsedTime, int &signal) override;
};


#endif //TOMESHSTEIN_PAUSE_MENU_H
