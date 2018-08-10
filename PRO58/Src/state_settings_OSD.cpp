#include "state_settings_OSD.h"

#include "state.h"
#include "buttons.h"
#include "ui_menu.h"
#include "ui.h"
#include "settings_eeprom.h"
#include "receiver.h"
#include "gpio.h"

#ifdef USE_OSD

static void OSD1MenuHandler(Ui::SettingsMenuItem* item);
static void OSD2MenuHandler(Ui::SettingsMenuItem* item);
static void OSD3MenuHandler(Ui::SettingsMenuItem* item);
static void OSD4MenuHandler(Ui::SettingsMenuItem* item);
static void OSD5MenuHandler(Ui::SettingsMenuItem* item);
static void exitMenuHandler(Ui::SettingsMenuItem* item);

void StateMachine::SettingsOSDStateHandler::onEnter() {
	this->menu.reset();

	const char* OSD1Value = (EepromSettings.OSDEnabled ? "ON" : "OFF");
	this->menu.addItem("OSD", OSD1MenuHandler, OSD1Value);

	const char* OSD2Value = (EepromSettings.OSDShowRssi ? "ON" : "OFF");
	this->menu.addItem("Show RSSI", OSD2MenuHandler, OSD2Value);

    const char* OSD3Value = (EepromSettings.OSDShowChannel ? "ON" : "OFF");
    this->menu.addItem("Show channel", OSD3MenuHandler, OSD3Value);

    const char* OSD4Value = (EepromSettings.OSDShowFrequency ? "ON" : "OFF");
    this->menu.addItem("Show frequency", OSD4MenuHandler, OSD4Value);

    const char* OSD5Value = (EepromSettings.OSDDefaultMode == OSD::videoModes::PAL ? "PAL" : "NTSC");
    this->menu.addItem("Default Mode", OSD5MenuHandler, OSD5Value);

	this->menu.addItem("Exit", exitMenuHandler);
}

void StateMachine::SettingsOSDStateHandler::onExit() {

}

void StateMachine::SettingsOSDStateHandler::onUpdate() {

}

void StateMachine::SettingsOSDStateHandler::onButtonChange(
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

void StateMachine::SettingsOSDStateHandler::onInitialDraw() {
    Ui::clear();
    this->menu.drawMenu();
    Ui::needDisplay();
}

void StateMachine::SettingsOSDStateHandler::onUpdateDraw() {
    this->menu.drawMenu();
    Ui::needDisplay();
}

static void OSD1MenuHandler(Ui::SettingsMenuItem* item){
	if(EepromSettings.OSDEnabled)
		EepromSettings.OSDEnabled = 0;
	else
		EepromSettings.OSDEnabled = 1;

	EepromSettings.markDirty();
	const char* buzzerValue = (EepromSettings.OSDEnabled ? "ON" : "OFF");
	item->value = buzzerValue;
}

static void OSD2MenuHandler(Ui::SettingsMenuItem* item){
    if(EepromSettings.OSDShowRssi)
      EepromSettings.OSDShowRssi = 0;
    else
      EepromSettings.OSDShowRssi = 1;

    EepromSettings.markDirty();
    const char* buzzerValue = (EepromSettings.OSDShowRssi ? "ON" : "OFF");
    item->value = buzzerValue;
}

static void OSD3MenuHandler(Ui::SettingsMenuItem* item){
    if(EepromSettings.OSDShowChannel)
      EepromSettings.OSDShowChannel = 0;
    else
      EepromSettings.OSDShowChannel = 1;

    EepromSettings.markDirty();
    const char* buzzerValue = (EepromSettings.OSDShowChannel ? "ON" : "OFF");
    item->value = buzzerValue;
}

static void OSD4MenuHandler(Ui::SettingsMenuItem* item){
    if(EepromSettings.OSDShowFrequency)
      EepromSettings.OSDShowFrequency = 0;
    else
      EepromSettings.OSDShowFrequency = 1;

    EepromSettings.markDirty();
    const char* buzzerValue = (EepromSettings.OSDShowFrequency ? "ON" : "OFF");
    item->value = buzzerValue;
}

static void OSD5MenuHandler(Ui::SettingsMenuItem* item){
    if(EepromSettings.OSDDefaultMode == OSD::videoModes::PAL)
        EepromSettings.OSDDefaultMode = OSD::videoModes::NTSC;
    else
        EepromSettings.OSDDefaultMode = OSD::videoModes::PAL;

    EepromSettings.markDirty();
    const char* buzzerValue = (EepromSettings.OSDDefaultMode == OSD::videoModes::PAL ? "PAL" : "NTSC");
    item->value = buzzerValue;
}

static void exitMenuHandler(Ui::SettingsMenuItem* item){
    StateMachine::switchState(StateMachine::State::SETTINGS);
}
#endif
