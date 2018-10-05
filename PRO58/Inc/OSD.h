/*
Copyright (c) 2018 Telekatz

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

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
    void print(uint8_t x, uint8_t y, uint8_t c);
    void copyBuffer(void);
    void printBar(uint8_t x, uint8_t y);
    void enableLCD(uint8_t enabled);
    void testScreen(void);
    size_t printNumber(uint8_t x, uint8_t y, unsigned long long n, uint8_t base);
    void update();

}
#endif
