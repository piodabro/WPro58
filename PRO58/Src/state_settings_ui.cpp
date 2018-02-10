#include "state.h"
#include "state_settings.h"
#include "ui.h"
#include "ui_menu.h"
#include <cstring>
//#include "pstr_helper.h"

#define ITEM_HEIGHT 8

void StateMachine::SettingsStateHandler::onInitialDraw() {
    Ui::clear();

    drawMenu();

    Ui::needDisplay();
}

void StateMachine::SettingsStateHandler::onUpdateDraw() {
    drawMenu();
    Ui::needDisplay();
}


void StateMachine::SettingsStateHandler::drawMenu() {
	const uint8_t currentMenuIndex = this->menu.getSelectedItemIndex();

	Ui::clear();
	Ui::display.setTextSize(1);

    for(uint8_t i=0;i<SETTINGS_MENU_ITEMS_MAX;i++){
    	const Ui::SettingsMenuItem* item = this->menu.getItem(i);
    	const uint8_t charLen = strlen((item->text));

    	if(i == currentMenuIndex){
    		Ui::display.fillRect(0,i*ITEM_HEIGHT,SCREEN_WIDTH,ITEM_HEIGHT,WHITE);
    		Ui::display.setTextColor(BLACK);
    	} else {
    		Ui::display.setTextColor(WHITE);
    	}

    	Ui::display.setCursor(2, i * ITEM_HEIGHT + ((ITEM_HEIGHT - CHAR_HEIGHT) / 2));
    	Ui::display.print(item->text);

    	if(item->value != nullptr){
    		Ui::display.setCursor(SCREEN_WIDTH - (CHAR_WIDTH * strlen(item->value)) - 4, i * ITEM_HEIGHT + ((ITEM_HEIGHT - CHAR_HEIGHT) / 2));
    		Ui::display.print(item->value);
    	}
    }
}
