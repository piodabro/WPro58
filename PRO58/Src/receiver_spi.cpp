#include <stdint.h>

#include "receiver_spi.h"
#include "settings.h"
#include "stm32f1xx_hal.h"
#include "DWT_Delay.h"

static inline void sendBit(uint8_t value);
static inline void sendBits(uint32_t bits, uint8_t count = 20);
static inline void sendSlaveSelect(GPIO_PinState value);
static inline void sendRegister(uint8_t address, uint32_t data);

#define SPI_ADDRESS_SYNTH_A 0x01
#define SPI_ADDRESS_POWER 0x0A


namespace ReceiverSpi {
    //
    // Sends SPI command to receiver module to change frequency.
    //
    // Format is LSB first, with the following bits in order:
    //     4 bits - address
    //     1 bit  - read/write enable
    //    20 bits - data
    //
    // Address for frequency select (Synth Register B) is 0x1
    // Expected data is (LSB):
    //     7 bits - A counter divider ratio
    //      1 bit - seperator
    //    12 bits - N counter divder ratio
    //
    // Forumla for calculating N and A is:/
    //    F_lo = 2 * (N * 32 + A) * (F_osc / R)
    //    where:
    //        F_osc = 8 Mhz
    //        R = 8
    //
    // Refer to RTC6715 datasheet for further details.
    //
    void setSynthRegisterB(uint16_t value) {
        sendRegister(SPI_ADDRESS_SYNTH_A, value);
    }

    void setPowerDownRegister(uint32_t value) {
        sendRegister(SPI_ADDRESS_POWER, value);
    }
}

static inline void sendRegister(uint8_t address, uint32_t data) {


    sendSlaveSelect(GPIO_PIN_RESET);

    sendBits(address, 4);
    sendBit(0x1); // Enable write.

    sendBits(data, 20);

    // Finished clocking data in
    sendSlaveSelect(GPIO_PIN_SET);
}


static inline void sendBits(uint32_t bits, uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        sendBit(bits & 0x1);
        bits >>= 1;
    }
}

static inline void sendBit(uint8_t value) {
    HAL_GPIO_WritePin(SPI_CLOCK_GPIO_Port,SPI_CLOCK_Pin,GPIO_PIN_RESET);//digitalWrite(PIN_SPI_CLOCK, LOW);
    DWT_Delay_us(1);//delayMicroseconds(1);

    HAL_GPIO_WritePin(SPI_DATA_GPIO_Port,SPI_DATA_Pin, value > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);//digitalWrite(PIN_SPI_DATA, value);
    DWT_Delay_us(1);//delayMicroseconds(1);
    HAL_GPIO_WritePin(SPI_CLOCK_GPIO_Port,SPI_CLOCK_Pin,GPIO_PIN_SET);//digitalWrite(PIN_SPI_CLOCK, HIGH);
    DWT_Delay_us(2);//delayMicroseconds(1);

    HAL_GPIO_WritePin(SPI_CLOCK_GPIO_Port,SPI_CLOCK_Pin,GPIO_PIN_RESET);//digitalWrite(PIN_SPI_CLOCK, LOW);
    DWT_Delay_us(1);//delayMicroseconds(1);
}

static void sendSlaveSelect(GPIO_PinState value) {
	HAL_GPIO_WritePin(SPI_SLAVE_SELECT_A_GPIO_Port,SPI_SLAVE_SELECT_A_Pin,value);
    HAL_GPIO_WritePin(SPI_SLAVE_SELECT_B_GPIO_Port,SPI_SLAVE_SELECT_B_Pin,value);
    DWT_Delay_us(1);

//    digitalWrite(PIN_SPI_SLAVE_SELECT_A, value);
//    digitalWrite(PIN_SPI_SLAVE_SELECT_B, value);
//    delayMicroseconds(1);
}
