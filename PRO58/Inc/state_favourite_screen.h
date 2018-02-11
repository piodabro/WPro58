#ifndef STATE_FAVOURITE_SCREEN_H
#define STATE_FAVOURITE_SCREEN_H

#include "state_graph_base_ui.h"

namespace StateMachine {
    class FavouriteScreenStateHandler : public StateMachine::BaseGraphUiStateHandler {
        private:
            void onUpdateAuto();

            void drawScanBar();

            void setChannel();

        public:
            uint8_t favouriteIndex = 0;

            void onEnter();
            void onUpdate();
            void onExit();

            void onButtonChange(Button button, Buttons::PressType pressType);
            void onFSPinsChange(uint8_t state);
    };
}


#endif //STATE_FAVOURITE_SCREEN_H
