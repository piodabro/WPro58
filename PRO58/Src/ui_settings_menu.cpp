#include "ui.h"
#include "ui_settings_menu.h"
#include <cstring>

#define ITEM_HEIGHT 8
#define TRIANGLE_SIZE 4
#define TRIANGLE_MARGIN 2

void Ui::SettingsMenuHelper::reset() {
    this->activeItems = 0;
    this->selectedItem = 0;
    this->menuOffset = 0;
}

void Ui::SettingsMenuHelper::addItem(
    const char* text,
    const Ui::SettingsMenuHandler handler,
	const char* value
) {
	if(this->activeItems < SETTINGS_MENU_ITEMS_MAX){
		this->menuItems[this->activeItems].text = text;
		this->menuItems[this->activeItems].handler = handler;
		this->menuItems[this->activeItems].value = value;
		this->activeItems++;
	}
}

void Ui::SettingsMenuHelper::selectNextItem() {
    if (++this->selectedItem >= this->activeItems)
        this->selectedItem = this->activeItems - 1;

    if(this->selectedItem - this->menuOffset > MAX_ITEMS_PER_SCREEN - 1){
		++menuOffset;
	}
}

void Ui::SettingsMenuHelper::selectPreviousItem() {
    if (--this->selectedItem > this->activeItems)
        this->selectedItem = 0;

    if(this->selectedItem - this->menuOffset < 0){
		--menuOffset;
	}
}

uint8_t Ui::SettingsMenuHelper::getSelectedItemIndex(){
	return this->selectedItem;
}

void Ui::SettingsMenuHelper::setSelectedItemIndex(uint8_t index){
	this->selectedItem = index;
}


uint8_t Ui::SettingsMenuHelper::getMenuOffset(){
	return this->menuOffset;
}

void Ui::SettingsMenuHelper::setMenuOffset(uint8_t offset){
	this->menuOffset = offset;
}

uint8_t Ui::SettingsMenuHelper::getItemCount(){
	return this->activeItems;
}


void Ui::SettingsMenuHelper::activateItem() {
	SettingsMenuItem *item = &this->menuItems[this->selectedItem];
    item->handler(item);
}


Ui::SettingsMenuItem* Ui::SettingsMenuHelper::getItem(uint8_t index) {
	if(index < 0 || index >= SETTINGS_MENU_ITEMS_MAX){
		return nullptr;
	}
    return &this->menuItems[index];
}

void Ui::SettingsMenuHelper::drawMenu() {
    const uint8_t currentMenuIndex = this->getSelectedItemIndex();

    Ui::clear();
    Ui::display.setTextSize(1);

    for(uint8_t i = 0; i < MAX_ITEMS_PER_SCREEN; ++i){
        const Ui::SettingsMenuItem* item = this->getItem(i + this->getMenuOffset());
        const uint8_t charLen = strlen((item->text));

        if(i + this->getMenuOffset() == currentMenuIndex){
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

void Ui::SettingsMenuHelper::drawTriangles() {
    if(this->getSelectedItemIndex() < this->getItemCount()-1){
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

    if(this->getSelectedItemIndex() > 0){
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
