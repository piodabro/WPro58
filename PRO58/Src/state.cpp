#include <stddef.h>

#include "state.h"

#include "state_screensaver.h"
#include "state_search.h"
#include "state_bandscan.h"
#include "state_menu.h"
#include "state_settings.h"
#include "state_settings_rssi.h"
#include "state_settings_callsign.h"
#include "state_settings_favourites.h"
#include "state_settings_OSD.h"
#include "state_favourite_screen.h"

#include "ui.h"
#include "buttons.h"
#include "settings_internal.h"
#include "settings_eeprom.h"
#ifdef USE_FS_PINS
	#include "fatshark_pins.h"
#endif


#include "timer.h"


void *operator new(size_t size, void *ptr){
  return ptr;
}

#define MAX(a, b) (a > b ? a : b)
#define STATE_BUFFER_SIZE \
    MAX(sizeof(ScreensaverStateHandler), \
    MAX(sizeof(SearchStateHandler), \
    MAX(sizeof(BandScanStateHandler), \
    MAX(sizeof(MenuStateHandler), \
    MAX(sizeof(SettingsStateHandler), \
	MAX(sizeof(SettingsRssiStateHandler), \
	MAX(sizeof(SettingsCallsignStateHandler), \
	MAX(sizeof(SettingsFavouritesStateHandler), \
	MAX(sizeof(SettingsOSDStateHandler), \
	sizeof(FavouriteScreenStateHandler) \
	)))))))))
;

namespace StateMachine {
    static void onButtonChange(Button button, Buttons::PressType pressType);
    static void onFSPinsChange(uint8_t state);
    static StateHandler *getStateHandler(State stateType);


    static uint8_t stateBuffer[STATE_BUFFER_SIZE];
    static StateHandler* currentHandler = nullptr;
    State currentState = State::BOOT;
    State lastState = currentState;


    void setup() {
        Buttons::registerChangeFunc(onButtonChange);

#ifdef USE_FS_PINS
        FatSharkPins::registerChangeFunc(onFSPinsChange);
#endif
    }

    void update() {
        if (currentHandler) {
            currentHandler->onUpdate();

            // FIXME: This should probably be handled in the UI module but not
            // 100% on how to decouple them at this stage
            static Timer drawTimer = Timer(OLED_FRAMERATE);
            if (currentHandler
                && Ui::shouldDrawUpdate
                && drawTimer.hasTicked()
            ) {
                if (Ui::shouldFullRedraw) {
                    currentHandler->onInitialDraw();
                    Ui::shouldFullRedraw = false;
                }

                currentHandler->onUpdateDraw();
                Ui::shouldDrawUpdate = false;
                drawTimer.reset();
            }
        }
    }

    void switchState(State newState) {
        if (currentHandler != nullptr) {
            currentHandler->onExit();
        }

        lastState = currentState;
        currentState = newState;
        currentHandler = getStateHandler(newState);

        if (currentHandler != nullptr) {
            currentHandler->onEnter();
            currentHandler->onInitialDraw();
        }
    }

    static StateHandler *getStateHandler(State state) {
        #define STATE_FACTORY(s, c) \
            case s: \
                return new (&stateBuffer) c(); \
                break;

        switch (state) {
            STATE_FACTORY(State::SEARCH, SearchStateHandler);
            STATE_FACTORY(State::SCREENSAVER, ScreensaverStateHandler);
            STATE_FACTORY(State::BANDSCAN, BandScanStateHandler);
            STATE_FACTORY(State::MENU, MenuStateHandler);
            STATE_FACTORY(State::FAVOURITES, FavouriteScreenStateHandler);
            STATE_FACTORY(State::SETTINGS, SettingsStateHandler);
            STATE_FACTORY(State::SETTINGS_RSSI, SettingsRssiStateHandler);
            STATE_FACTORY(State::SETTINGS_CALLSIGN, SettingsCallsignStateHandler);
            STATE_FACTORY(State::SETTINGS_FAVOURITES, SettingsFavouritesStateHandler);
            STATE_FACTORY(State::SETTINGS_OSD, SettingsOSDStateHandler);

            default:
                return nullptr;
        }

        #undef STATE_FACTORY
    }

    static void onButtonChange(Button button, Buttons::PressType pressType) {
        if (currentHandler != nullptr) {
            currentHandler->onButtonChange(button, pressType);
        }
    }

    static void onFSPinsChange(uint8_t state) {
		if (currentHandler != nullptr) {
			currentHandler->onFSPinsChange(state);
		}
	}
}
