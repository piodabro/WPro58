#ifndef STATE_SEARCH_H
#define STATE_SEARCH_H


#include "state_graph_base_ui.h"
#include "ui_state_menu.h"


#define PEAK_LOOKAHEAD 4


namespace StateMachine {
    class SearchStateHandler : public StateMachine::BaseGraphUiStateHandler {
        private:
            enum class ScanDirection : int8_t {
                UP = 1,
                DOWN = -1
            };

            bool scanning = false;
            ScanDirection direction = ScanDirection::UP;
            bool forceNext = false;

            Ui::StateMenuHelper menu = Ui::StateMenuHelper(this);

            void onUpdateAuto();

            void drawScanBar();
            void drawMenu();

            void setAndSaveChannel();

        public:
            bool manual = false;
            uint8_t orderedChanelIndex = 0;

            void onEnter();
            void onUpdate();
            void onExit();

            void onButtonChange(Button button, Buttons::PressType pressType);
            void onFSPinsChange(uint8_t state);
    };
}


#endif
