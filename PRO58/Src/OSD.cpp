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

#include <spi.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "tim.h"

#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_dma.h>
#include <stm32f103xb.h>

#include "settings_eeprom.h"
#include "receiver.h"
#include "receiver_spi.h"
#include "channels.h"
#include "gpio.h"
#include "OSD.h"
#include "OSD_font.h"

#define FIRST_LINE                  41  //This should become a setting to adjust to used goggles/monitor
#define OSD_ROWS                    6
#define OSD_COLUMNS                 18
#define LINEBUFFER_SIZE             72
#define OSD_LCDWIDTH                128
#define OSD_LCDHEIGHT               64
#define FONT_HEIGHT                 18
#define VIDEO_BUFFER_CHARS          480
#define VIDEO_LINES_NTSC            13
#define VIDEO_LINES_PAL             16
#define LOSTSYNC_COUNTER            30
#define SYNC_COUNTER                2

//#define DEBUG_OUT

extern uint8_t buffer[];

#ifdef OSD_VSYNC_IRQ
#define ENABLE_VSYNC_IRQ()      HAL_NVIC_EnableIRQ(OSD_VSYNC_IRQ)
#define DISABLE_VSYNC_IRQ()     HAL_NVIC_DisableIRQ(OSD_VSYNC_IRQ)
#else
#define ENABLE_VSYNC_IRQ()
#define DISABLE_VSYNC_IRQ()
#endif

#ifdef USE_OSD

namespace OSD {

    static uint8_t screenBuffer[VIDEO_BUFFER_CHARS];

    static volatile uint16_t line = 0;
    static volatile uint16_t firstLine = 625;
    static volatile uint16_t lastLine = 625;
    static volatile uint16_t firstLineLCD = 625;
    static volatile uint16_t lastLineLCD = 625;
    static volatile uint16_t lineCounter = 0;
    static volatile uint16_t internalLine = 0;
    static volatile uint8_t videoConnect = 0;
    static volatile videoModes videoMode = videoModes::PAL;

    static volatile uint8_t ntscSync = 4;
    static volatile uint8_t palSync = 4;
    static volatile uint8_t lostSync = LOSTSYNC_COUNTER;

    static uint8_t OSDbuffer[OSD_LCDWIDTH * OSD_LCDHEIGHT / 8];

    uint8_t linebufferA[2][LINEBUFFER_SIZE] = {0};
    uint8_t linebufferB[2][LINEBUFFER_SIZE] = {0};

    static syncModes syncMode;

#ifdef DEBUG_OUT
    static volatile uint16_t debug_line = 0;
    static volatile uint16_t debug_internalLine = 0;
    static volatile uint16_t debug2 = 0;
#endif

    typedef struct _uint32u16u8_t {
        union {
        uint32_t dbword;
        uint16_t word[2];
        uint8_t byte[4];
        };

    } uint32u16u8_t;


    void vsync_callback(void) {
        static uint32_t oldTick = 0;
        uint32_t actTick = HAL_GetTick();
        uint32_t difTick = actTick - oldTick;


        if(!videoConnect) {
            if (difTick > 15 && difTick < 18) {
                if(ntscSync) {
                    ntscSync--;
                }
            } else {
                if(ntscSync < SYNC_COUNTER) {
                    ntscSync++;
                }
            }

            if (difTick > 18 && difTick < 22) {
                if(palSync) {
                    palSync--;
                }
            } else {
                if(palSync < SYNC_COUNTER) {
                    palSync++;
                }
            }
        }

        oldTick = actTick;

        if(!ntscSync) {
            if(difTick > 14) {
                line = 1;
                internalLine = 1;
                if(lostSync < LOSTSYNC_COUNTER) {
                    lostSync++;
                }
            }
            if(!videoConnect) {
                videoMode = videoModes::NTSC;
                videoConnect = 1;
            }
        }

        if(!palSync) {
            if(difTick > 18) {
                line = 1;
                internalLine = 1;
                if(lostSync < LOSTSYNC_COUNTER) {
                    lostSync++;
                }
            }
            if(!videoConnect) {
                videoMode = videoModes::PAL;
                videoConnect = 1;
            }
        }

    }

