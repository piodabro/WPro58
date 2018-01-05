#ifndef STATE_FAVOURITE_SCREEN_H
#define STATE_FAVOURITE_SCREEN_H


#include "state.h"

namespace StateMachine {
    class FavouriteScreenStateHandler : public StateMachine::StateHandler {
        private:
            void onUpdateAuto();

            void drawBorders();
            void drawChannelText();
            void drawFrequencyText();
            void drawScanBar();
            void drawRssiGraph();

            void setChannel();

        public:
            uint8_t favouriteIndex = 0;

            void onEnter();
            void onUpdate();
            void onExit();

            void onInitialDraw();
            void onUpdateDraw();

            void onButtonChange(Button button, Buttons::PressType pressType);
            void onFSPinsChange(uint8_t state);
    };
}


#endif //STATE_FAVOURITE_SCREEN_H
