/*
 * This lib is reimplementation of lib described in AN2594: EEPROM emulation in STM32F10x microcontrollers
 * PioDabro reimplemented this to use HAL libraries
 * Due to problems with optimization this lib has disabled optimizations.
 * */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef EEPROM_EMULATED_H_
#define EEPROM_EMULATED_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#pragma GCC push_options
#pragma GCC optimize ("O0")

/* Exported constants --------------------------------------------------------*/
/* Define the STM32F10Xxx Flash page size depending on the used STM32 device */
#define PAGE_SIZE FLASH_PAGE_SIZE  /* Page size = 1KByte */

/* EEPROM start address in Flash */
#define EEPROM_START_ADDRESS    ((uint32_t)0x0800F000U) /* EEPROM emulation start address:
                                                  after 60KByte of used Flash memory */

/* Pages 0 and 1 base and end addresses */
#define PAGE0_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + 0x000U))
#define PAGE0_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (PAGE_SIZE - 1)))

#define PAGE1_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + PAGE_SIZE))
#define PAGE1_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (2 * PAGE_SIZE - 1)))

/* Used Flash pages for EEPROM emulation */
#define PAGE0                   ((uint16_t)0x0000)
#define PAGE1                   ((uint16_t)0x0001)

/* No valid page define */
#define NO_VALID_PAGE           ((uint16_t)0x00AB)

/* Page status definitions */
#define ERASED                  ((uint16_t)0xFFFF)     /* PAGE is empty */
#define RECEIVE_DATA            ((uint16_t)0xEEEE)     /* PAGE is marked to receive data */
#define VALID_PAGE              ((uint16_t)0x0000)     /* PAGE containing valid data */

/* Valid pages in read and write defines */
#define READ_FROM_VALID_PAGE    ((uint8_t)0x00)
#define WRITE_IN_VALID_PAGE     ((uint8_t)0x01)

/* Page full define */
#define PAGE_FULL               ((uint8_t)0x80)

/* Variables' number */
//#define NumbOfVar               ((uint8_t)0x03)

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint16_t EE_Init(void);
uint16_t EE_ReadVariable(uint16_t VirtAddress, uint16_t* Data);
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint16_t Data);


#define E2END 255

struct EERef{

    EERef( const int index )
        : index( index )                 {}

    //Access/read members.
    uint16_t operator*() const            {
    	 uint16_t data;
    	 EE_ReadVariable(index, &data);
    	 return data;
    }
    operator const uint16_t() const       { return **this; }

    //Assignment/write members.
    EERef &operator=( const EERef &ref ) { return *this = *ref; }
    EERef &operator=( uint16_t in )       {
    	EE_WriteVariable(index, in);
    	return *this;
    }
    EERef &operator +=( uint16_t in )     { return *this = **this + in; }
    EERef &operator -=( uint16_t in )     { return *this = **this - in; }
    EERef &operator *=( uint16_t in )     { return *this = **this * in; }
    EERef &operator /=( uint16_t in )     { return *this = **this / in; }
    EERef &operator ^=( uint16_t in )     { return *this = **this ^ in; }
    EERef &operator %=( uint16_t in )     { return *this = **this % in; }
    EERef &operator &=( uint16_t in )     { return *this = **this & in; }
    EERef &operator |=( uint16_t in )     { return *this = **this | in; }
    EERef &operator <<=( uint16_t in )    { return *this = **this << in; }
    EERef &operator >>=( uint16_t in )    { return *this = **this >> in; }

    EERef &update( uint16_t in )          { return  in != *this ? *this = in : *this; }

    /** Prefix increment/decrement **/
    EERef& operator++()                  { return *this += 1; }
    EERef& operator--()                  { return *this -= 1; }

    /** Postfix increment/decrement **/
    uint16_t operator++ (int){
        uint16_t ret = **this;
        return ++(*this), ret;
    }

    uint16_t operator-- (int){
        uint16_t ret = **this;
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
    It comes from Arduino...
***/
struct EEPROMClass{

    //Basic user access methods.
    EERef operator[]( const int idx )    { return idx; }
    uint16_t read( int idx )              { return EERef( idx ); }
    void write( int idx, uint16_t val )   { (EERef( idx )) = val; }
    void update( int idx, uint16_t val )  { EERef( idx ).update( val ); }

    //STL and C++11 iteration capability.
    EEPtr begin()                        { return 0x00; }
    EEPtr end()                          { return length(); } //Standards requires this to be the item after the last valid entry. The returned pointer is invalid.
    uint16_t length()                    { return E2END + 1; }

    //Functionality to 'get' and 'put' objects to and from EEPROM.
    template< typename T > T &get( int idx, T &t ){
        EEPtr e = idx;
        uint16_t *ptr = (uint16_t*) &t;
        int cnt = (sizeof(T) % 2 == 0) ? sizeof(T)/2 : sizeof(T)/2 + 1;
        for( int count = cnt ; count ; --count, ++e )  *ptr++ = *e;
        return t;
    }

    template< typename T > const T &put( int idx, const T &t ){
        EEPtr e = idx;
        const uint16_t *ptr = (const uint16_t*) &t;
        int cnt = (sizeof(T) % 2 == 0) ? sizeof(T)/2 : sizeof(T)/2 + 1;
        for( int count = cnt ; count ; --count, ++e )  (*e).update( *ptr++ );
        return t;
    }
};

static EEPROMClass EEPROM;


#pragma GCC pop_options

#endif /* EEPROM_EMULATED_H_ */
//

