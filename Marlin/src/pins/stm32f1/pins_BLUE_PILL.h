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
// This setting should probably be in configuration.h
// but it is literally the only board which uses it.
#define FLASH_EEPROM_EMULATION

//#define SDSS                              SS_PIN

// Based on PWM timer usage, we have to use these timers and soft PWM for the fans
// On STM32F103:
// PB3, PB6, PB7, and PB8 can be used with pwm, which rules out TIM2 and TIM4.
// On STM32F070, 16 and 17 are in use, but 1 and 3 are available.
#undef STEP_TIMER
#undef TEMP_TIMER
#define STEP_TIMER 1
#define TEMP_TIMER 3

//
// Limit Switches
//
#define X_MIN_PIN                           PB4
#define Y_MAX_PIN                           PA15
#define Z_MIN_PIN                           PB5

//
// Steppers
//
// X & Y enable are the same
#define X_STEP_PIN                          PB14
#define X_DIR_PIN                           PB8
#define X_ENABLE_PIN                        PA8

#define Y_STEP_PIN                          PB12
#define Y_DIR_PIN                           PB9
#define Y_ENABLE_PIN                        PA3

#define Z_STEP_PIN                          PB10
#define Z_DIR_PIN                           PC14
#define Z_ENABLE_PIN                        PB11

#define E0_STEP_PIN                         PB0
#define E0_DIR_PIN                          PC15
#define E0_ENABLE_PIN                       PB1

//
// Heaters and temperature sensors
//
#define HEATER_0_PIN                        PB6   // HOTEND0 MOSFET
#define TEMP_0_PIN                          PA0   // Analog Input (HOTEND0 thermistor)

// Heater 0 switched on/off by a 10V mosfet driven by a NPN transistor. There's a 100K pull-up resistor on transistor base.
// 0v = enabled
// 3-5v = disabled
#define HEATER_0_INVERTING true                     

//#define HEATER_BED_PIN                      PB7   // BED MOSFET
//#define TEMP_BED_PIN                        PA1   // Analog Input (BED thermistor)

//#define MALYAN_FAN1_PIN                     PB8   // FAN1 header on board - PRINT FAN

#define FAN_PIN       PB3
#define BEEPER_PIN    PB15

#define SLCD_CLK_PIN  PA7
#define SLCD_DAT_PIN  PA6
#define SLCD_DC_PIN   PA5
#define SLCD_CS_PIN   PA4

#define BTN_EN1 PA11
#define BTN_EN2 PA12
#define BTN_ENC PC13

//#define LED_PIN PC13

//#define SOFTWARE_SPI
//#define HARDWARE_SPI
