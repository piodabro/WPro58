#include "state.h"
#include "state_settings.h"
#include "ui.h"
#include "ui_menu.h"
#include <cstring>
//#include "pstr_helper.h"

#define ITEM_HEIGHT 8
#define TRIANGLE_SIZE 4
#define TRIANGLE_MARGIN 2

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

    for(uint8_t i = 0; i < MAX_ITEMS_PER_SCREEN; ++i){
    	const Ui::SettingsMenuItem* item = this->menu.getItem(i + this->menu.getMenuOffset());
    	const uint8_t charLen = strlen((item->text));

    	if(i + this->menu.getMenuOffset() == currentMenuIndex){
    		Ui::display.fillRect(0,i*ITEM_HEIGHT,SCREEN_WIDTH-6,ITEM_HEIGHT,WHITE);
    		Ui::display.setTextColor(BLACK);
    	} else {
    		Ui::display.setTextColor(WHITE);
    	}

    	Ui::display.setCursor(2, i * ITEM_HEIGHT + ((ITEM_HEIGHT - CHAR_HEIGHT) / 2));
    	Ui::display.print(item->text);

    	if(item->value != nullptr){
    		Ui::display.setCursor(SCREEN_WIDTH - (CHAR_WIDTH * strlen(item->value)) - 10, i * ITEM_HEIGHT + ((ITEM_HEIGHT - CHAR_HEIGHT) / 2));
    		Ui::display.print(item->value);
    	}
    }

    drawTriangles();
}

void StateMachine::SettingsStateHandler::drawTriangles() {
	if(this->menu.getSelectedItemIndex() < this->menu.getItemCount()-1){
		Ui::display.fillTriangle(
			SCREEN_WIDTH - 1 - (TRIANGLE_SIZE),
			SCREEN_HEIGHT_MID + TRIANGLE_MARGIN,

			SCREEN_WIDTH - 1 - (TRIANGLE_SIZE / 2),
			SCREEN_HEIGHT_MID + TRIANGLE_MARGIN + (TRIANGLE_SIZE),

			SCREEN_WIDTH - 1,
			SCREEN_HEIGHT_MID + TRIANGLE_MARGIN,

			WHITE
		);
	}

	if(this->menu.getSelectedItemIndex() > 0){
		Ui::display.fillTriangle(
			SCREEN_WIDTH - 1 - (TRIANGLE_SIZE),
			SCREEN_HEIGHT_MID - TRIANGLE_MARGIN,

			SCREEN_WIDTH - 1 - (TRIANGLE_SIZE / 2),
			SCREEN_HEIGHT_MID - TRIANGLE_MARGIN - (TRIANGLE_SIZE),

			SCREEN_WIDTH - 1,
			SCREEN_HEIGHT_MID - TRIANGLE_MARGIN,

			WHITE
		);
	}
}
