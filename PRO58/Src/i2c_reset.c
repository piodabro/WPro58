#include "i2c_reset.h"
#include "DWT_Delay.h"

void I2C_Reset(I2C_HandleTypeDef *hi2c, void(*initFunc)()){

	//Software RST
    HAL_I2C_MspInit(hi2c);
    SET_BIT(hi2c->Instance->CR1, I2C_CR1_SWRST);
    DWT_Delay_us(5);
    CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_SWRST);
    DWT_Delay_us(5);

    initFunc();
}
