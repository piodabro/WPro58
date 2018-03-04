#ifdef USE_FS_PINS

#include "fatshark_pins.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "beeper.h"
#include "settings_eeprom.h"

static FatSharkPins::FSChangeFunc changeFuncs[FS_PIN_HOOKS_MAX] = { nullptr };

namespace FatSharkPins {

	uint8_t lastState = 0;
	bool valueChanged = false;

	static void updatePins();
	static void runChangeFuncs(uint8_t newState);


	void init(){
		lastState = (
						static_cast<uint8_t>(HAL_GPIO_ReadPin(FS_PIN_EB0_GPIO_Port,FS_PIN_EB0_Pin)) |
						static_cast<uint8_t>(HAL_GPIO_ReadPin(FS_PIN_EB1_GPIO_Port,FS_PIN_EB1_Pin)) << 1 |
						static_cast<uint8_t>(HAL_GPIO_ReadPin(FS_PIN_EB2_GPIO_Port,FS_PIN_EB2_Pin)) << 2
					) & 0x07;
	}

	void update(){
		updatePins();
	}

	void registerChangeFunc(FSChangeFunc func) {
		for (uint8_t i = 0; i < FS_PIN_HOOKS_MAX; i++) {
			if (changeFuncs[i] == nullptr) {
				changeFuncs[i] = func;
				return;
			}
		}
	}

	void deregisterChangeFunc(FSChangeFunc func) {
		for (uint8_t i = 0; i < FS_PIN_HOOKS_MAX; i++) {
			if (changeFuncs[i] == func) {
				changeFuncs[i] = nullptr;
				return;
			}
		}
	}

	static void runChangeFuncs(uint8_t newState){
		if(EepromSettings.beepEnabled) Beeper::beepHigh(50);
		for (uint8_t i = 0; i < FS_PIN_HOOKS_MAX; i++) {
			if (changeFuncs[i] != nullptr) {
				changeFuncs[i](newState);
			}
		}
	}

    static void updatePins(){
    	uint8_t value = (
						static_cast<uint8_t>(HAL_GPIO_ReadPin(FS_PIN_EB0_GPIO_Port,FS_PIN_EB0_Pin)) |
						static_cast<uint8_t>(HAL_GPIO_ReadPin(FS_PIN_EB1_GPIO_Port,FS_PIN_EB1_Pin)) << 1 |
						static_cast<uint8_t>(HAL_GPIO_ReadPin(FS_PIN_EB2_GPIO_Port,FS_PIN_EB2_Pin)) << 2
					) & 0x07;

    	if(lastState != value){
    		runChangeFuncs(value);
    		lastState = value;
    	}
    }
}

#endif
