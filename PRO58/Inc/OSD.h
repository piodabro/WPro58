#pragma once

#ifdef USE_OSD

namespace OSD {

    enum class syncModes : uint8_t {
        automatic,
        internal,
        external,
        off
    };

    enum class videoModes : uint8_t {
        PAL,
        NTSC
    };

    void vsync_callback(void);
    void csync_callback(void);
    void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

    void setSyncMode(syncModes mode);
    void init(void);
    void clear(void);
    void print(uint8_t x, uint8_t y, const char *str);
    void copyBuffer(void);
    void printBar(uint8_t x, uint8_t y, uint8_t value);
    void enableLCD(uint8_t enabled);
    void testScreen(void);
    size_t printNumber(uint8_t x, uint8_t y, unsigned long long n, uint8_t base);
    void update();

}
#endif
