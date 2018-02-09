#include "state_settings.h"

#include "state.h"
#include "buttons.h"
#include "ui_menu.h"
#include "ui.h"
#include "settings_eeprom.h"
#include "receiver.h"

static void diveristyModeMenuHandler(Ui::SettingsMenuItem* item);
static void rssiCalibrationMenuHandler(Ui::SettingsMenuItem* item);
static void callsignMenuHandler(Ui::SettingsMenuItem* item);
static void exitMenuHandler(Ui::SettingsMenuItem* item);
static void favouritesSettingsMenuHandler(Ui::SettingsMenuItem* item);
#ifdef USE_BUZZER
static void buzzerSettingMenuHandler(Ui::SettingsMenuItem* item);
#endif

void StateMachine::SettingsStateHandler::onEnter() {
	this->menu.reset();
	this->menu.addItem("RSSI Calibration", rssiCalibrationMenuHandler);
	this->menu.addItem("Favorite channels", favouritesSettingsMenuHandler);

#ifdef CALLSIGN
	this->menu.addItem("Callsign", callsignMenuHandler);
#endif

	const char* divValue = (EepromSettings.diversityMode == Receiver::DiversityMode::AUTO ? "AUTO" : (EepromSettings.diversityMode == Receiver::DiversityMode::FORCE_A ? "A" : "B"));
	this->menu.addItem("Diversity Mode", diveristyModeMenuHandler, divValue);

#ifdef USE_BUZZER
	const char* buzzerValue = (EepromSettings.beepEnabled ? "ON" : "OFF");
	this->menu.addItem("Buzzer", buzzerSettingMenuHandler, buzzerValue);
#endif

	this->menu.addItem("Exit", exitMenuHandler);
}

void StateMachine::SettingsStateHandler::onExit() {

}

void StateMachine::SettingsStateHandler::onUpdate() {

}

void StateMachine::SettingsStateHandler::onButtonChange(
    Button button,
    Buttons::PressType pressType
) {
    if (pressType != Buttons::PressType::SHORT)
        return;

    switch (button) {
        case Button::UP:
            this->menu.selectPreviousItem();
            Ui::needUpdate();
            break;

        case Button::DOWN:
            this->menu.selectNextItem();
            Ui::needUpdate();
            break;

        case Button::MODE:
            this->menu.activateItem();
            Ui::needUpdate();
            break;
    }
}

static void diveristyModeMenuHandler(Ui::SettingsMenuItem* item) {
//	StateMachine::switchState(StateMachine::State::SETTINGS_DIVERSITY_MODE);
	switch(EepromSettings.diversityMode){
		case Receiver::DiversityMode::FORCE_A:
			Receiver::setDiversityMode(Receiver::DiversityMode::FORCE_B);
			break;
		case Receiver::DiversityMode::FORCE_B:
			Receiver::setDiversityMode(Receiver::DiversityMode::AUTO);
			break;
		case Receiver::DiversityMode::AUTO:
			Receiver::setDiversityMode(Receiver::DiversityMode::FORCE_A);
			break;
	}
	EepromSettings.markDirty();
	const char* divValue = (EepromSettings.diversityMode == Receiver::DiversityMode::AUTO ?
			"AUTO" : (EepromSettings.diversityMode == Receiver::DiversityMode::FORCE_A ?
					"A" : "B"));
	item->value = divValue;
}

static void rssiCalibrationMenuHandler(Ui::SettingsMenuItem* item){
	StateMachine::switchState(StateMachine::State::SETTINGS_RSSI);
}

static void callsignMenuHandler(Ui::SettingsMenuItem* item){
	StateMachine::switchState(StateMachine::State::SETTINGS_CALLSIGN);
}

static void exitMenuHandler(Ui::SettingsMenuItem* item){
	StateMachine::switchState(StateMachine::State::MENU);
}

#ifdef USE_BUZZER
static void buzzerSettingMenuHandler(Ui::SettingsMenuItem* item){
	if(EepromSettings.beepEnabled)
		EepromSettings.beepEnabled = 0;
	else
		EepromSettings.beepEnabled = 1;

	EepromSettings.markDirty();
	const char* buzzerValue = (EepromSettings.beepEnabled ? "ON" : "OFF");
	item->value = buzzerValue;
}
#endif

static void favouritesSettingsMenuHandler(Ui::SettingsMenuItem* item){
	StateMachine::switchState(StateMachine::State::SETTINGS_FAVOURITES);
}
