#include <stdint.h>
#include <string.h>

#include "state_settings_callsign.h"

#include "receiver.h"
#include "channels.h"

#include "settings.h"
#include "settings_internal.h"
#include "settings_eeprom.h"
#include "buttons.h"
#include "timer.h"

#include "ui.h"

static Timer holdTimer = Timer(100);

void StateMachine::SettingsCallsignStateHandler::onEnter() {
	EepromSettings.getCallSign(callsign);
	cursor = -1;
}

void StateMachine::SettingsCallsignStateHandler::onUpdate() {

}

void StateMachine::SettingsCallsignStateHandler::onButtonChange(
    Button button,
    Buttons::PressType pressType
) {
	if (button == Button::MODE){
		if(pressType == Buttons::PressType::SHORT){
			cursor++;
			if (cursor==CALLSIGN_LEN){
				cursor = -1;
			}
			else{
				cursor = cursor % CALLSIGN_LEN;
			}
			Ui::needUpdate();
	    	} else {
			if (EepromSettings.cmpCallSign(callsign) && strlen(callsign)>0 && cursor!=-1){
				EepromSettings.setCallSign(callsign);
				EepromSettings.save();
			}
			StateMachine::switchState(StateMachine::State::SETTINGS);
		}
	} else {
		if ((pressType == Buttons::PressType::SHORT) ||(pressType == Buttons::PressType::HOLDING && holdTimer.hasTicked())){
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
			if (pressType == Buttons::PressType::HOLDING){
				holdTimer.reset();
			}
			Ui::needUpdate();
		}
	}

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
	int textStartX = Ui::display.width()/2 - TEXTSIZE * 6 * CALLSIGN_LEN / 2;
	int textStartY = Ui::display.height()/2-TEXTSIZE * 6;
	Ui::display.setCursor(textStartX, textStartY);
	Ui::display.print(callsign);

	if (cursor !=-1){
		//Ui::clearRect(textStartX+TEXTSIZE*6*cursor-2, textStartY-2,TEXTSIZE*6+4,
		//		TEXTSIZE*6+4);

		Ui::display.fillRect(textStartX+TEXTSIZE*6*cursor-2, textStartY-2,TEXTSIZE*CHAR_WIDTH+4,
				TEXTSIZE*CHAR_HEIGHT+6, WHITE);
		//Ui::display.setTextColor(BLACK);
		Ui::display.drawChar(textStartX+TEXTSIZE*6*cursor, textStartY, callsign[cursor], BLACK, BLACK, TEXTSIZE);
		//Ui::display.setCursor(textStartX+TEXTSIZE*6*cursor, textStartY);
		//Ui::display.drawChar(callsign[cursor], textStartX+TEXTSIZE*6*cursor, textStartY, TEXTSIZE);

		Ui::display.setTextSize(1);
		Ui::display.setTextColor(WHITE);
		Ui::display.setCursor(0, Ui::display.height() - CHAR_HEIGHT - 1);
		Ui::display.print(("Long MODE to save."));
	}
	else{
		Ui::display.setTextSize(1);
		Ui::display.setTextColor(WHITE);
		Ui::display.setCursor(0, Ui::display.height() - CHAR_HEIGHT - 1);
		Ui::display.print(("Long MODE to exit."));
	}
    Ui::needDisplay();
}
