#include <string.h>

#include "settings.h"
#include "settings_internal.h"
#include "settings_eeprom.h"

#ifdef USE_EXTERNAL_EEPROM
#include "Eeprom24C01_02.h"
#include "eeprom_external.h"
#else
#include "eeprom_emulated.h"
//This is needed to determine how much variable 16-bit elements are stored to flash.
//This is used for example in page data copy. It must be equal to 16-bit EepromSettings struct size wchich is written to flash.
uint16_t DataStructSize = (sizeof(EepromSettings) % 2 == 0) ? sizeof(EepromSettings)/2 : sizeof(EepromSettings)/2 + 1;
#endif

#include "timer.h"

#include <inttypes.h>

static Timer saveTimer = Timer(EEPROM_SAVE_TIME);
static bool isDirty = false;

struct EepromSettings EepromSettings;

#ifdef USE_EXTERNAL_EEPROM
Eeprom24C01_02 eep(0x50);

void EepromSettings::init(I2C_HandleTypeDef *i2c_handle) {
	eep.initialize(i2c_handle);
}
#else
void EepromSettings::init() {
	EE_Init();
}
#endif

void EepromSettings::update() {
	if (isDirty) {
		if (saveTimer.hasTicked()) {
			isDirty = false;
			saveTimer.reset();
			this->save();
		}
	}
}

void EepromSettings::load() {
	EEPROM.get(0, *this);
	if (this->magic != EEPROM_MAGIC)
		this->initDefaults();
}

void EepromSettings::save() {
	EEPROM.put(0, *this);
}

void EepromSettings::markDirty() {
	isDirty = true;
	saveTimer.reset();
}

void EepromSettings::initDefaults() {
	magic = EEPROM_MAGIC;
	startChannel = 0;
	beepEnabled = true;
	screensaverEnabled = true;
	searchManual = false;
	rssiAMin = RSSI_MIN_VAL;
	rssiAMax = RSSI_MAX_VAL;

#ifdef USE_DIVERSITY
	diversityMode = Receiver::DiversityMode::AUTO;
	rssiBMin = RSSI_MIN_VAL;
	rssiBMax = RSSI_MAX_VAL;
#endif

#ifdef USE_VOLTAGE_MONITORING
	vbatScale = VBAT_SCALE;
	vbatWarning = WARNING_VOLTAGE;
	vbatCritical = CRITICAL_VOLTAGE;
#endif
	char sign[CALLSIGN_LEN + 1] = CALLSIGN_DEF;
	memcpy(callsign, sign, CALLSIGN_LEN);

	memcpy(favouriteChannels, EepromDefaults.favouriteChannels, 8);
	//memcpy(this, &EepromDefaults, sizeof(EepromDefaults));
	this->save();
}

void EepromSettings::getCallSign(char *sign) {
	memcpy(sign, callsign, CALLSIGN_LEN);
	sign[CALLSIGN_LEN + 1] = 0;
}

void EepromSettings::setCallSign(char *sign) {
	memcpy(callsign, sign, CALLSIGN_LEN);
}

uint8_t EepromSettings::cmpCallSign(char *sign) {
	return strncmp(sign, (char*) callsign, CALLSIGN_LEN);
}

