#ifndef UI_H
#define UI_H

#include <Adafruit_GFX_AS.h>
#include "SSD1306_STM32.h"

#define OLED_RESET -1

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SCREEN_WIDTH_MID ((SCREEN_WIDTH / 2) - 1)
#define SCREEN_HEIGHT_MID ((SCREEN_HEIGHT / 2) - 1)

#define CHAR_WIDTH 5
#define CHAR_HEIGHT 7

namespace Ui {
    extern SSD1306 display;
    extern bool shouldDrawUpdate;
    extern bool shouldDisplay;
    extern bool shouldFullRedraw;

    void setup(I2C_HandleTypeDef *i2c_handle);
    void update();

    void drawGraph(
        const uint8_t data[],
        const uint8_t dataSize,
        const uint8_t dataScale,
        const uint8_t x,
        const uint8_t y,
        const uint8_t w,
        const uint8_t h
    );

    void drawDashedHLine(const int x, const int y, const int w, const int step);
    void drawDashedVLine(const int x, const int y, const int w, const int step);

    void clear();
    void clearRect(const int x, const int y, const int w, const int h);

    void needUpdate();
    void needDisplay();
    void needFullRedraw();
}

#endif // file_defined


