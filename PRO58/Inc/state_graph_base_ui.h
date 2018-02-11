#ifndef STATE_GRAPH_BASE_UI_H
#define STATE_GRAPH_BASE_UI_H


#include "state.h"

#define BORDER_GRAPH_L_X 59

#define CHANNEL_TEXT_SIZE 5
#define CHANNEL_TEXT_X 0
#define CHANENL_TEXT_Y 0
#define CHANNEL_TEXT_H (CHAR_HEIGHT * CHANNEL_TEXT_SIZE)

#define FREQUENCY_TEXT_SIZE 2
#define FREQUENCY_TEXT_X 6
#define FREQUENCY_TEXT_Y (SCREEN_HEIGHT - (CHAR_HEIGHT * 2))
#define FREQUENCY_TEXT_H (CHAR_HEIGHT * FREQUENCY_TEXT_SIZE)

#define SCANBAR_BORDER_X 0
#define SCANBAR_BORDER_Y (CHANNEL_TEXT_H + 4)
#define SCANBAR_BORDER_W (BORDER_GRAPH_L_X - 4)
#define SCANBAR_BORDER_H 7

#define SCANBAR_X (SCANBAR_BORDER_X + 2)
#define SCANBAR_Y (SCANBAR_BORDER_Y + 2)
#define SCANBAR_W (SCANBAR_BORDER_W - 4)
#define SCANBAR_H (SCANBAR_BORDER_H - 4)

#define GRAPH_SEPERATOR_Y SCREEN_HEIGHT_MID
#define GRAPH_SEPERATOR_W (SCREEN_WIDTH - BORDER_GRAPH_L_X)
#define GRAPH_SEPERATOR_STEP 3

#define GRAPH_X (BORDER_GRAPH_L_X + 2)
#define GRAPH_W (SCREEN_WIDTH - BORDER_GRAPH_L_X)
#ifdef USE_DIVERSITY
    #define GRAPH_H (GRAPH_SEPERATOR_Y - 2)
    #define GRAPH_A_Y 0
    #define GRAPH_B_Y (SCREEN_HEIGHT - GRAPH_H - 1)

    #define RX_TEXT_SIZE 1
    #define RX_TEXT_X (BORDER_GRAPH_L_X + 4)
    #define RX_TEXT_H (CHAR_HEIGHT * RX_TEXT_SIZE)
    #define RX_TEXT_A_Y ((GRAPH_A_Y + GRAPH_H / 2) - (RX_TEXT_H / 2))
    #define RX_TEXT_B_Y ((GRAPH_B_Y + GRAPH_H / 2) - (RX_TEXT_H / 2))
	#define RX_RSSI_X (BORDER_GRAPH_L_X + GRAPH_W - 4 * CHAR_WIDTH - 4)
#else
    #define GRAPH_H (SCREEN_HEIGHT - 1)
    #define GRAPH_Y 0
    #define GRAPH_B_Y 0
#endif


namespace StateMachine {
    class BaseGraphUiStateHandler : public StateMachine::StateHandler {
        private:
            virtual void drawBorders();
            virtual void drawChannelText();
            virtual void drawFrequencyText();
            virtual void drawScanBar() {}
            virtual void drawRssiGraph();
            virtual void drawMenu() {}

        public:
            virtual void onEnter() {}
            virtual void onUpdate() {}

            virtual void onInitialDraw();
            virtual void onUpdateDraw();

            virtual void onButtonChange(Button button, Buttons::PressType pressType) {}
            virtual void onFSPinsChange(uint8_t state) {}
    };
}


#endif //STATE_GRAPH_BASE_UI_H
