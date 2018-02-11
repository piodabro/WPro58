
#include <timer.h>

namespace Beeper {

	extern bool beeping;

	void init();
	void welcome();
	void beepLow(uint16_t time);
	void beepHigh(uint16_t time);
	void beepC(uint16_t time);
	void beepD(uint16_t time);
	void beepE(uint16_t time);
	void beepF(uint16_t time);
	void beepG(uint16_t time);
	void beepA(uint16_t time);
	void beepH(uint16_t time);
	void update();
}
