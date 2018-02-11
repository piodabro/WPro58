#include "state_favourite_screen.h"

#include "receiver.h"
#include "channels.h"
#include "ui.h"
#include "settings_eeprom.h"
//#include "pstr_helper.h"

using Ui::display;


void StateMachine::FavouriteScreenStateHandler::drawScanBar() {
    uint8_t scanWidth = this->favouriteIndex * SCANBAR_W / 7;

    display.fillRect(
        SCANBAR_X,
        SCANBAR_Y,
        scanWidth,
        SCANBAR_H,
        WHITE
    );
}
