/*
 * OSD58_HW.h
 *
 *  Created on: 15.10.2018
 *      Author: PDabrowski
 */

#ifndef HW_OSD58_H_
#define HW_OSD58_H_


#define RECEIVER_SW_Pin                     GPIO_PIN_4
#define RECEIVER_SW_GPIO_Port               GPIOA
#define RSSI_A_ADC_Pin                      GPIO_PIN_0
#define RSSI_A_ADC_GPIO_Port                GPIOA
#define RSSI_B_ADC_Pin                      GPIO_PIN_1
#define RSSI_B_ADC_GPIO_Port                GPIOA

#define LED_A_Pin                           GPIO_PIN_15
#define LED_A_GPIO_Port                     GPIOC
#define LED_B_Pin                           GPIO_PIN_14
#define LED_B_GPIO_Port                     GPIOC
#define LED_STATUS_Pin                      GPIO_PIN_15
#define LED_STATUS_GPIO_Port                GPIOA

#define SPI_SLAVE_SELECT_B_Pin              GPIO_PIN_13
#define SPI_SLAVE_SELECT_B_GPIO_Port        GPIOC
#define SPI_SLAVE_SELECT_A_Pin              GPIO_PIN_5
#define SPI_SLAVE_SELECT_A_GPIO_Port        GPIOB
#define SPI_DATA_Pin                        GPIO_PIN_2
#define SPI_DATA_GPIO_Port                  GPIOA
#define SPI_CLOCK_Pin                       GPIO_PIN_3
#define SPI_CLOCK_GPIO_Port                 GPIOA

#define BUTTON_DOWN_Pin                     GPIO_PIN_12
#define BUTTON_DOWN_GPIO_Port               GPIOB
#define BUTTON_MODE_Pin                     GPIO_PIN_11
#define BUTTON_MODE_GPIO_Port               GPIOA
#define BUTTON_UP_Pin                       GPIO_PIN_12
#define BUTTON_UP_GPIO_Port                 GPIOA

#define FS_PIN_EB0_Pin                      GPIO_PIN_8
#define FS_PIN_EB0_GPIO_Port                GPIOA
#define FS_PIN_EB1_Pin                      GPIO_PIN_9
#define FS_PIN_EB1_GPIO_Port                GPIOA
#define FS_PIN_EB2_Pin                      GPIO_PIN_10
#define FS_PIN_EB2_GPIO_Port                GPIOA

#define I2C_DISPLAY_SCL_Pin                 GPIO_PIN_6
#define I2C_DISPLAY_SCL_GPIO_Port           GPIOB
#define I2C_DISPLAY_SDA_Pin                 GPIO_PIN_7
#define I2C_DISPLAY_SDA_GPIO_Port           GPIOB

#define I2C_EEPROM_SCL_Pin                  GPIO_PIN_10
#define I2C_EEPROM_SCL_GPIO_Port            GPIOB
#define I2C_EEPROM_SDA_Pin                  GPIO_PIN_11
#define I2C_EEPROM_SDA_GPIO_Port            GPIOB

#define BUZZER_Pin                          GPIO_PIN_8
#define BUZZER_GPIO_Port                    GPIOB


#define OSD_VSYNC_PORT                      GPIOB
#define OSD_VSYNC_PIN                       GPIO_PIN_14
#define OSD_VSYNC_IRQ                       EXTI15_10_IRQn

#define OSD_CSYNC_PORT                      GPIOB
#define OSD_CSYNC_PIN                       GPIO_PIN_9
#define OSD_CSYNC_IRQ                       EXTI9_5_IRQn

#define OSD_SPI_A                           SPI2
#define OSD_DMA_SPI_A                       DMA1_Channel5
#define OSD_DMA_SPI_A_IRQ                   DMA1_Channel5_IRQn

#define OSD_SPI_B                           SPI1
#define OSD_DMA_SPI_B                       DMA1_Channel3
#define OSD_DMA_SPI_B_IRQ                   DMA1_Channel3_IRQn

#define OSD_SYNC_OUT_PORT                   GPIOB
#define OSD_SYNC_OUT_PIN                    GPIO_PIN_1
#define OSD_TIM_CCR                         CCR3
#define OSD_TIM_CHANNEL                     3
#define OSD_TIM_IRQ                         TIM1_UP_IRQn


#endif /* HW_OSD58_H_ */
