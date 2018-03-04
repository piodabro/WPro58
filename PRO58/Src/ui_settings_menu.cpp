#include "ui.h"
#include "ui_settings_menu.h"


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
