#include "state_favourite_screen.h"

#include "settings.h"
#include "settings_internal.h"
#include "settings_eeprom.h"
#include "receiver.h"
#include "channels.h"
#include "buttons.h"
#include "ui.h"
#include "OSD.h"

using StateMachine::FavouriteScreenStateHandler;

void FavouriteScreenStateHandler::onEnter() {
#ifdef USE_OSD
    OSD::enableLCD(false);
    OSD::setSyncMode(OSD::syncModes::external);
#endif
    for(int i=0;i<8;i++){
		if(EepromSettings.startChannel == EepromSettings.favouriteChannels[i]){
			this->favouriteIndex = i;
			return;
		}
	}
	this->favouriteIndex = 0;

	this->setChannel();
}

void FavouriteScreenStateHandler::onUpdate() {
	Ui::needUpdate();
}

void FavouriteScreenStateHandler::onExit(){
#ifdef USE_OSD
    OSD::enableLCD(true);
    OSD::setSyncMode(OSD::syncModes::automatic);
    OSD::clear();
#endif
}

void FavouriteScreenStateHandler::onButtonChange(
    Button button,
    Buttons::PressType pressType
) {
	if (pressType == Buttons::PressType::SHORT) {
		if(button != Button::MODE){

			if (button == Button::UP) {
				this->favouriteIndex += 1;
			} else if (button == Button::DOWN) {
				this->favouriteIndex -= 1;
			}

			if (this->favouriteIndex == 255)
				this->favouriteIndex = 7;
			else if (this->favouriteIndex >= 8)
				this->favouriteIndex = 0;

			this->setChannel();
		}
    }
}

void FavouriteScreenStateHandler::setChannel() {
	EepromSettings.favouriteChannels[this->favouriteIndex] = EepromSettings.favouriteChannels[this->favouriteIndex] % CHANNELS_SIZE;
    Receiver::setChannel(EepromSettings.favouriteChannels[this->favouriteIndex], true);
}

void FavouriteScreenStateHandler::onFSPinsChange(uint8_t state){
	this->favouriteIndex = state;
	Receiver::setChannel(EepromSettings.favouriteChannels[this->favouriteIndex]);
}
