#include "ui.h"
#include "ui_settings_menu.h"


void Ui::SettingsMenuHelper::reset() {
    this->activeItems = 0;
    this->selectedItem = 0;
}

void Ui::SettingsMenuHelper::addItem(
    const char* text,
    const Ui::SettingsMenuHandler handler,
	const char* value
) {
    this->menuItems[this->activeItems].text = text;
    this->menuItems[this->activeItems].handler = handler;
    this->menuItems[this->activeItems].value = value;
    this->activeItems++;
}

//void Ui::SettingsMenuHelper::addValuedItem(
//    const char* text,
//	const char* value,
//    const Ui::SettingsMenuHandler handler
//) {
//    this->menuItems[this->activeItems].text = text;
//    this->menuItems[this->activeItems].value = value;
//    this->menuItems[this->activeItems].handler = handler;
//
//    this->activeItems++;
//}

void Ui::SettingsMenuHelper::selectNextItem() {
    if (++this->selectedItem >= this->activeItems)
        this->selectedItem = 0;
}

void Ui::SettingsMenuHelper::selectPreviousItem() {
    if (--this->selectedItem < 0)
        this->selectedItem = this->activeItems - 1;
}

uint8_t Ui::SettingsMenuHelper::getCurrentItemIndex(){
	return this->selectedItem;
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
