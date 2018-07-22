#include "state_settings.h"

#include "state.h"
#include "buttons.h"
#include "ui_menu.h"
#include "ui.h"
#include "settings_eeprom.h"
#include "receiver.h"
#include "gpio.h"

static void diveristyModeMenuHandler(Ui::SettingsMenuItem* item);
static void rssiCalibrationMenuHandler(Ui::SettingsMenuItem* item);
static void callsignMenuHandler(Ui::SettingsMenuItem* item);
static void OSDMenuHandler(Ui::SettingsMenuItem* item);
static void exitMenuHandler(Ui::SettingsMenuItem* item);
static void favouritesSettingsMenuHandler(Ui::SettingsMenuItem* item);
#ifdef USE_BUZZER
static void buzzerSettingMenuHandler(Ui::SettingsMenuItem* item);
#endif
static void screensaverSettingMenuHandler(Ui::SettingsMenuItem* item);

static uint8_t resetCofirmation = 0;
static void resetSettingsMenuHandler(Ui::SettingsMenuItem* item);

static void flipScreenMenuHandler(Ui::SettingsMenuItem* item);
static void fsPinsModeMenuHandler(Ui::SettingsMenuItem* item);

void StateMachine::SettingsStateHandler::onEnter() {
	this->menu.reset();
	this->menu.addItem("RSSI Calibration", rssiCalibrationMenuHandler);
	this->menu.addItem("Favorite channels", favouritesSettingsMenuHandler);

#ifdef USE_CALLSIGN
	this->menu.addItem("Callsign", callsignMenuHandler);
#endif

#ifdef USE_OSD
    this->menu.addItem("OSD", OSDMenuHandler);
#endif

	const char* divValue = (EepromSettings.diversityMode == Receiver::DiversityMode::AUTO ? "AUTO" : (EepromSettings.diversityMode == Receiver::DiversityMode::FORCE_A ? "A" : "B"));
	this->menu.addItem("Diversity Mode", diveristyModeMenuHandler, divValue);

#ifdef USE_BUZZER
	const char* buzzerValue = (EepromSettings.beepEnabled ? "ON" : "OFF");
	this->menu.addItem("Buzzer", buzzerSettingMenuHandler, buzzerValue);
#endif

	const char* screensaverValue = (EepromSettings.screensaverEnabled ? "ON" : "OFF");
	this->menu.addItem("Screensaver", screensaverSettingMenuHandler, screensaverValue);

	const char* flipScreenValue = EepromSettings.screenFlip ? "FLIP" : "NORM";
	this->menu.addItem("Flip screen", flipScreenMenuHandler, flipScreenValue);

#ifdef USE_FS_PINS
	const char* FSPinsMode = (EepromSettings.FSPinsMode ? "FS" : "BUTTON");
	this->menu.addItem("FS pins mode", fsPinsModeMenuHandler, FSPinsMode);
#endif
	this->menu.addItem("Reset Settings", resetSettingsMenuHandler);
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

            if(resetCofirmation != 0){
            	this->resetConfirmation();
            }

            Ui::needUpdate();
            break;

        case Button::DOWN:
            this->menu.selectNextItem();

            if(resetCofirmation != 0){
				this->resetConfirmation();
			}

            Ui::needUpdate();
            break;

        case Button::MODE:
            this->menu.activateItem();
            Ui::needUpdate();
            break;
    }
}

void StateMachine::SettingsStateHandler::resetConfirmation(){
	uint8_t selectedItem = this->menu.getSelectedItemIndex();
	uint8_t offset = this->menu.getMenuOffset();
	resetCofirmation = 0;
	onEnter();
	this->menu.setMenuOffset(offset);
	this->menu.setSelectedItemIndex(selectedItem);
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

static void OSDMenuHandler(Ui::SettingsMenuItem* item){
    StateMachine::switchState(StateMachine::State::SETTINGS_OSD);
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

static void screensaverSettingMenuHandler(Ui::SettingsMenuItem* item){
	if(EepromSettings.screensaverEnabled)
			EepromSettings.screensaverEnabled = 0;
		else
			EepromSettings.screensaverEnabled = 1;

	EepromSettings.markDirty();
	const char* value = (EepromSettings.screensaverEnabled ? "ON" : "OFF");
	item->value = value;
}

static void flipScreenMenuHandler(Ui::SettingsMenuItem* item){
	EepromSettings.screenFlip = !EepromSettings.screenFlip;
	EepromSettings.markDirty();
	const char* flipScreenValue = EepromSettings.screenFlip ? "FLIP" : "NORM";
	item->value = flipScreenValue;
	Ui::display.setRotation(EepromSettings.screenFlip ? 2 : 0);
}

static void fsPinsModeMenuHandler(Ui::SettingsMenuItem* item){
	EepromSettings.FSPinsMode = !EepromSettings.FSPinsMode;

	const char* FSPinValue = (EepromSettings.FSPinsMode ? "FS" : "BUTTON");
	item->value = FSPinValue;

	uint32_t pull = EepromSettings.FSPinsMode ? GPIO_PULLDOWN : GPIO_PULLUP;
	GPIO_FS_Reinit(pull);

	EepromSettings.save();
}

static void resetSettingsMenuHandler(Ui::SettingsMenuItem* item){
	if(resetCofirmation == 0){
		resetCofirmation = 1;
		item->text = "Confirm: press MODE";
	}
	else if(resetCofirmation == 1){
		EepromSettings.initDefaults();
		EepromSettings.save();
		HAL_Delay(500);
		HAL_NVIC_SystemReset();
	}
}
