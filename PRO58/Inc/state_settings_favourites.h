#ifndef STATE_SETTINGS_FAVOURITES_H
#define STATE_SETTINGS_FAVOURITES_H


#include "state.h"


namespace StateMachine {
    class SettingsFavouritesStateHandler : public StateMachine::StateHandler {
        private:
            uint8_t favIndex = 0;

        public:
            void onEnter();
            void onUpdate();
            void onExit();

            void onInitialDraw();
            void onUpdateDraw();

            void onButtonChange(Button button, Buttons::PressType pressType);

            void drawChannelText(uint8_t channel, uint8_t x, uint8_t y, uint8_t color);
    };
}


#endif //STATE_SETTINGS_FAVOURITES_H
