#include <string.h>


#include "settings.h"
#include "settings_internal.h"
#include "settings_eeprom.h"

#ifndef HB5808
#include "Eeprom24C01_02.h"
#include "eeprom_external.h"
#else
#include "eeprom_emulated.h"
uint16_t DataStructSize = sizeof(EepromSettings);
#endif

#include "timer.h"

#include <inttypes.h>

static Timer saveTimer = Timer(EEPROM_SAVE_TIME);
static bool isDirty = false;

struct EepromSettings EepromSettings;

#ifndef HB5808
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
#ifndef HB5808
    EEPROM.get(0, *this);
#else
    uint16_t *ptr = (uint16_t*) this;
	for( uint16_t count = 0; count < sizeof(EepromSettings)/2; ++count )  {
		EE_ReadVariable(count, ptr++);
	}
#endif
    
    if (this->magic != EEPROM_MAGIC)
        this->initDefaults();
}

void EepromSettings::save() {
#ifndef HB5808
   EEPROM.put(0, *this);
#else
   const uint16_t *ptr = (const uint16_t*) this;
   for( uint16_t count = 0; count < sizeof(EepromSettings)/2 ; ++count)  {
	   EE_WriteVariable(count, *ptr);
	   ++ptr;
   }
#endif

}

void EepromSettings::markDirty() {
    isDirty = true;
    saveTimer.reset();
}

void EepromSettings::initDefaults() {
	magic = EEPROM_MAGIC;
	startChannel = 0;
	beepEnabled = true;
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
	char sign[CALLSIGN_LEN+1] = CALLSIGN_DEF;
	memcpy(callsign, sign, CALLSIGN_LEN);

    //memcpy(this, &EepromDefaults, sizeof(EepromDefaults));
    this->save();
}

void EepromSettings::getCallSign(char *sign){
	memcpy(sign, callsign, CALLSIGN_LEN);
	sign[CALLSIGN_LEN+1]=0;
}

void EepromSettings::setCallSign(char *sign){
	memcpy(callsign, sign, CALLSIGN_LEN);
}

uint8_t EepromSettings::cmpCallSign(char *sign){
	return strncmp(sign, (char*)callsign, CALLSIGN_LEN);
}

