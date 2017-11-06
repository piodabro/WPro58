/**************************************************************************//**
 * \brief EEPROM 24C01 / 24C02 library for Arduino
 * \author Copyright (C) 2012  Julien Le Sech - www.idreammicro.com
 * \version 1.0
 * \date 20120217
 *
 * This file is part of the EEPROM 24C01 / 24C02 library for Arduino.
 *
 * This library is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/
 ******************************************************************************/

/**************************************************************************//**
 * \headerfile Eeprom24C01_02.h
 ******************************************************************************/

#ifndef Eeprom24C01_02_h
#define Eeprom24C01_02_h

/******************************************************************************
 * Header file inclusion.
 ******************************************************************************/
#include "i2c.h"
#include "i2c_reset.h"

/*
  EEPROM.h - EEPROM library
  Original Copyright (c) 2006 David A. Mellis.  All right reserved.
  New version by Christopher Andrews 2015.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef EEPROM_h
#define EEPROM_h

#include "stm32f1xx_hal.h"

/**************************************************************************//**
 * \class Eeprom24C01_02
 *
 * \brief EEPROM 24C01 / 24C02 memory driver.
 *
 * This driver is designed for 24C01 and 24C02 EEPROM memories.
 ******************************************************************************/
class Eeprom24C01_02
{
    public:
    
        /******************************************************************//**
         * \fn Eeprom24C01_02(uint8_t deviceAddress)
         *
         * \brief Constructor.
         *
         * \param   deviceAddress   EEPROM address on TWI bus.
         **********************************************************************/
        Eeprom24C01_02
        (
            uint8_t deviceAddress
        );

        /******************************************************************//**
         * \fn void initialize()
         *
         * \brief Initialize library abnd TWI bus.
         * 
         * If several devices are connected to TWI bus, this method mustn't be
         * called. TWI bus must be initialized out of this library using
         * Wire.begin() method.
         **********************************************************************/        
        void
        initialize(I2C_HandleTypeDef *i2c_handle);

        /******************************************************************//**
         * \fn void writeByte(
         * uint8_t address,
         * uint8_t data)
         *
         * \brief Write a byte in EEPROM memory.
         *
         * \remarks A delay of 10 ms is required after write cycle.
         *
         * \param   address Address.
         * \param   data    Byte to write.
         **********************************************************************/
        void
        writeByte
        (
            uint8_t    address,
            uint8_t    data
        );
        
        /******************************************************************//**
         * \fn void writeBytes(
         * uint8_t     address,
         * uint8_t     length,
         * uint8_t*    p_data)
         * 
         * \brief Write bytes in EEPROM memory.
         *
         * \param       address Start address.
         * \param       length  Number of bytes to write.
         * \param[in]   p_data  Bytes to write.
         **********************************************************************/
        void
        writeBytes
        (
            uint8_t    address,
            uint8_t    length,
            uint8_t*   p_data
        );
        
        /******************************************************************//**
         * \fn uint8_t readByte(uint8_t address)
         * 
         * \brief Read a byte in EEPROM memory.
         *
         * \param   address Address.
         *
         * \return Read byte.
         **********************************************************************/
        uint8_t
        readByte
        (
            uint8_t    address
        );

        /******************************************************************//**
         * \fn void readBytes(
         * uint8_t     address,
         * uint8_t     length,
         * uint8_t*    p_data)
         *
         * \brief Read bytes in EEPROM memory.
         *
         * \param       address Start address.
         * \param       length  Number of bytes to read.
         * \patam[in]   p_data  Byte array to fill with read bytes.
         **********************************************************************/
        void
        readBytes
        (
            uint8_t    address,
            uint8_t    length,
            uint8_t*   p_buffer
        );
        
    private:

        uint8_t m_deviceAddress;
        I2C_HandleTypeDef *i2c_handler;

        /******************************************************************//**
         * \fn void writePage(
         * uint8_t     address,
         * uint8_t     length,
         * uint8_t*    p_data)
         *
         * \brief Write page in EEPROM memory.
         *
         * \param       address Start address.
         * \param       length  Number of bytes (64 bytes max).
         * \param[in]   p_data  Data.
         **********************************************************************/
        void
        writePage
        (
            uint8_t    address,
            uint8_t    length,
            uint8_t*   p_data
        );

        /******************************************************************//**
         * \fn void writeBuffer(
         * uint8_t     address,
         * uint8_t     length,
         * uint8_t*    p_data)
         *
         * \brief Write bytes into memory.
         *
         * \param       address Start address.
         * \param       length  Number of bytes (30 bytes max).
         * \param[in]   p_date  Data.
         **********************************************************************/
        void
        writeBuffer
        (
            uint8_t    address,
            uint8_t    length,
            uint8_t*   p_data
       );

        /******************************************************************//**
         * \fn void readBuffer(
         * byte     address,
         * byte     length,
         * byte*    p_data)
         *
         * \brief Read bytes in memory.
         *
         * \param       address Start address.
         * \param       length  Number of bytes to read (32 bytes max).
         * \param[in]   p_data  Buffer to fill with read bytes.
         **********************************************************************/
        void
        readBuffer
        (
            uint8_t    address,
            uint8_t    length,
            uint8_t*   p_data
        );
};

/***
    EERef class.
    
    This object references an EEPROM cell.
    Its purpose is to mimic a typical byte of RAM, however its storage is the EEPROM.
    This class has an overhead of two bytes, similar to storing a pointer to an EEPROM cell.
***/


#endif



#endif // Eeprom24C01_02_h