    void __attribute__((optimize("O3"))) csync_callback(void) {
        static uint8_t cBuffer = 0;

        if((line >= firstLine ) && (line <= lastLine)) {

            DMA_Channel_TypeDef* dmaSpiA = OSD_DMA_SPI_A;

            dmaSpiA->CCR = 0;
            dmaSpiA->CNDTR = 33;
            dmaSpiA->CMAR = (uint32_t)linebufferA[cBuffer];
            uint32_t ccr = dmaSpiA->CCR | DMA_CCR_EN;

#ifdef OSD_SPI_B
            DMA_Channel_TypeDef* dmaSpiB = OSD_DMA_SPI_B;

            dmaSpiB->CCR = 0;
            dmaSpiB->CNDTR = 33;
            dmaSpiB->CMAR = (uint32_t)linebufferB[cBuffer];

            dmaSpiB->CCR = ccr;
#endif
            dmaSpiA->CCR = ccr;

            cBuffer = 1 - cBuffer;

            if( line < lastLine) {
                uint16_t fontRow = ((line - FIRST_LINE) % FONT_HEIGHT);
                uint32u16u8_t ch1 = {0};
                uint32u16u8_t ch2 = {0};
                uint16_t row = (line - FIRST_LINE) / FONT_HEIGHT;
                uint16_t buf = 3;
                uint16_t screenBufferCounter = row * OSD_COLUMNS;

                if(videoConnect == 2) {
                    for(unsigned col=0; col<OSD_COLUMNS; col +=2) {
                        ch1.dbword = fontdata[screenBuffer[screenBufferCounter++] * (FONT_HEIGHT) + fontRow];
                        if(col < OSD_COLUMNS-1) {
                            ch2.dbword = (fontdata[screenBuffer[screenBufferCounter++] * (FONT_HEIGHT) + fontRow])>>4;
                        } else {
                            ch2.dbword = 0x0000;
                        }

#ifdef OSD_SPI_B
                        linebufferA[cBuffer][buf]   = ch1.byte[3];
                        linebufferA[cBuffer][buf+1] = ch1.byte[2] | ch2.byte[3];
                        linebufferA[cBuffer][buf+2] = ch2.byte[2];

                        linebufferB[cBuffer][buf]   = ch1.byte[1];
                        linebufferB[cBuffer][buf+1] = ch1.byte[0] | ch2.byte[1];
                        linebufferB[cBuffer][buf+2] = ch2.byte[0];
#else
                        linebufferA[cBuffer][buf]   = ch1.byte[1];
                        linebufferA[cBuffer][buf+1] = ch1.byte[0] | ch2.byte[1];
                        linebufferA[cBuffer][buf+2] = ch2.byte[0];
#endif
                        buf +=3;
                    }
                } else {
                    buf += 2;
                    for(unsigned col=0; col<OSD_COLUMNS; col +=2) {
                        ch1.dbword = fontdata[screenBuffer[screenBufferCounter++] * (FONT_HEIGHT) + fontRow];
                        ch2.dbword = (fontdata[screenBuffer[screenBufferCounter++] * (FONT_HEIGHT) + fontRow])>>4;


#ifdef OSD_SPI_B
                        linebufferB[cBuffer][buf]   = ch1.byte[1];
                        linebufferB[cBuffer][buf+1] = ch1.byte[0] | ch2.byte[1];
                        linebufferB[cBuffer][buf+2] = ch2.byte[0];
#else
                        linebufferA[cBuffer][buf]   = ch1.byte[1];
                        linebufferA[cBuffer][buf+1] = ch1.byte[0] | ch2.byte[1];
                        linebufferA[cBuffer][buf+2] = ch2.byte[0];
#endif
                        buf +=3;
                    }
                }
                lineCounter++;

            } else {
                uint8_t buf = LINEBUFFER_SIZE;
#ifdef OSD_SPI_B
                uint8_t alpha = 0xff;
#else
                uint8_t alpha = 0x00;
#endif
                if(videoConnect == 2) {
                    alpha=0x00;
                }
                while(buf--) {
                    linebufferA[0][buf] = alpha;
                    linebufferB[0][buf] = 0;
                    linebufferA[1][buf] = alpha;
                    linebufferB[1][buf] = 0;
                }
                lineCounter = 0;
            }
        }

        if((line >= firstLineLCD ) && (line <= lastLineLCD+1)) {
            DMA_Channel_TypeDef* dmaSpiA = OSD_DMA_SPI_A;

            dmaSpiA->CCR = 0;
            dmaSpiA->CNDTR = 33;
            dmaSpiA->CMAR = (uint32_t)linebufferA[cBuffer];
            uint32_t ccr = dmaSpiA->CCR | DMA_CCR_EN;

#ifdef OSD_SPI_B
            DMA_Channel_TypeDef* dmaSpiB = OSD_DMA_SPI_B;

            dmaSpiB->CCR = 0;
            dmaSpiB->CNDTR = 33;
            dmaSpiB->CMAR = (uint32_t)linebufferB[cBuffer];

            dmaSpiB->CCR = ccr;
#endif
            dmaSpiA->CCR = ccr;

            cBuffer = 1 - cBuffer;

            if( line < lastLineLCD) {;
                uint16_t buf = 9;

                if(videoConnect == 2) {
                    uint8_t value;
                    for(unsigned col=0; col<16; col++) {
                        value = OSDbuffer[lineCounter * 16  + col];
                        linebufferA[cBuffer][buf] = value;
                        linebufferB[cBuffer][buf++] = 0xff;
                    }

                } else {
                    buf += 2;
                    for(unsigned col=0; col<16; col++) {
                        uint8_t value;
                        value = OSDbuffer[lineCounter * 16  + col];
#ifdef OSD_SPI_B
                        linebufferB[cBuffer][buf++] = value;
#else
                        linebufferA[cBuffer][buf++] = value;
#endif
                    }

                }
                lineCounter++;

            } else {
                uint8_t buf = LINEBUFFER_SIZE;
#ifdef OSD_SPI_B
                uint8_t alpha = 0xff;
#else
                uint8_t alpha = 0x00;
#endif
                if(videoConnect == 2) {
                    alpha=0x00;
                }
                while(buf--) {
                    linebufferA[cBuffer][buf] = alpha;
                    linebufferB[cBuffer][buf] = 0;
                }
                lineCounter = 0;
            }
        }

        line += 1;

        if(line == 260) {
            SPI1->CR1 &= ~(SPI_CR1_SPE);
        }

        if(line == 265) {
            SPI1->CR1 |= SPI_CR1_SPE;
#ifdef DEBUG_OUT
            debug_line = line;
            debug_internalLine = internalLine;
#endif
        }

    }


