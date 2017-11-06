#include "stm32f1xx_hal.h"
#include "i2c.h"

#ifdef __cplusplus
 extern "C" {
#endif

void I2C_Reset(I2C_HandleTypeDef hi2c, void(*initFunc)());

#ifdef __cplusplus
 }
#endif
