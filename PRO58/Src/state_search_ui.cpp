#include "state_search.h"

#include "receiver.h"
#include "channels.h"
#include "ui.h"
#include "settings_eeprom.h"
//#include "pstr_helper.h"

using Ui::display;

void StateMachine::SearchStateHandler::drawScanBar() {
    uint8_t scanWidth = orderedChanelIndex * SCANBAR_W / CHANNELS_SIZE;

    display.fillRect(
        SCANBAR_X,
        SCANBAR_Y,
        scanWidth,
        SCANBAR_H,
        WHITE
    );
}

void StateMachine::SearchStateHandler::drawMenu() {
    this->menu.draw();
}
