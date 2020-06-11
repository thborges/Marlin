/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
#pragma once

/**
 * BLUE PILL pin assignments
 */

#if NONE(__STM32F1__, STM32F1xx, STM32F0xx)
  #error "Oops! Select an STM32 board in your IDE."
#endif

#define BOARD_INFO_NAME "Blue pill custom board"

// Enable EEPROM Emulation for this board
#define FLASH_EEPROM_EMULATION

//#define SDSS                              SS_PIN

//
// Limit Switches
//
#define X_MIN_PIN                           PB7
#define Y_MAX_PIN                           PB6
#define Z_MIN_PIN                           PB5

//
// Steppers
//
// X & Y enable are the same
#define X_STEP_PIN                          PA7
#define X_DIR_PIN                           PA6
#define X_ENABLE_PIN                        PB0

#define Y_STEP_PIN                          PA5
#define Y_DIR_PIN                           PA4
#define Y_ENABLE_PIN                        PB0

#define Z_STEP_PIN                          PC15
#define Z_DIR_PIN                           PC14
#define Z_ENABLE_PIN                        PA0

#define E0_STEP_PIN                         PB10
#define E0_DIR_PIN                          PB1
#define E0_ENABLE_PIN                       PB11

//
// Heaters and temperature sensors
//
#define HEATER_0_PIN                        PA1   // HOTEND0 MOSFET
#define TEMP_0_PIN                          PA2   // Analog Input (HOTEND0 thermistor)

// Heater 0 switched on/off by a 10V mosfet driven by a NPN transistor. There's a 100K pull-up resistor on transistor base.
// 0v = enabled
// 3-5v = disabled
#define HEATER_0_INVERTING true                     

#define HEATER_BED_PIN                      PC13   // BED MOSFET
#define TEMP_BED_PIN                        PA3   // Analog Input (BED thermistor)

//#define MALYAN_FAN1_PIN                     PB8   // FAN1 header on board - PRINT FAN

#define FAN_PIN       PB9
#define BEEPER_PIN    PB8

#define SLCD_CLK_PIN  PA8
#define SLCD_DAT_PIN  PB13
#define SLCD_DC_PIN   PB15
#define SLCD_CS_PIN   PB14

#define BTN_EN1 PB3
#define BTN_EN2 PA15
#define BTN_ENC PB4

//#define LED_PIN PC13

//#define SOFTWARE_SPI
//#define HARDWARE_SPI

// Disable DEBUG to free some IO pins, like PB3
#define DISABLE_DEBUG

// Use software PWM to prevent timers clash
#define FAN_SOFT_PWM