    void __attribute__((optimize("O3"))) TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
        DMA_Channel_TypeDef* dmaSpiA = OSD_DMA_SPI_A;
        uint8_t buf = LINEBUFFER_SIZE;

#ifdef DEBUG_OUT
        screenBuffer[24] = '0'+videoConnect;
        screenBuffer[26] = '0'+palSync;
        screenBuffer[28] = '0'+ntscSync;
        screenBuffer[30] = '0'+lostSync;
#endif

        switch(videoConnect){
        case 0:
            break;

        case 1:
#ifdef OSD_CSYNC_IRQ
            HAL_NVIC_EnableIRQ(OSD_CSYNC_IRQ);
#endif
            TIM1->OSD_TIM_CCR = 0;
            if(videoMode == videoModes::PAL) {
                TIM1->ARR = 512;
            } else {
                TIM1->ARR = 508;
            }
            internalLine = line;
            videoConnect++;
            lostSync = LOSTSYNC_COUNTER;
            return;

        case 2:
            internalLine++;
            if(internalLine == 7) {
                internalLine += 6;
            } else if(internalLine == 50) {
                if((line < 48) || (line > 52)) {
                    lostSync--;
                }
            } else if(internalLine == 100) {
                if((line < 98) || (line > 102)) {
                    lostSync--;
                }
            } else if(internalLine == 150) {
                if((line < 148) || (line > 152)) {
                    lostSync--;
                }
            } else if ((videoMode == videoModes::PAL) && (internalLine > 323)){
                internalLine = 1;
            } else if ((videoMode == videoModes::NTSC) && (internalLine > 272)){
                internalLine = 1;
            }
            if(!lostSync) {
                videoConnect++;
            }
            return;

        case 3:
#ifdef OSD_CSYNC_IRQ
            HAL_NVIC_DisableIRQ(OSD_CSYNC_IRQ);
#endif
            while(buf--) {
                linebufferA[0][buf] = 0x00;
            }
            dmaSpiA->CCR = 0;
            dmaSpiA->CNDTR = 1;
            dmaSpiA->CMAR = (uint32_t)linebufferA[0];
            dmaSpiA->CCR |= DMA_CCR_EN;
            
            videoConnect = 0;
            palSync = SYNC_COUNTER;
            ntscSync = SYNC_COUNTER;
#ifdef DEBUG_OUT
            debug2++;
#endif
            return;
        }

