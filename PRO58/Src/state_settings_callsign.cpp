#include <stdint.h>

#include "state_settings_callsign.h"

#include "receiver.h"
#include "channels.h"

#include "settings.h"
#include "settings_internal.h"
#include "settings_eeprom.h"
#include "buttons.h"

#include "ui.h"

void StateMachine::SettingsCallsignStateHandler::onEnter() {
	EepromSettings.getCallSign(callsign);
	cursor = 0;
}

void StateMachine::SettingsCallsignStateHandler::onUpdate() {

}

void StateMachine::SettingsCallsignStateHandler::onButtonChange(
    Button button,
    Buttons::PressType pressType
) {
	switch(button){
		case Button::DOWN:
			if (callsign[cursor]>32){
				callsign[cursor]--;
			}
			else {
				callsign[cursor]=0;
			}
			break;
		case Button::UP:
		    if (callsign[cursor]==0){
		    		callsign[cursor]=65;
		    }
			else if (callsign[cursor]<126){
				callsign[cursor]++;
			}
			else {
				callsign[cursor]=0;
			}
			break;
	}

    if(pressType == Buttons::PressType::SHORT){
		switch(button){
			case Button::MODE:
				cursor = (cursor+1)%8;
				break;
		}
    	} else {
    		switch(button){
			case Button::MODE:
				if (EepromSettings.cmpCallSign(callsign)){
					EepromSettings.setCallSign(callsign);
					EepromSettings.save();
				}
				StateMachine::switchState(StateMachine::State::SETTINGS);
				break;
    		}
    	}
    Ui::needUpdate();
}


void StateMachine::SettingsCallsignStateHandler::onInitialDraw() {
    Ui::needUpdate(); // Lazy. :(
}

void StateMachine::SettingsCallsignStateHandler::onUpdateDraw() {
	const int TEXTSIZE = 2;
    Ui::clear();



    Ui::display.setTextSize(1);
    Ui::display.setTextColor(WHITE);
	Ui::display.setCursor(5, 2);
	Ui::display.print(("Callsign:"));

	Ui::display.setTextColor(WHITE);
	Ui::display.setTextSize(TEXTSIZE);
	int textStart = Ui::display.width()/2 - TEXTSIZE * 6 * 8 / 2;
	Ui::display.setCursor(textStart,	Ui::display.height()/2-TEXTSIZE * 6);
	Ui::display.print(callsign);
	Ui::display.setTextColor(BLACK, WHITE);
	Ui::display.setCursor(textStart+TEXTSIZE*6*cursor, Ui::display.height()/2-TEXTSIZE * 6);
	Ui::display.print(callsign[cursor]);

	Ui::display.setTextSize(1);
	Ui::display.setTextColor(WHITE);
	Ui::display.setCursor(0, Ui::display.height() - CHAR_HEIGHT - 1);
	Ui::display.print(("Long MODE to save."));

    Ui::needDisplay();
}
