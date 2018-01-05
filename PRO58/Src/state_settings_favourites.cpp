#include <stdint.h>

#include "state_settings_favourites.h"

#include "receiver.h"
#include "channels.h"

#include "settings.h"
#include "settings_internal.h"
#include "settings_eeprom.h"
#include "buttons.h"

#include "ui.h"
//#include "pstr_helper.h"

void StateMachine::SettingsFavouritesStateHandler::onEnter() {
    favIndex = 0;
}

void StateMachine::SettingsFavouritesStateHandler::onUpdate() {

}

void StateMachine::SettingsFavouritesStateHandler::onExit() {
    EepromSettings.save();
}

void StateMachine::SettingsFavouritesStateHandler::onButtonChange(
    Button button,
    Buttons::PressType pressType
) {
    if (pressType == Buttons::PressType::SHORT)
    {
    	if(button == Button::MODE){
    		favIndex = (favIndex + 1) % 8;
    	} else if(button == Button::UP){
    		++EepromSettings.favouriteChannels[favIndex];
    		if(EepromSettings.favouriteChannels[favIndex] == CHANNELS_SIZE)
    			EepromSettings.favouriteChannels[favIndex] = 0;
    	} else if(button == Button::DOWN){
    		--EepromSettings.favouriteChannels[favIndex];
    		if(EepromSettings.favouriteChannels[favIndex] == 255){
    			EepromSettings.favouriteChannels[favIndex] = CHANNELS_SIZE - 1;
    		}
    	}

    	Ui::needUpdate();
    }
}


void StateMachine::SettingsFavouritesStateHandler::onInitialDraw() {
    Ui::needUpdate(); // Lazy. :(
}

void StateMachine::SettingsFavouritesStateHandler::drawChannelText(uint8_t channel, uint8_t x, uint8_t y, uint8_t color) {
    Ui::display.setTextSize(2);
    Ui::display.setCursor(x, y);
    if(color == INVERSE){
    	Ui::display.setTextColor(BLACK);
    	Ui::display.fillRect(x-1,y-1,CHAR_WIDTH * 4 + 4, CHAR_HEIGHT * 2 + 2,WHITE);
    } else {
    	Ui::display.setTextColor(WHITE);
    }

    Ui::display.print(Channels::getName(channel));
}

void StateMachine::SettingsFavouritesStateHandler::onUpdateDraw() {
    Ui::clear();

    for(uint8_t i = 0;i < 4; i++){
    	drawChannelText(EepromSettings.favouriteChannels[i] % CHANNELS_SIZE, 2 + i * ((CHAR_WIDTH * 4) + 11), 10, i == favIndex ? INVERSE : WHITE);
    	drawChannelText(EepromSettings.favouriteChannels[i+4] % CHANNELS_SIZE, 2 + i * ((CHAR_WIDTH * 4) + 11), 36, (i + 4) == favIndex ? INVERSE : WHITE);
    }

    Ui::needDisplay();
}
