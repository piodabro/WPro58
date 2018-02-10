#ifndef UI_SETTINGS_MENU_H
#define UI_SETTINGS_MENU_H

#include "ui.h"


#define SETTINGS_MENU_ITEMS_MAX 8


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

        private:
            Ui::SettingsMenuItem menuItems[SETTINGS_MENU_ITEMS_MAX];

            int activeItems = 0;
            int selectedItem = 0;
    };
}


#endif
