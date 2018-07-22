#include "state.h"
#include "state_settings.h"
#include "ui.h"
#include "ui_menu.h"
#include <cstring>
//#include "pstr_helper.h"


void StateMachine::SettingsStateHandler::onInitialDraw() {
    Ui::clear();

    this->menu.drawMenu();

    Ui::needDisplay();
}

void StateMachine::SettingsStateHandler::onUpdateDraw() {
    this->menu.drawMenu();
    Ui::needDisplay();
}

