/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * ESPExtra pin assignments
 */

#if !defined(ARDUINO_ESP_EXTRA)
  #error "Oops!  Make sure you have 'ESPExtra' selected from the 'Tools -> Boards' menu."
#endif

#define DEFAULT_MACHINE_NAME    "ESPExtra"
#define DEFAULT_SOURCE_CODE_URL "https://github.com/Hubberthus/Marlin"
#define BOARD_NAME              "ESPExtra"

#define LARGE_FLASH true

#define USBCON

//
// Limit Switches
//
#define X_MIN_PIN          D18
#define X_MAX_PIN          D19
#define Y_MIN_PIN          D20
#define Y_MAX_PIN          D21
#define Z_MIN_PIN          D22
#define Z_MAX_PIN          D23

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  D24
#endif

//
// Steppers
//
#define X_STEP_PIN         D0
#define X_DIR_PIN          D8
#define X_ENABLE_PIN       D13

#define Y_STEP_PIN         D1
#define Y_DIR_PIN          D9
#define Y_ENABLE_PIN       D14

#define Z_STEP_PIN         D2
#define Z_DIR_PIN          D10
#define Z_ENABLE_PIN       D15

#define E0_STEP_PIN        D3
#define E0_DIR_PIN         D11
#define E0_ENABLE_PIN      D16

#define E1_STEP_PIN        D4
#define E1_DIR_PIN         D12
#define E1_ENABLE_PIN      D17

//
// Temperature Sensors
//
#define TEMP_0_PIN         A1   // Analog Input
#define TEMP_1_PIN         A2   // Analog Input
#define TEMP_BED_PIN       A0   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN       D5
#define HEATER_1_PIN       D6
#define HEATER_BED_PIN     D7

#define FAN_PIN            CS_2

//
// Misc. Functions
//
#define SDSS               CS_0
#define LED_PIN            -1
#define PS_ON_PIN          -1
#define SUICIDE_PIN        -1  // PIN that has to be turned on right after start, to keep power flowing.

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)

  // No buzzer installed
  #define BEEPER_PIN -1

  // LCD Pins
  #if ENABLED(DOGLCD)

    #if ENABLED(U8GLIB_ST7920)

      #define LCD_PINS_RS     CS_1 // CS chip select /SS chip slave select
      #define LCD_PINS_ENABLE MOSI // SID (MOSI)
      #define LCD_PINS_D4     SCK // SCK (CLK) clock
      // Pin 27 is taken by LED_PIN, but Melzi LED does nothing with
      // Marlin so this can be used for BEEPER_PIN. You can use this pin
      // with M42 instead of BEEPER_PIN.
      #define BEEPER_PIN      D25

    #else // DOGM SPI LCD Support

      #define DOGLCD_A0          MOSI
      #define DOGLCD_CS          CS_1
      #define LCD_CONTRAST       1

    #endif

    // Uncomment screen orientation
    #define LCD_SCREEN_ROT_0
    //#define LCD_SCREEN_ROT_90
    //#define LCD_SCREEN_ROT_180
    //#define LCD_SCREEN_ROT_270

  #endif // !DOGLCD

  //The encoder and click button
  #define BTN_EN1                D27
  #define BTN_EN2                D28
  #define BTN_ENC                D29
  #define LCD_SDSS               CS_0

  #define SD_DETECT_PIN          D30

#elif ENABLED(MAKRPANEL)

  #define BEEPER_PIN             D25

  // Pins for DOGM SPI LCD Support
  #define DOGLCD_A0              MOSI
  #define DOGLCD_CS              CS_1
  #define LCD_BACKLIGHT_PIN      D26     // backlight LED on PA3
  // GLCD features
  #define LCD_CONTRAST           1
  // Uncomment screen orientation
  #define LCD_SCREEN_ROT_0
  //#define LCD_SCREEN_ROT_90
  //#define LCD_SCREEN_ROT_180
  //#define LCD_SCREEN_ROT_270
  //The encoder and click button
  #define BTN_EN1                D27
  #define BTN_EN2                D28
  #define BTN_ENC                D29

  #define SD_DETECT_PIN          D30

#endif // MAKRPANEL
