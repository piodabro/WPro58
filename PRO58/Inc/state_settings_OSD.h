#ifndef STATE_SETTINGS_OSD_H
#define STATE_SETTINGS_OSD_H


#include "state.h"
#include "ui_settings_menu.h"

namespace StateMachine {
    class SettingsOSDStateHandler : public StateMachine::StateHandler {
        public:
            void onEnter();
            void onExit();
            void onUpdate();

            void onInitialDraw();
            void onUpdateDraw();

            void onButtonChange(Button button, Buttons::PressType pressType);

        private:

			Ui::SettingsMenuHelper menu;
    };
}

#endif
