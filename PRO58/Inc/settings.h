#ifndef SETTINGS_H
#define SETTINGS_H


// === Diversity ===============================================================

#ifdef USE_DIVERSITY
    // RSSI strength should be greater than the value below (percent) over the
    // other receiver before we switch. This pervents flicker when RSSI values
    // are close and delays diversity checks counter.
//    #define DIVERSITY_HYSTERESIS 2
	#define DIVERSITY_HYSTERESIS 4

    // How long (ms) the RSSI strength has to have a greater difference than the
    // above before switching.
    #define DIVERSITY_HYSTERESIS_PERIOD 10//5
#endif

// === RSSI ====================================================================

// RSSI default raw range.
#define RSSI_MIN_VAL 500
#define RSSI_MAX_VAL 1600

// 75% threshold, when channel is printed in spectrum.
#define RSSI_SEEK_FOUND 75

// 80% under max value for RSSI.
#define RSSI_SEEK_TRESHOLD 80

// Scan loops for setup run.
#define RSSI_SETUP_RUN 3

// === Misc ====================================================================

// Key debounce delay in milliseconds.
// Good values are in the range of 100-200ms.
// Shorter values will make it more reactive, but may lead to double trigger.
#define BUTTON_DEBOUNCE_DELAY 80

#define SCREENSAVER_TIMEOUT 30 // Seconds to wait before entering screensaver
#define SCREENSAVER_DISPLAY_CYCLE 3 // Seconds between switching logo/channel

// Time needed to hold mode to get to menu
#define BUTTON_WAIT_FOR_MENU 1000

// rx5808 module need >20ms to tune.
// 25 ms will do a 40 channel scan in 1 second.
#define MIN_TUNE_TIME 40

#define CALLSIGN_LEN 10
#define CALLSIGN_DEF "WPro58"

#endif // file_defined

