#ifndef STATE_SETTINGS_H
#define STATE_SETTINGS_H


#include "state.h"
#include "ui_menu.h"


namespace StateMachine {
    class SettingsStateHandler : public StateMachine::StateHandler {
        public:
            void onEnter();
            void onExit();
            void onUpdate();

            void onInitialDraw();
            void onUpdateDraw();

            void onButtonChange(Button button, Buttons::PressType pressType);

        private:
			enum class InternalState : uint8_t {
							RSSI_CALIBRATION,
							DIVERSITY_MODE,
							CALLSIGN_MODE
						};
			InternalState internalState = InternalState::RSSI_CALIBRATION;
			Ui::MenuHelper menu;
			void drawMenuEntry();
    };
}

#endif
