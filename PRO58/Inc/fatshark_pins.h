#ifndef FATSHARK_PINS_H
#define FATSHARK_PINS_H


#include <stdint.h>

#define FS_PIN_HOOKS_MAX 4

namespace FatSharkPins
{
	extern uint8_t lastState;
	extern bool valueChanged;

    typedef void(*FSChangeFunc)(uint8_t value);

	void update();
	void init();

    void registerChangeFunc(FSChangeFunc func);
    void deregisterChangeFunc(FSChangeFunc func);


}

#endif //FATSHARK_PINS
