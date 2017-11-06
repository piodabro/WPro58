/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

#include "settings_eeprom.h"

#include "channels.h"
#include "receiver.h"
#include "receiver_spi.h"

#include "buttons.h"
#include "state.h"
#include "ui.h"

#include "DWT_Delay.h"
#include "i2c_reset.h"

#include "beeper.h"

#include "logo.h"

#include "fatshark_pins.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void globalMenuButtonHandler(
  Button button,
  Buttons::PressType pressType
);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  HAL_Delay(200);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();

  /* USER CODE BEGIN 2 */

  if(DWT_Delay_Init()){
	_Error_Handler(__FILE__,__LINE__); /* Call Error Handler */
  }

  HAL_GPIO_WritePin(RECEIVER_SW_GPIO_Port,RECEIVER_SW_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SPI_SLAVE_SELECT_A_GPIO_Port,SPI_SLAVE_SELECT_A_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(SPI_SLAVE_SELECT_B_GPIO_Port,SPI_SLAVE_SELECT_B_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(SPI_CLOCK_GPIO_Port,SPI_CLOCK_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SPI_DATA_GPIO_Port,SPI_DATA_Pin,GPIO_PIN_RESET);

  HAL_Delay(100); //Delay 1000ms to allow RX5808 startup.

  I2C_Reset(hi2c1, MX_I2C1_Init);
  Ui::setup(&hi2c1);

  Ui::display.drawBitmap(
		0,
		0,
		logo,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		WHITE
  );
  Ui::display.display();

  Beeper::init();
  Beeper::beepC(200); //welcome beeep ;)
  while(Beeper::beeping){
    Beeper::update();
  }
  Beeper::beepE(200); //welcome beeep ;)
  while(Beeper::beeping){
    Beeper::update();
  }
  Beeper::beepG(200); //welcome beeep ;)
  while(Beeper::beeping){
    Beeper::update();
  }
  HAL_Delay(200);
  Beeper::beepE(200); //welcome beeep ;)
  while(Beeper::beeping){
    Beeper::update();
  }
  Beeper::beepG(400); //welcome beeep ;)
  while(Beeper::beeping){
    Beeper::update();
  }

  HAL_Delay(200);
  I2C_Reset(hi2c2, MX_I2C2_Init);
  EepromSettings.init(&hi2c2);
  EepromSettings.load();
  Receiver::setup(&hadc1);
  Receiver::setChannel(EepromSettings.startChannel);

  FatSharkPins::init();

  StateMachine::setup();

  Receiver::setActiveReceiver(Receiver::ReceiverId::A);

  Buttons::registerChangeFunc(globalMenuButtonHandler);
  // Switch to initial state.
  StateMachine::switchState(StateMachine::State::SEARCH);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  Beeper::update();
	  Receiver::update();
	  Buttons::update();
	  FatSharkPins::update();
	  StateMachine::update();
	  Ui::update();
	  EepromSettings.update();

	  if (
//	    StateMachine::currentState != StateMachine::State::SCREENSAVER
//	    && StateMachine::currentState != StateMachine::State::BANDSCAN
//		&& StateMachine::currentState != StateMachine::State::SETTINGS_RSSI
	    StateMachine::currentState == StateMachine::State::SEARCH
	    && (HAL_GetTick() - Buttons::lastChangeTime) >
	    (SCREENSAVER_TIMEOUT * 1000)
	  ) {
	    StateMachine::switchState(StateMachine::State::SCREENSAVER);
	  }

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
static void globalMenuButtonHandler(
  Button button,
  Buttons::PressType pressType
) {
  if (
    StateMachine::currentState != StateMachine::State::MENU &&
    button == Button::MODE &&
    pressType == Buttons::PressType::HOLDING
  ) {
    StateMachine::switchState(StateMachine::State::MENU);
  }

}
/* USER CODE END 4 */

#ifdef __cplusplus
 extern "C" {
#endif
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(const char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

#ifdef __cplusplus
 }
#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
