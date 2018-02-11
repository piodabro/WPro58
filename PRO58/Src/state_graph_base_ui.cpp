#include "state_graph_base_ui.h"

#include "receiver.h"
#include "channels.h"
#include "ui.h"
#include "settings_eeprom.h"


using Ui::display;


void StateMachine::BaseGraphUiStateHandler::onInitialDraw() {
    Ui::clear();

    drawBorders();

    drawChannelText();
    drawFrequencyText();
    drawScanBar();
    drawRssiGraph();

    Ui::needDisplay();
}

void StateMachine::BaseGraphUiStateHandler::onUpdateDraw() {
    Ui::clearRect(
        0,
        0,
        BORDER_GRAPH_L_X,
        CHANNEL_TEXT_H
    );

    Ui::clearRect(
        0,
        FREQUENCY_TEXT_Y,
        BORDER_GRAPH_L_X,
        CHAR_HEIGHT * 2
    );

    Ui::clearRect(
        SCANBAR_X,
        SCANBAR_Y,
        SCANBAR_W,
        SCANBAR_H
    );

    drawChannelText();
    drawFrequencyText();
    drawScanBar();
    drawRssiGraph();
    drawMenu();

    Ui::needDisplay();
}

void StateMachine::BaseGraphUiStateHandler::drawBorders() {
    display.drawRoundRect(
        SCANBAR_BORDER_X,
        SCANBAR_BORDER_Y,
        SCANBAR_BORDER_W,
        SCANBAR_BORDER_H,
        2,
        WHITE
    );

    Ui::drawDashedVLine(
        BORDER_GRAPH_L_X,
        0,
        SCREEN_HEIGHT,
        GRAPH_SEPERATOR_STEP
    );
}

void StateMachine::BaseGraphUiStateHandler::drawChannelText() {
    display.setTextSize(CHANNEL_TEXT_SIZE);
    display.setTextColor(WHITE);
    display.setCursor(CHANNEL_TEXT_X, CHANENL_TEXT_Y);

    display.print(Channels::getName(Receiver::activeChannel));
}

void StateMachine::BaseGraphUiStateHandler::drawFrequencyText() {
    display.setTextSize(FREQUENCY_TEXT_SIZE);
    display.setTextColor(WHITE);
    display.setCursor(FREQUENCY_TEXT_X, FREQUENCY_TEXT_Y);

    display.print(Channels::getFrequency(Receiver::activeChannel));
}

void StateMachine::BaseGraphUiStateHandler::drawRssiGraph() {
    #ifdef USE_DIVERSITY

    display.setTextSize(RX_TEXT_SIZE);
    display.setTextColor(INVERSE);

	if(EepromSettings.diversityMode == Receiver::DiversityMode::AUTO || EepromSettings.diversityMode == Receiver::DiversityMode::FORCE_A){
        Ui::drawGraph(
            Receiver::rssiALast,
            RECEIVER_LAST_DATA_SIZE,
            100,
            GRAPH_X,
            GRAPH_A_Y,
            GRAPH_W,
            GRAPH_H
        );

		if(Receiver::rssiA < 100 && Receiver::rssiA >= 10){
			display.setCursor(RX_RSSI_X + CHAR_WIDTH, RX_TEXT_A_Y);
		} else if (Receiver::rssiA < 10){
			display.setCursor(RX_RSSI_X + CHAR_WIDTH*2, RX_TEXT_A_Y);
		} else {
			display.setCursor(RX_RSSI_X, RX_TEXT_A_Y);
		}
		display.print(Receiver::rssiA);
		display.setCursor(SCREEN_WIDTH - CHAR_WIDTH, RX_TEXT_A_Y);
		display.print("%");

        display.setCursor(RX_TEXT_X, RX_TEXT_A_Y);
        display.print(("A"));
	}

	if(EepromSettings.diversityMode == Receiver::DiversityMode::AUTO || EepromSettings.diversityMode == Receiver::DiversityMode::FORCE_B){
        Ui::drawGraph(
            Receiver::rssiBLast,
            RECEIVER_LAST_DATA_SIZE,
            100,
            GRAPH_X,
            GRAPH_B_Y,
            GRAPH_W,
            GRAPH_H
        );

		if(Receiver::rssiB < 100 && Receiver::rssiB >= 10){
			display.setCursor(RX_RSSI_X + 5, RX_TEXT_B_Y);
		} else if (Receiver::rssiB < 10){
			display.setCursor(RX_RSSI_X + 10, RX_TEXT_B_Y);
		} else {
			display.setCursor(RX_RSSI_X, RX_TEXT_B_Y);
		}
		display.print(Receiver::rssiB);
		display.setCursor(SCREEN_WIDTH - CHAR_WIDTH, RX_TEXT_B_Y);
		display.print("%");

        display.setCursor(RX_TEXT_X, RX_TEXT_B_Y);
        display.print(("B"));
	}

	Ui::drawDashedHLine(
		GRAPH_X,
		GRAPH_SEPERATOR_Y,
		GRAPH_SEPERATOR_W,
		GRAPH_SEPERATOR_STEP
	);

    #else
        Ui::drawGraph(
            Receiver::rssiALast,
            RECEIVER_LAST_DATA_SIZE,
            100,
            GRAPH_X,
            GRAPH_Y,
            GRAPH_W,
            GRAPH_H
        );
    #endif
}
