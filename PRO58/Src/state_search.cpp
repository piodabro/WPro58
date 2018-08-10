#include "state_search.h"

#include "settings.h"
#include "settings_internal.h"
#include "settings_eeprom.h"
#include "timer.h"
#include "receiver.h"
#include "channels.h"
#include "buttons.h"
#include "ui.h"
#include "OSD.h"

using StateMachine::SearchStateHandler;


const unsigned char autoIcon[] = {
    0x00, 0x00, 0x1E, 0x00, 0x3F, 0x00, 0x73, 0x80, 0x61, 0x98, 0x7F, 0x84, 0x7F, 0x82, 0x61, 0x82,
    0x61, 0x80, 0x61, 0x80, 0x61, 0xA2, 0x08, 0x36, 0x08, 0x2A, 0x04, 0x22, 0x03, 0x22, 0x00, 0x00
};

const unsigned char manualIcon[] = {
    0x00, 0x00, 0x60, 0xC0, 0x71, 0xC0, 0x7B, 0xC0, 0x7F, 0xD8, 0x7F, 0xC4, 0x6E, 0xC2, 0x64, 0xC2,
    0x60, 0xC0, 0x60, 0xC0, 0x60, 0xC0, 0x08, 0x0C, 0x08, 0x12, 0x04, 0x1E, 0x03, 0x12, 0x00, 0x00
};

static const unsigned char* menuModeIcon(void* state) {
    SearchStateHandler* search = static_cast<SearchStateHandler*>(state);
    return search->manual ? manualIcon : autoIcon;
}

static void menuModeHandler(void* state) {
    SearchStateHandler* search = static_cast<SearchStateHandler*>(state);
    search->manual = !search->manual;

    EepromSettings.searchManual = search->manual;
    EepromSettings.markDirty();
}

static Timer channelSeekTimer = Timer(200);


void SearchStateHandler::onEnter() {
    menu.addItem(menuModeIcon, menuModeHandler);

    this->manual = EepromSettings.searchManual;
	this->orderedChanelIndex = EepromSettings.startChannel;
#ifdef USE_OSD	
	OSD::enableLCD(false);
	OSD::setSyncMode(OSD::syncModes::external);
#endif
	Receiver::setChannel(orderedChanelIndex);
}

void SearchStateHandler::onExit(){
#ifdef USE_OSD
    OSD::clear();
#endif    
}

void SearchStateHandler::onUpdate() {
    if (!manual) {
        onUpdateAuto();
    }

    Ui::needUpdate();
}

void SearchStateHandler::onUpdateAuto() {
	if(!Receiver::isRssiStable())
		return;

	if (scanning) {
		bool threshold =
			EepromSettings.diversityMode == Receiver::DiversityMode::AUTO ?
					(Receiver::rssiA >= RSSI_SEEK_TRESHOLD || Receiver::rssiB >= RSSI_SEEK_TRESHOLD) :
					(EepromSettings.diversityMode == Receiver::DiversityMode::FORCE_A ?
							Receiver::rssiA >= RSSI_SEEK_TRESHOLD : Receiver::rssiB >= RSSI_SEEK_TRESHOLD);
		if(!forceNext && threshold){
			this->setAndSaveChannel();
			scanning = false;
			forceNext = false;
			return;
		} else {
			orderedChanelIndex += static_cast<int8_t>(direction);
			if (orderedChanelIndex == 255)
				orderedChanelIndex = CHANNELS_SIZE - 1;
			else if (orderedChanelIndex >= CHANNELS_SIZE)
				orderedChanelIndex = 0;

			Receiver::setChannel(orderedChanelIndex);

			forceNext = false;
		}
	}

}

void SearchStateHandler::onButtonChange(
    Button button,
    Buttons::PressType pressType
) {
    if (this->menu.handleButtons(button, pressType))
        return;

    if (!this->manual) {
        if (pressType != Buttons::PressType::SHORT)
        {
            return;
        }

        if(button == Button::MODE){
			if(scanning){
				orderedChanelIndex = EepromSettings.startChannel;
				Receiver::setChannel(orderedChanelIndex);
				scanning = false;
				forceNext = false;
			}
        } else {
			scanning = true;
			forceNext = true;
			direction = button == Button::UP ? ScanDirection::UP : ScanDirection::DOWN;
        }
    } else {
        if (
            pressType != Buttons::PressType::SHORT &&
            pressType != Buttons::PressType::HOLDING
        ) {
            return;
        }

        if(!channelSeekTimer.hasTicked() && pressType == Buttons::PressType::HOLDING){
        	return;
        }

		if (button == Button::UP) {
			orderedChanelIndex += 1;
		} else if (button == Button::DOWN) {
			orderedChanelIndex -= 1;
		}

		if (orderedChanelIndex == 255)
			orderedChanelIndex = CHANNELS_SIZE - 1;
		else if (orderedChanelIndex >= CHANNELS_SIZE)
			orderedChanelIndex = 0;

		this->setAndSaveChannel();
		channelSeekTimer.reset();
    }
}

void SearchStateHandler::setAndSaveChannel() {
    Receiver::setChannel(orderedChanelIndex, true);
}

void SearchStateHandler::onFSPinsChange(uint8_t state){
	uint8_t bandOffset = Receiver::activeChannel - (Receiver::activeChannel % 8);
	orderedChanelIndex = bandOffset + state;
	this->setAndSaveChannel();
}
