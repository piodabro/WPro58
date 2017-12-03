#include <string.h>
#include "state_screensaver.h"

#include "receiver.h"
#include "channels.h"
#include "buttons.h"
#include "state.h"
#include "ui.h"
#include "settings_eeprom.h"

#ifdef PUBLIC_RELEASE
#include "logo.h"
#else
#include "piodabro_logo.h"
#endif

void StateMachine::ScreensaverStateHandler::onEnter() {
    showLogo = false;
}

void StateMachine::ScreensaverStateHandler::onUpdate() {
    if (this->displaySwapTimer.hasTicked()) {
        this->displaySwapTimer.reset();
        showLogo = !showLogo;

        Ui::needUpdate();
    }
}


void StateMachine::ScreensaverStateHandler::onButtonChange(
    Button button,
    Buttons::PressType pressType
) {
    StateMachine::switchState(StateMachine::lastState);
}


void StateMachine::ScreensaverStateHandler::onInitialDraw() {
    Ui::clear();

#ifdef CALLSIGN
    if (showLogo) {
		char sign[CALLSIGN_LEN+1] = "";
		EepromSettings.getCallSign(sign);
    		Ui::display.setTextColor(WHITE);

    	    Ui::display.setTextSize(2);
		Ui::display.setCursor(
			SCREEN_WIDTH_MID - (2*(CHAR_WIDTH + 1)* strlen(sign))/2,
			SCREEN_HEIGHT_MID - CHAR_HEIGHT );
		Ui::display.print(sign);
#else
	if (showLogo) {
		Ui::display.drawBitmap(
			0,
			0,
			logo,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			WHITE
		);
#endif
    } else {
        Ui::display.setTextColor(WHITE);

        Ui::display.setTextSize(6);
        Ui::display.setCursor(
            SCREEN_WIDTH_MID - ((CHAR_WIDTH) * 6) / 2 * 2 - 3,
            2);

        Ui::display.print(Channels::getName(Receiver::activeChannel));

        Ui::display.setTextSize(2);
        Ui::display.setCursor(
            SCREEN_WIDTH_MID - ((CHAR_WIDTH + 1) * 2) / 2 * 4 - 1,
            SCREEN_HEIGHT - CHAR_HEIGHT * 2 - 2);
        Ui::display.print(Channels::getFrequency(Receiver::activeChannel));
    }

    Ui::needDisplay();
}

void StateMachine::ScreensaverStateHandler::onUpdateDraw() {
    this->onInitialDraw();
}

void StateMachine::ScreensaverStateHandler::onFSPinsChange(uint8_t state){
	uint8_t bandOffset = Receiver::activeChannel - (Receiver::activeChannel % 8);
	uint8_t orderedChanelIndex = bandOffset + state;
	Receiver::setChannel(orderedChanelIndex);
	showLogo = false;
	this->displaySwapTimer.reset();
	this->onInitialDraw();
}
