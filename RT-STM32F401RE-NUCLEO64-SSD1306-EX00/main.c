/*
    ChibiOS Demos
    Copyright (C) 2024  Domenico Rega [dodorega@gmail.com]

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
 * [SSD1306] SSD1306 OLED Display Example 01
 * A simple example with the SSD1306 OLED display. This demo writes some text
 * on an oled display based on the ssd1306 driver. The display I2C should be
 * connected on PB8 and PB9.
 */

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#include "ssd1306.h"
#include "stdio.h"

#define BUFF_SIZE   20
char buff[BUFF_SIZE];

static const I2CConfig i2ccfg = {
  OPMODE_I2C,
  400000,
  FAST_DUTY_CYCLE_2,
};

static const SSD1306Config ssd1306cfg = {
  &I2CD1,
  &i2ccfg,
  SSD1306_SAD_0X78,
};

static SSD1306Driver SSD1306D1;

static THD_WORKING_AREA(waOledDisplay, 512);
static THD_FUNCTION(OledDisplay, arg) {
  (void)arg;

  chRegSetThreadName("OledDisplay");

  /*
   * Initialize SSD1306 Display Driver Object.
   */
  ssd1306ObjectInit(&SSD1306D1);

  /*
   * Start the SSD1306 Display Driver Object with
   * configuration.
   */
  ssd1306Start(&SSD1306D1, &ssd1306cfg);

  ssd1306FillScreen(&SSD1306D1, 0x00);

  while (true) {

    ssd1306GotoXy(&SSD1306D1, 0, 1);
    chsnprintf(buff, BUFF_SIZE, "ChibiOS");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);

    ssd1306GotoXy(&SSD1306D1, 0, 20);
    chsnprintf(buff, BUFF_SIZE, "Demos");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_BLACK);

    ssd1306UpdateScreen(&SSD1306D1);
    chThdSleepMilliseconds(500);
  }

}

int main(void) {

  halInit();
  chSysInit();

  /* Configuring I2C related PINs */
   palSetLineMode(LINE_ARD_D15, PAL_MODE_ALTERNATE(4) |
                  PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST |
                  PAL_STM32_PUPDR_PULLUP);
   palSetLineMode(LINE_ARD_D14, PAL_MODE_ALTERNATE(4) |
                  PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST |
                  PAL_STM32_PUPDR_PULLUP);

  chThdCreateStatic(waOledDisplay, sizeof(waOledDisplay), NORMALPRIO, OledDisplay, NULL);

  /*Infinite loop*/
  while (true) {
    uint32_t flag = 0;

    if(palReadLine(LINE_BUTTON) == PAL_LOW) {
      while(palReadLine(LINE_BUTTON) == PAL_LOW){
        chThdSleepMilliseconds(20);
      }
      flag = 1;
    }

    if (flag == 1) {
      ssd1306GotoXy(&SSD1306D1, 0, 36);
      chsnprintf(buff, BUFF_SIZE, "2020");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
      ssd1306UpdateScreen(&SSD1306D1);
    }

    chThdSleepMilliseconds(50);
  }
}

/*
 * 	Simple code to turn off Oled Display
 *
    uint8_t txbuf[] = { 0x00, 0xAE };
    i2cAcquireBus(&I2CD1);
    i2cStart(&I2CD1, &i2ccfg);
    i2cMasterTransmitTimeout(&I2CD1, SSD1306_SAD_0X78, txbuf, sizeof(txbuf), NULL, 0, TIME_INFINITE);
    i2cReleaseBus(&I2CD1);
 *
 *
 */

