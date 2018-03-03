#ifndef UI_SETTINGS_MENU_H
#define UI_SETTINGS_MENU_H

#include "ui.h"


#define SETTINGS_MENU_ITEMS_MAX 16
#define MAX_ITEMS_PER_SCREEN 8


namespace Ui {
	struct SettingsMenuItem;

    typedef void(*SettingsMenuHandler)(SettingsMenuItem* item);

	struct SettingsMenuItem {
		const char* text = nullptr;
		const char* value = nullptr;
		Ui::SettingsMenuHandler handler = nullptr;
	};

    class SettingsMenuHelper {
        public:
            void reset();
            void addItem(
                const char* text,
                const SettingsMenuHandler handler,
				const char* value = nullptr
            );

            SettingsMenuItem* getItem(uint8_t index);

            void selectNextItem();
            void selectPreviousItem();
            void activateItem();
            uint8_t getSelectedItemIndex();
            void setSelectedItemIndex(uint8_t index);
            uint8_t getItemCount();
            uint8_t getMenuOffset();
            void setMenuOffset(uint8_t offset);

        private:
            Ui::SettingsMenuItem menuItems[SETTINGS_MENU_ITEMS_MAX];

            uint8_t activeItems = 0;
            uint8_t selectedItem = 0;
            uint8_t menuOffset = 0;
    };
}


#endif
