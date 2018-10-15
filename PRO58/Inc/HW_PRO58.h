/*
 * PRO58_HW.h
 *
 *  Created on: 15.10.2018
 *      Author: PDabrowski
 */

#ifndef HW_PRO58_H_
#define HW_PRO58_H_

#define RECEIVER_SW_Pin						GPIO_PIN_15
#define RECEIVER_SW_GPIO_Port 				GPIOC
#define RSSI_A_ADC_Pin 						GPIO_PIN_0
#define RSSI_A_ADC_GPIO_Port 				GPIOA
#define RSSI_B_ADC_Pin 						GPIO_PIN_1
#define RSSI_B_ADC_GPIO_Port 				GPIOA
#define LED_B_Pin 							GPIO_PIN_2
#define LED_B_GPIO_Port 					GPIOA
#define SPI_SLAVE_SELECT_B_Pin 				GPIO_PIN_3
#define SPI_SLAVE_SELECT_B_GPIO_Port 		GPIOA
#define SPI_SLAVE_SELECT_A_Pin 				GPIO_PIN_4
#define SPI_SLAVE_SELECT_A_GPIO_Port 		GPIOA
#define SPI_DATA_Pin 						GPIO_PIN_5
#define SPI_DATA_GPIO_Port 					GPIOA
#define SPI_CLOCK_Pin 						GPIO_PIN_7
#define SPI_CLOCK_GPIO_Port 				GPIOA
#define I2C_EEPROM_SCL_Pin 					GPIO_PIN_10
#define I2C_EEPROM_SCL_GPIO_Port 			GPIOB
#define I2C_EEPROM_SDA_Pin 					GPIO_PIN_11
#define I2C_EEPROM_SDA_GPIO_Port			GPIOB
#define BUTTON_DOWN_Pin 					GPIO_PIN_12
#define BUTTON_DOWN_GPIO_Port 				GPIOB
#define FS_PIN_EB0_Pin 						GPIO_PIN_13
#define FS_PIN_EB0_GPIO_Port 				GPIOB
#define BUTTON_MODE_Pin 					GPIO_PIN_14
#define BUTTON_MODE_GPIO_Port 				GPIOB
#define BUTTON_UP_Pin 						GPIO_PIN_8
#define BUTTON_UP_GPIO_Port 				GPIOA
#define FS_PIN_EB1_Pin 						GPIO_PIN_11
#define FS_PIN_EB1_GPIO_Port 				GPIOA
#define I2C_DISPLAY_SCL_Pin 				GPIO_PIN_6
#define I2C_DISPLAY_SCL_GPIO_Port	 		GPIOB
#define I2C_DISPLAY_SDA_Pin 				GPIO_PIN_7
#define I2C_DISPLAY_SDA_GPIO_Port 			GPIOB
#define BUZZER_Pin 							GPIO_PIN_8
#define BUZZER_GPIO_Port 					GPIOB
#define LED_A_Pin 							GPIO_PIN_9
#define LED_A_GPIO_Port 					GPIOB

#ifdef USE_OSD
	//PA9     ------> TIM1_CH2
	#define OSD_SYNC_OUT_PORT                   GPIOA
	#define OSD_SYNC_OUT_PIN                    GPIO_PIN_9
	#define OSD_TIM_CCR                         CCR2
	#define OSD_TIM_CHANNEL                     2
	#define OSD_TIM_IRQ                         TIM1_UP_IRQn

	#define OSD_SPI_A                           SPI2
	#define OSD_SPI_A_PORT                      GPIOB
	#define OSD_SPI_A_PIN                       GPIO_PIN_15
	#define OSD_DMA_SPI_A                       DMA1_Channel5
	#define OSD_DMA_SPI_A_IRQ                   DMA1_Channel5_IRQn

	#ifdef USE_OSD_OVERLAY
	#define OSD_VSYNC_PORT                      GPIOA
	#define OSD_VSYNC_PIN                       GPIO_PIN_10
	#define OSD_VSYNC_IRQ                       EXTI15_10_IRQn

	#define OSD_CSYNC_PORT                      GPIOB
	#define OSD_CSYNC_PIN                       GPIO_PIN_4
	#define OSD_CSYNC_IRQ                       EXTI4_IRQn
	#endif

	#define FS_PIN_EB2_Pin          			GPIO_PIN_1
	#define FS_PIN_EB2_GPIO_Port    			GPIOC
#else
	#undef FS_PIN_EB2_Pin
	#undef FS_PIN_EB2_GPIO_Port
	#undef USE_FS_PINS
#endif


#endif /* HW_PRO58_H_ */
