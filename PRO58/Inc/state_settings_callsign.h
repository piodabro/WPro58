#ifndef STATE_SETTINGS_CALLSIGN_H
#define STATE_SETTINGS_CALLSIGN_H


#include "state.h"


namespace StateMachine {
    class SettingsCallsignStateHandler : public StateMachine::StateHandler {
        private:
    			int cursor;
    			char callsign[9];
        public:
            void onEnter();
            void onUpdate();

            void onInitialDraw();
            void onUpdateDraw();

            void onButtonChange(Button button, Buttons::PressType pressType);
    };
}


#endif