        if(syncMode >= syncModes::external) return;

        if(videoMode == videoModes::PAL) {
            internalLine++;
            if(internalLine == 322) {
                internalLine = 1;
            }

            //uint8_t d = 3;
            //while (d--) asm("NOP");

            csync_callback();

            switch(internalLine) {
            case 316:
                TIM1->ARR = 256;
                TIM1->OSD_TIM_CCR = 16;
                break;
            case 320:
                TIM1->OSD_TIM_CCR = 218;
                break;
            case 1:
                line = 1;
                break;
            case 5:
                TIM1->OSD_TIM_CCR = 16;
                break;
            case 10:
                TIM1->OSD_TIM_CCR = 32;
                break;
            case 11:
                TIM1->ARR = 512;
                break;
            }
        } else {

            internalLine++;
            if(internalLine == 273) {
                internalLine = 1;
            }

            //uint8_t d = 3;
            //while (d--) asm("NOP");

            csync_callback();

            switch(internalLine) {
            case 265:
                TIM1->OSD_TIM_CCR = 16;
                break;
            case 266:
                TIM1->ARR = 254;
                break;
            case 271:
                TIM1->OSD_TIM_CCR = 218;
                break;
            case 1:
                line = 1;
                break;
            case 5:
                TIM1->OSD_TIM_CCR = 16;
                break;
            case 11:
                TIM1->OSD_TIM_CCR = 32;
                break;
            case 12:
                TIM1->ARR = 508;
                break;
            }
        }

    }

    void setSyncMode(syncModes mode) {

        if(!EepromSettings.OSDEnabled) {
            mode = syncModes::off;
        }

        if(mode == syncMode) {
            return;
        }

        switch(mode) {
        case syncModes::automatic:
#ifdef OSD_SPI_B
            syncMode = syncModes::automatic;
            ENABLE_VSYNC_IRQ();
#else
            syncMode = syncModes::internal;
            DISABLE_VSYNC_IRQ();
            ReceiverSpi::setPowerDownRegister(0b01010000110000010011);  //Deactivate video amp power
            GPIO_Reinit(OSD_SYNC_OUT_PORT, OSD_SYNC_OUT_PIN, GPIO_MODE_AF_PP);
            GPIO_Reinit(OSD_SPI_A_PORT, OSD_SPI_A_PIN, GPIO_MODE_AF_PP);
#endif
            videoConnect = 2;
            break;
        case syncModes::external:
#ifndef OSD_SPI_B
            ReceiverSpi::setPowerDownRegister(0b00010000110000010011);  //Activate video amp power
            GPIO_Reinit(OSD_SYNC_OUT_PORT, OSD_SYNC_OUT_PIN, GPIO_MODE_INPUT);
#ifdef OSD_CSYNC_PIN
            GPIO_Reinit(OSD_SPI_A_PORT, OSD_SPI_A_PIN, GPIO_MODE_AF_PP);
#else
            GPIO_Reinit(OSD_SPI_A_PORT, OSD_SPI_A_PIN, GPIO_MODE_INPUT);
#endif
#endif
            ENABLE_VSYNC_IRQ();
            TIM1->OSD_TIM_CCR = 0;
            videoConnect = 2;
            syncMode = syncModes::external;
            break;
        case syncModes::internal:
#ifndef OSD_SPI_B
            ReceiverSpi::setPowerDownRegister(0b01010000110000010011);  //Deactivate video amp power
            GPIO_Reinit(OSD_SYNC_OUT_PORT, OSD_SYNC_OUT_PIN, GPIO_MODE_AF_PP);
            GPIO_Reinit(OSD_SPI_A_PORT, OSD_SPI_A_PIN, GPIO_MODE_AF_PP);
#endif
            DISABLE_VSYNC_IRQ();
            videoConnect = 2;
            syncMode = syncModes::internal;
            break;
        case syncModes::off:
#ifndef OSD_SPI_B
            ReceiverSpi::setPowerDownRegister(0b00010000110000010011);  //Activate video amp power
            GPIO_Reinit(OSD_SYNC_OUT_PORT, OSD_SYNC_OUT_PIN, GPIO_MODE_INPUT);
            GPIO_Reinit(OSD_SPI_A_PORT, OSD_SPI_A_PIN, GPIO_MODE_INPUT);
#endif
            DISABLE_VSYNC_IRQ();
            TIM1->OSD_TIM_CCR = 0;
            videoConnect = 2;
            syncMode = syncModes::off;
            break;
        }

    }

    void init(void)
    {
        firstLine = FIRST_LINE;
        videoMode = EepromSettings.OSDDefaultMode;

        enableLCD(false);

        clear();

        HAL_Delay(100);
        spi_send_DMA(OSD_SPI_A,linebufferB[0],1);
        HAL_Delay(100);
#ifdef OSD_SPI_B
        spi_send_DMA(OSD_SPI_B,linebufferA[0],1);
        HAL_Delay(100);
        spi_send_DMA(OSD_SPI_A,linebufferB[0],1);
        HAL_Delay(100);
#endif

#ifdef OSD_VSYNC_PIN
        GPIO_InitTypeDef GPIO_InitStruct;

        //Configure vsync pin
        GPIO_InitStruct.Pin = OSD_VSYNC_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(OSD_VSYNC_PORT, &GPIO_InitStruct);
        HAL_NVIC_SetPriority(OSD_VSYNC_IRQ, 2, 1);


        //Configure csync pin
        GPIO_InitStruct.Pin = OSD_CSYNC_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(OSD_CSYNC_PORT, &GPIO_InitStruct);
        HAL_NVIC_SetPriority(OSD_CSYNC_IRQ, 0, 0);
#endif

        setSyncMode(syncModes::external);
        HAL_NVIC_EnableIRQ(OSD_TIM_IRQ);

    }

    void enableLCD(uint8_t enabled) {
        if(enabled) {
            lastLine = firstLine + FONT_HEIGHT * 5;
            firstLineLCD = lastLine + 5;
            lastLineLCD = firstLineLCD + OSD_LCDHEIGHT;
        } else {
            firstLineLCD = 625;
            lastLineLCD = 625;
            lastLine = firstLine + FONT_HEIGHT * OSD_ROWS;
        }
    }

    void clear(void) {
        for(uint16_t x = 0; x < VIDEO_BUFFER_CHARS; x++ ) {
            screenBuffer[x] = 0x20;
        }
    }

    void print(uint8_t x, uint8_t y, const char *str) {
        uint16_t pos = x + y * OSD_COLUMNS;

        while (*str) {
            screenBuffer[pos++] = *str++;
        }
    }

    void print(uint8_t x, uint8_t y, uint8_t c) {
        uint16_t pos = x + y * OSD_COLUMNS;

        screenBuffer[pos++] = c;
    }

    void printBar(uint8_t x, uint8_t y, uint8_t value) {
        uint16_t pos = x + y * OSD_COLUMNS;
        uint16_t tempValue = value * 3 + 12;

        screenBuffer[pos++] = 0xed;
        for(x=0; x<2;x++) {
            if(tempValue < 25) {
                screenBuffer[pos++] = 0xA0;
            } else if(tempValue >=150) {
                screenBuffer[pos++] = 0xee;
                tempValue -= 150;
            } else {
                screenBuffer[pos++] = 0xA0 + (tempValue/25);
                tempValue = 0;
            }
        }
        screenBuffer[pos++] = 0xf3;
    }

    //TODO: maybe use DMA???
    void copyBuffer(void) {
        uint16_t mask;
        uint8_t x2;
        memset(OSDbuffer, 0, (OSD_LCDWIDTH*OSD_LCDHEIGHT/8));

        for(uint8_t y = 0; y<64; y++){
            for(uint8_t x=0; x<16; x++) {
                mask = 0x80;
                x2 = 0;
                while(mask > 0) {
                    if(buffer[x*8+x2+ (y/8)*OSD_LCDWIDTH] & (1 << (y&7))) {
                        OSDbuffer[(x)+ y*OSD_LCDWIDTH/8] |= mask;
                    }
                    mask >>= 1;
                    x2++;
                }
            }
        }
    }

    void testScreen(void) {
        for(uint16_t x = 0; x < VIDEO_BUFFER_CHARS; x++ ) {
            screenBuffer[x] = x & 0xff;
        }
    }

    size_t printNumber(uint8_t x, uint8_t y, unsigned long long n, uint8_t base) {
        unsigned char buf[8 * sizeof(long long)];
        unsigned long i = 0;
        uint16_t pos = x + y * OSD_COLUMNS;

        size_t s=0;
        if (n == 0) {
            screenBuffer[pos] = '0';
            return 1;
        }

        while (n > 0) {
            buf[i++] = n % base;
            n /= base;
        }

        for (; i > 0; i--) {
            s += screenBuffer[pos++] = ((char)(buf[i - 1] < 10 ?
                         '0' + buf[i - 1] :
                         'A' + buf[i - 1] - 10));
        }
        return s;
    }

    void update() {
        static uint32_t lastTick=0;

        if((HAL_GetTick() - lastTick) > 100) {
            lastTick = HAL_GetTick();
#ifdef DEBUG_OUT
            print(1,1, "   ");
            print(1,2 ,"   ");
            print(6,2 ,"   ");
            printNumber(1,1, debug_line, 10);
            printNumber(1,2, debug_internalLine, 10);
            printNumber(6,2, debug2, 10);
#endif

            if(!EepromSettings.OSDEnabled) {
                if(syncMode != syncModes::off) {
                    setSyncMode(syncModes::off);
                }
                return;
            } else {
                if(syncMode == syncModes::off) {
                    setSyncMode(syncModes::automatic);
                }
            }

            if(EepromSettings.OSDShowRssi) {
                if(EepromSettings.diversityMode == Receiver::DiversityMode::AUTO || EepromSettings.diversityMode == Receiver::DiversityMode::FORCE_A){
                    printBar(0,0,Receiver::rssiA);
                    if(EepromSettings.diversityMode == Receiver::DiversityMode::AUTO)
                        print(0,0, 0xA9 + (uint8_t)Receiver::activeReceiver);
                } else {
                    print(0,0,"    ");
                }

                if(EepromSettings.diversityMode == Receiver::DiversityMode::AUTO || EepromSettings.diversityMode == Receiver::DiversityMode::FORCE_B){
                    printBar(OSD_COLUMNS-4,0,Receiver::rssiB);
                    if(EepromSettings.diversityMode == Receiver::DiversityMode::AUTO)
                        print(OSD_COLUMNS-1,0, 0xA8 - (uint8_t)Receiver::activeReceiver);
                } else {
                    print(OSD_COLUMNS-4,0,"    ");
                }

            } else {
                print(0,0,"    ");
                print(OSD_COLUMNS-4,0,"    ");
            }

            if(EepromSettings.OSDShowChannel) {
                print(4,0,0xB0 + Receiver::activeChannel);
            } else {
                print(4,0,' ');
            }

            if(EepromSettings.OSDShowFrequency) {
                printNumber(7,0,Channels::getFrequency(Receiver::activeChannel),10);
            } else {
                print(7,0,"    ");
            }

        }
    }

}
#endif

