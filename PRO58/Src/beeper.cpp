
#include "beeper.h"
#include "tim.h"
#include "stm32f1xx_hal.h"

static Timer beeperTimer = Timer(100);

namespace Beeper
{
	bool beeping = false;

	void init(){
		HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	}

	void beepLow(uint16_t time){
		beeping = true;
		TIM4->PSC = 1637;
		TIM4->CCR3 = 50;
		beeperTimer.setDelay(time);
	}

	void beepHigh(uint16_t time){
		beeping = true;
		TIM4->PSC = 720;
		TIM4->CCR3 = 50;
		beeperTimer.setDelay(time);
	}

	void beepFreq(uint16_t time, float freq){
		beeping = true;
		uint32_t psc = 720000 / freq;
		TIM4->PSC = psc;
		TIM4->CCR3 = 50;
		beeperTimer.setDelay(time);
	}


	void beepC(uint16_t time){
		beepFreq(time, 523.251136);
	}

	void beepD(uint16_t time){
		beepFreq(time, 587.329542);
	}

	void beepE(uint16_t time){
		beepFreq(time, 659.255121);
	}

	void beepF(uint16_t time){
		beepFreq(time, 698.456470);
	}

	void beepG(uint16_t time){
		beepFreq(time, 783.990880);
	}

	void beepA(uint16_t time){
		beepFreq(time, 880.000009);
	}

	void beepH(uint16_t time){
		beepFreq(time, 987.766613);
	}

	void update(){
		if(beeperTimer.hasTicked() && beeping){
			TIM4->CCR3 = 0;
			beeping = false;
		}
	}
}
