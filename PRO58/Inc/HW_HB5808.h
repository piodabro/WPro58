/*
 * HW_HB5808.h
 *
 *  Created on: 15.10.2018
 *      Author: PDabrowski
 */

#ifndef HW_HB5808_H_
#define HW_HB5808_H_

#define RECEIVER_SW_Pin GPIO_PIN_15
#define RECEIVER_SW_GPIO_Port GPIOB
#define RSSI_A_ADC_Pin GPIO_PIN_0
#define RSSI_A_ADC_GPIO_Port GPIOA
#define RSSI_B_ADC_Pin GPIO_PIN_1
#define RSSI_B_ADC_GPIO_Port GPIOA

#define SPI_SLAVE_SELECT_B_Pin GPIO_PIN_13
#define SPI_SLAVE_SELECT_B_GPIO_Port GPIOB	// the same one cause HB5808 has single.
#define SPI_SLAVE_SELECT_A_Pin GPIO_PIN_13  // This is just a bit mask. A | B = A = B
#define SPI_SLAVE_SELECT_A_GPIO_Port GPIOB

#define SPI_DATA_Pin GPIO_PIN_12
#define SPI_DATA_GPIO_Port GPIOB
#define SPI_CLOCK_Pin GPIO_PIN_14
#define SPI_CLOCK_GPIO_Port GPIOB

#define BUTTON_DOWN_Pin GPIO_PIN_2
#define BUTTON_DOWN_GPIO_Port GPIOA
#define BUTTON_MODE_Pin GPIO_PIN_3
#define BUTTON_MODE_GPIO_Port GPIOA
#define BUTTON_UP_Pin GPIO_PIN_4
#define BUTTON_UP_GPIO_Port GPIOA

#define I2C_DISPLAY_SCL_Pin GPIO_PIN_10
#define I2C_DISPLAY_SCL_GPIO_Port GPIOB
#define I2C_DISPLAY_SDA_Pin GPIO_PIN_11
#define I2C_DISPLAY_SDA_GPIO_Port GPIOB

#define LED_A_Pin GPIO_PIN_0
#define LED_A_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_1
#define LED_B_GPIO_Port GPIOB
#define LED_STATUS_Pin GPIO_PIN_2
#define LED_STATUS_GPIO_Port GPIOB

#undef USE_FS_PINS


#endif /* HW_HB5808_H_ */
