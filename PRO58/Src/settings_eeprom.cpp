#include <string.h>
#include "Eeprom24C01_02.h"

#include "settings.h"
#include "settings_internal.h"
#include "settings_eeprom.h"

#include "timer.h"

#include <inttypes.h>





#define E2END 255


static Timer saveTimer = Timer(EEPROM_SAVE_TIME);
static bool isDirty = false;

struct EepromSettings EepromSettings;

Eeprom24C01_02 eep(0x50);


struct EERef{

    EERef( const int index )
        : index( index )                 {}
    
    //Access/read members.
    uint8_t operator*() const            { return eep.readByte( index ); }
    operator const uint8_t() const       { return **this; }
    
    //Assignment/write members.
    EERef &operator=( const EERef &ref ) { return *this = *ref; }
    EERef &operator=( uint8_t in )       { return eep.writeByte( index, in ), *this;  }
    EERef &operator +=( uint8_t in )     { return *this = **this + in; }
    EERef &operator -=( uint8_t in )     { return *this = **this - in; }
    EERef &operator *=( uint8_t in )     { return *this = **this * in; }
    EERef &operator /=( uint8_t in )     { return *this = **this / in; }
    EERef &operator ^=( uint8_t in )     { return *this = **this ^ in; }
    EERef &operator %=( uint8_t in )     { return *this = **this % in; }
    EERef &operator &=( uint8_t in )     { return *this = **this & in; }
    EERef &operator |=( uint8_t in )     { return *this = **this | in; }
    EERef &operator <<=( uint8_t in )    { return *this = **this << in; }
    EERef &operator >>=( uint8_t in )    { return *this = **this >> in; }
    
    EERef &update( uint8_t in )          { return  in != *this ? *this = in : *this; }
    
    /** Prefix increment/decrement **/
    EERef& operator++()                  { return *this += 1; }
    EERef& operator--()                  { return *this -= 1; }
    
    /** Postfix increment/decrement **/
    uint8_t operator++ (int){ 
        uint8_t ret = **this;
        return ++(*this), ret;
    }

    uint8_t operator-- (int){ 
        uint8_t ret = **this;
        return --(*this), ret;
    }
    
    int index; //Index of current EEPROM cell.
};

/***
    EEPtr class.
    
    This object is a bidirectional pointer to EEPROM cells represented by EERef objects.
    Just like a normal pointer type, this can be dereferenced and repositioned using 
    increment/decrement operators.
***/

struct EEPtr{

    EEPtr( const int index )
        : index( index )                {}
        
    operator const int() const          { return index; }
    EEPtr &operator=( int in )          { return index = in, *this; }
    
    //Iterator functionality.
    bool operator!=( const EEPtr &ptr ) { return index != ptr.index; }
    EERef operator*()                   { return index; }
    
    /** Prefix & Postfix increment/decrement **/
    EEPtr& operator++()                 { return ++index, *this; }
    EEPtr& operator--()                 { return --index, *this; }
    EEPtr operator++ (int)              { return index++; }
    EEPtr operator-- (int)              { return index--; }

    int index; //Index of current EEPROM cell.
};

/***
    EEPROMClass class.
    
    This object represents the entire EEPROM space.
    It wraps the functionality of EEPtr and EERef into a basic interface.
    This class is also 100% backwards compatible with earlier Arduino core releases.
***/

struct EEPROMClass{

    //Basic user access methods.
    EERef operator[]( const int idx )    { return idx; }
    uint8_t read( int idx )              { return EERef( idx ); }
    void write( int idx, uint8_t val )   { (EERef( idx )) = val; }
    void update( int idx, uint8_t val )  { EERef( idx ).update( val ); }
    
    //STL and C++11 iteration capability.
    EEPtr begin()                        { return 0x00; }
    EEPtr end()                          { return length(); } //Standards requires this to be the item after the last valid entry. The returned pointer is invalid.
    uint16_t length()                    { return E2END + 1; }
    
    //Functionality to 'get' and 'put' objects to and from EEPROM.
    template< typename T > T &get( int idx, T &t ){
        EEPtr e = idx;
        uint8_t *ptr = (uint8_t*) &t;
        for( int count = sizeof(T) ; count ; --count, ++e )  *ptr++ = *e;
        return t;
    }
    
    template< typename T > const T &put( int idx, const T &t ){
        EEPtr e = idx;
        const uint8_t *ptr = (const uint8_t*) &t;
        for( int count = sizeof(T) ; count ; --count, ++e )  (*e).update( *ptr++ );
        return t;
    }
};

static EEPROMClass EEPROM;


void EepromSettings::init(I2C_HandleTypeDef *i2c_handle) {
    eep.initialize(i2c_handle);
}

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

