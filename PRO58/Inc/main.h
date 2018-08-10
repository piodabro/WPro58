/**
  ******************************************************************************
  * File Name          : main.hpp
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#if defined OSD58

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

#else
#ifndef HB5808
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define RECEIVER_SW_Pin GPIO_PIN_15
#define RECEIVER_SW_GPIO_Port GPIOC
#define RSSI_A_ADC_Pin GPIO_PIN_0
#define RSSI_A_ADC_GPIO_Port GPIOA
#define RSSI_B_ADC_Pin GPIO_PIN_1
#define RSSI_B_ADC_GPIO_Port GPIOA
#define LED_B_Pin GPIO_PIN_2
#define LED_B_GPIO_Port GPIOA
#define SPI_SLAVE_SELECT_B_Pin GPIO_PIN_3
#define SPI_SLAVE_SELECT_B_GPIO_Port GPIOA
#define SPI_SLAVE_SELECT_A_Pin GPIO_PIN_4
#define SPI_SLAVE_SELECT_A_GPIO_Port GPIOA
#define SPI_DATA_Pin GPIO_PIN_5
#define SPI_DATA_GPIO_Port GPIOA
#define SPI_CLOCK_Pin GPIO_PIN_7
#define SPI_CLOCK_GPIO_Port GPIOA
#define I2C_EEPROM_SCL_Pin GPIO_PIN_10
#define I2C_EEPROM_SCL_GPIO_Port GPIOB
#define I2C_EEPROM_SDA_Pin GPIO_PIN_11
#define I2C_EEPROM_SDA_GPIO_Port GPIOB
#define BUTTON_DOWN_Pin GPIO_PIN_12
#define BUTTON_DOWN_GPIO_Port GPIOB
#define FS_PIN_EB0_Pin GPIO_PIN_13
#define FS_PIN_EB0_GPIO_Port GPIOB
#define BUTTON_MODE_Pin GPIO_PIN_14
#define BUTTON_MODE_GPIO_Port GPIOB
#define BUTTON_UP_Pin GPIO_PIN_8
#define BUTTON_UP_GPIO_Port GPIOA
#define FS_PIN_EB1_Pin GPIO_PIN_11
#define FS_PIN_EB1_GPIO_Port GPIOA
#define I2C_DISPLAY_SCL_Pin GPIO_PIN_6
#define I2C_DISPLAY_SCL_GPIO_Port GPIOB
#define I2C_DISPLAY_SDA_Pin GPIO_PIN_7
#define I2C_DISPLAY_SDA_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOB
#define LED_A_Pin GPIO_PIN_9
#define LED_A_GPIO_Port GPIOB

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

#define FS_PIN_EB2_Pin          GPIO_PIN_1
#define FS_PIN_EB2_GPIO_Port    GPIOC
#else
#define FS_PIN_EB2_Pin          GPIO_PIN_15
#define FS_PIN_EB2_GPIO_Port    GPIOB
#endif
/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
#else

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

#endif
#endif
/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(const char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
