#include "timer.h"
#include "stm32f1xx_hal.h"

Timer::Timer(uint16_t delay) {
    this->delay = delay;
    this->nextTick = HAL_GetTick() + this->delay;
    this->ticked = false;
}

const bool Timer::hasTicked() {
    if (this->ticked)
        return true;

    if (HAL_GetTick() >= this->nextTick) {
        this->ticked = true;
        return true;
    }

    return false;
}

void Timer::reset() {
    this->nextTick = HAL_GetTick() + this->delay;
    this->ticked = false;
}


void Timer::setDelay(uint16_t delay){
    this->delay = delay;
    this->nextTick = HAL_GetTick() + this->delay;
    this->ticked = false;
}
