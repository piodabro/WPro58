#include <stdint.h>

#include "state_settings_rssi.h"

#include "receiver.h"
#include "channels.h"

#include "settings.h"
#include "settings_internal.h"
#include "settings_eeprom.h"
#include "buttons.h"

#include "ui.h"
//#include "pstr_helper.h"

#define ADC_MAX 4095
#define PROBES_PER_CHANNEL 2

void StateMachine::SettingsRssiStateHandler::onEnter() {
    internalState = InternalState::WAIT_FOR_LOW;
}

void StateMachine::SettingsRssiStateHandler::onUpdate() {
    if (!Receiver::isRssiStable())
        return;

    switch (internalState) {
        case InternalState::SCANNING_LOW:
            if (Receiver::rssiARaw < EepromSettings.rssiAMin)
                EepromSettings.rssiAMin = Receiver::rssiARaw;

            #ifdef USE_DIVERSITY
                if (Receiver::rssiBRaw < EepromSettings.rssiBMin)
                    EepromSettings.rssiBMin = Receiver::rssiBRaw;
            #endif
        break;

        case InternalState::SCANNING_HIGH:
            if (Receiver::rssiARaw > EepromSettings.rssiAMax)
                EepromSettings.rssiAMax = Receiver::rssiARaw;

            #ifdef USE_DIVERSITY
                if (Receiver::rssiBRaw > EepromSettings.rssiBMax)
                    EepromSettings.rssiBMax = Receiver::rssiBRaw;
            #endif
        break;
    }

    Receiver::setChannel((Receiver::activeChannel + 1) % CHANNELS_SIZE);

    if (Receiver::activeChannel == 0) {
        currentSweep++;

        if (currentSweep == RSSI_SETUP_RUN) {
            switch (internalState) {
                case InternalState::SCANNING_LOW:
                    internalState = InternalState::WAIT_FOR_HIGH;
                break;

                case InternalState::SCANNING_HIGH:
                    internalState = InternalState::DONE;
                break;
            }

            Ui::needUpdate();
        }
    }
}

void StateMachine::SettingsRssiStateHandler::onButtonChange(
    Button button,
    Buttons::PressType pressType
) {
    if (button != Button::MODE || pressType != Buttons::PressType::SHORT)
        return;

    switch (internalState) {
        case InternalState::WAIT_FOR_LOW:
            internalState = InternalState::SCANNING_LOW;
            currentSweep = 0;
            Receiver::setChannel(0);

            EepromSettings.rssiAMin = ADC_MAX;
            #ifdef USE_DIVERSITY
                EepromSettings.rssiBMin = ADC_MAX;
            #endif
        break;

        case InternalState::WAIT_FOR_HIGH:
            internalState = InternalState::SCANNING_HIGH;
            currentSweep = 0;
            Receiver::setChannel(0);

            EepromSettings.rssiAMax = 0;
            #ifdef USE_DIVERSITY
                EepromSettings.rssiBMax = 0;
            #endif
        break;

        case InternalState::DONE:
            EepromSettings.save();
            StateMachine::switchState(StateMachine::State::SETTINGS);
        break;
    }

    Ui::needUpdate();
}


void StateMachine::SettingsRssiStateHandler::onInitialDraw() {
    Ui::needUpdate(); // Lazy. :(
}

void StateMachine::SettingsRssiStateHandler::onUpdateDraw() {
    Ui::clear();

    switch (internalState) {
        case InternalState::WAIT_FOR_LOW:
            Ui::display.setTextSize(1);
            Ui::display.setCursor(0, 0);
            Ui::display.print(("1/4\nTurn off all VTXs."));
            Ui::display.setCursor(0, (CHAR_HEIGHT + 1) * 2);
            Ui::display.print(("Remove RX antennas."));

            Ui::display.setCursor(0, SCREEN_HEIGHT - CHAR_HEIGHT - 1);
            Ui::display.print(("Press MODE when ready."));
        break;

        case InternalState::SCANNING_LOW:
            Ui::display.setTextSize(1);
            Ui::display.setCursor(0, 0);
            Ui::display.print(("2/4\nScanning for lowest\nRSSI..."));
        break;

        case InternalState::WAIT_FOR_HIGH:
            Ui::display.setTextSize(1);
            Ui::display.setCursor(0, 0);
            Ui::display.print(("3/4\nTurn on your VTX."));

            Ui::display.setCursor(0, SCREEN_HEIGHT - CHAR_HEIGHT - 1);
            Ui::display.print(("Press MODE when ready."));
        break;

        case InternalState::SCANNING_HIGH:
            Ui::display.setTextSize(1);
            Ui::display.setCursor(0, 0);
            Ui::display.print(("4/4\nScanning for highest\nRSSI..."));
        break;

        case InternalState::DONE:
            Ui::display.setTextSize(1);
            Ui::display.setCursor(0, 0);
            Ui::display.print(("All done!"));

            Ui::display.setCursor(0, CHAR_HEIGHT * 2);
            Ui::display.print(("Min: "));

            Ui::display.setCursor((CHAR_WIDTH + 1) * 5, CHAR_HEIGHT * 2);
            Ui::display.print(EepromSettings.rssiAMin);
            #ifdef USE_DIVERSITY
                Ui::display.setCursor((CHAR_WIDTH + 1) * 12, CHAR_HEIGHT * 2);
                Ui::display.print(EepromSettings.rssiAMin);
            #endif

            Ui::display.setCursor(0, CHAR_HEIGHT * 3 + 1);
            Ui::display.print(("Max: "));

            Ui::display.setCursor((CHAR_WIDTH + 1) * 5, CHAR_HEIGHT * 3 + 1);
            Ui::display.print(EepromSettings.rssiAMax);
            #ifdef USE_DIVERSITY
                Ui::display.setCursor((CHAR_WIDTH + 1) * 12,
                    CHAR_HEIGHT * 3 + 1);
                Ui::display.print(EepromSettings.rssiBMax);
            #endif

            Ui::display.setCursor(0, SCREEN_HEIGHT - CHAR_HEIGHT - 1);
            Ui::display.print(("Press MODE to save."));
        break;
    }

    Ui::needDisplay();
}
