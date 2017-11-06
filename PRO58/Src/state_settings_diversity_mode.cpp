#include <stdint.h>

#include "state_settings_diversity_mode.h"

#include "receiver.h"
#include "channels.h"

#include "settings.h"
#include "settings_internal.h"
#include "settings_eeprom.h"
#include "buttons.h"

#include "ui.h"

void StateMachine::SettingsDiversityModeStateHandler::onEnter() {

}

void StateMachine::SettingsDiversityModeStateHandler::onUpdate() {

}

void StateMachine::SettingsDiversityModeStateHandler::onButtonChange(
    Button button,
    Buttons::PressType pressType
) {
	Receiver::DiversityMode mode = EepromSettings.diversityMode;

    if(pressType == Buttons::PressType::SHORT){
    	switch(button){
    		case Button::DOWN:
    			if(mode == Receiver::DiversityMode::FORCE_B)
    				Receiver::setDiversityMode(Receiver::DiversityMode::FORCE_A);
    			else if(mode == Receiver::DiversityMode::FORCE_A)
					Receiver::setDiversityMode(Receiver::DiversityMode::AUTO);
    			else if(mode == Receiver::DiversityMode::AUTO)
					Receiver::setDiversityMode(Receiver::DiversityMode::FORCE_B);
    			break;
    		case Button::UP:
    			if(mode == Receiver::DiversityMode::FORCE_B)
					Receiver::setDiversityMode(Receiver::DiversityMode::AUTO);
				else if(mode == Receiver::DiversityMode::AUTO)
					Receiver::setDiversityMode(Receiver::DiversityMode::FORCE_A);
				else if(mode == Receiver::DiversityMode::FORCE_A)
					Receiver::setDiversityMode(Receiver::DiversityMode::FORCE_B);
				break;
    		case Button::MODE:
    			EepromSettings.save();
    			StateMachine::switchState(StateMachine::State::SETTINGS);
    			break;
    	}
    }

    Ui::needUpdate();
}


void StateMachine::SettingsDiversityModeStateHandler::onInitialDraw() {
    Ui::needUpdate(); // Lazy. :(
}

void StateMachine::SettingsDiversityModeStateHandler::onUpdateDraw() {
    Ui::clear();

    Ui::display.setTextSize(1);
	Ui::display.setCursor(5, SCREEN_HEIGHT/2 - CHAR_HEIGHT * 2);
	Ui::display.print(("Diversity mode:"));
	Ui::display.setCursor(30, SCREEN_HEIGHT/2 - CHAR_HEIGHT + 2);

	switch (EepromSettings.diversityMode){
		case Receiver::DiversityMode::AUTO:
			Ui::display.print(("AUTO"));
			break;
		case Receiver::DiversityMode::FORCE_A:
			Ui::display.print(("FORCE A"));
			break;
		case Receiver::DiversityMode::FORCE_B:
			Ui::display.print(("FORCE B"));
			break;
	}

	Ui::display.setCursor(0, SCREEN_HEIGHT - CHAR_HEIGHT - 1);
	Ui::display.print(("Press MODE to save."));

    Ui::needDisplay();
}
