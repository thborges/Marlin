/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * PCD8544 LCD implementation
 *
 * Graphic screen implementation for common NOKIA 5110 LCDs
 * by Thiago Borges de Oliveira, @thborges, May 2020
 */

#pragma once

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(NOKIA5110_LCD)

#define CUSTOM_STATUS_SCREEN

#include "../../libs/numtostr.h"
#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"
#include "../../module/planner.h"
#include "../menu/menu.h"
#include "nokia5110_ga.h"

#if ENABLED(SDSUPPORT)
  #include "../../libs/duration_t.h"
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #if DISABLED(FILAMENT_LOAD_UNLOAD_GCODES)
  #error "Please enable FILAMENT_LOAD_UNLOAD_GCODES in Configuration_adv.h"
  #endif

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #include "../../feature/runout.h"
  #endif
#endif

typedef enum { 
  S_NONE,
  S_MENU,
  S_PRINT,
  S_PAUSE,
  S_STOP,
  S_AUTOHOME,
  S_PRHEAT1,
  S_PRHEAT2,
  S_COOLDOWN,
  #if HAS_FILAMENT_SENSOR
  S_FILAMENT,
  #endif
  S_NOZZLE1,
  #if HOTENDS > 1
  S_NOZZLE2,
  #endif
  #if HAS_HEATED_BED
  S_BED,
  #endif
  #if ENABLED(BABYSTEPPING)
  S_ZBABYSTEPPING,
  #endif
  S_FEEDRATE,
  S_FLOW,
  #if HAS_FAN0
  S_FAN,
  #endif
  S_Z,
  S_X,
  S_Y,
  S_LAST,
} selected_item;

selected_item menu_selected = S_NONE;

NOKIA5110GA lcd;

bool MarlinUI::detected() { return true; }
void MarlinUI::set_font(const MarlinFont font_nr) { }

#define ICON_WIDTH 8

const uint8_t nozzle_icon[] PROGMEM = {
0b00000000,
0b00011000,
0b00100111,
0b01000001,
0b01000001,
0b00100111,
0b00011000,
0b00000000,
};

const uint8_t bed_icon[] PROGMEM = {
0b01100000,
0b00101010,
0b00100101,
0b00100000,
0b00101010,
0b00100101,
0b01100000,
0b00000000,
};

const uint8_t fan_icon[] PROGMEM = {
0b00000110,
0b01001100,
0b01101000,
0b00111110,
0b00001011,
0b00011001,
0b00110000,
0b00000000,
};

const uint8_t feedrate_icon[] PROGMEM = {
0b01100011,
0b01110001,
0b00011011,
0b00001001,
0b00000010,
0b01010100,
0b01111000,
0b00000000,
};

const uint8_t folder_icon[] PROGMEM = {
0b00000000,
0b01111111,
0b01100001,
0b01110001,
0b01111001,
0b01111111,
0b00111100,
0b00011100,
0b00001100,
0b00000100,
0b00000000,
};

const uint8_t file_icon[] PROGMEM = {
0b00000000,
0b11111111,
0b10000001,
0b10000001,
0b10000111,
0b10000110,
0b11111100,
0b00000000,
};

const uint8_t fire_icon[] PROGMEM = {
0b00000000,
0b00011000,
0b00100100,
0b01001010,
0b01010101,
0b01001010,
0b00100100,
0b00011000,
0b00000000,
};

const uint8_t cooldown_icon[] PROGMEM = {
0b00000000,
0b00100010,
0b01101011,
0b00011100,
0b00110110,
0b00011100,
0b01101011,
0b00100010,
0b00000000,
};

const uint8_t flow_icon[] PROGMEM = {
0b00000000,
0b00101011,
0b00001011,
0b00101011,
0b00001011,
0b00101011,
0b00000000,
0b00000000,
};

const uint8_t spool_icon[] PROGMEM = {
0b00011100,
0b00100010,
0b01001001,
0b01010101,
0b01001001,
0b01100010,
0b01011100,
0b01000000,
};

const uint8_t clock_icon[] PROGMEM = {
0b00011100,
0b00100010,
0b01000001,
0b01001111,
0b01001001,
0b00100010,
0b00011100,
0b00000000,
};

const uint8_t print_icon[] PROGMEM = {
0b00000000,
0b01000000,
0b01000110,
0b01001011,
0b01010000,
0b01101011,
0b01100110,
0b01100000,
0b00000000,
};

const uint8_t pause_icon[] PROGMEM = {
0b00000000,
0b00111110,
0b00111110,
0b00000000,
0b00000000,
0b00111110,
0b00111110,
0b00000000,
};

const uint8_t continue_icon[] PROGMEM = {
0b00000000,
0b00000000,
0b01111110,
0b00111100,
0b00011000,
0b00000000,
0b00000000,
0b00000000,
};

const uint8_t stop_icon[] PROGMEM = {
0b00000000,
0b00111110,
0b00111110,
0b00111110,
0b00111110,
0b00111110,
0b00111110,
0b00000000,
};

const uint8_t menu_icon[] PROGMEM = {
0b00000000,
0b00101010,
0b00101010,
0b00101010,
0b00101010,
0b00101010,
0b00101010,
0b00101010,
0b00000000,
};

const uint8_t home_icon[] PROGMEM = {
0b00000000,
0b00001000,
0b01110100,
0b01111010,
0b00010101,
0b01111010,
0b01110100,
0b00001000,
0b00000000,
};

#if ENABLED(SHOW_BOOTSCREEN)

  const uint8_t start_bmp[] PROGMEM = {
    B00011111,B11111111,B00000000,
    B01100000,B00000000,B11000000,
    B01000000,B00000000,B01000000,
    B10000000,B00000000,B00100000,
    B10011111,B11110000,B00100000,
    B10011111,B11111100,B00100000,
    B10000000,B00001110,B00100000,
    B10000000,B00000110,B00100000,
    B10000000,B00000110,B00100000,
    B10000000,B00001110,B00100000,
    B10011111,B11111100,B00100000,
    B10011111,B11111100,B00100000,
    B10000000,B00001110,B00100000,
    B10000000,B00000110,B00100000,
    B10000000,B00000110,B00100000,
    B10000000,B00001110,B00100000,
    B10011111,B11111100,B00100000,
    B10011111,B11110000,B00100000,
    B10000000,B00000000,B00100000,
    B10000111,B11000000,B00100000,
    B10001110,B11100000,B00100000,
    B10011100,B01110000,B00100000,
    B10011000,B00110000,B00100000,
    B10011000,B00110000,B00100000,
    B10000000,B01110000,B00100000,
    B10011111,B11100000,B00100000,
    B10011111,B11000000,B00100000,
    B10011000,B00000000,B00100000,
    B10011111,B11000000,B00100000,
    B10011111,B11100000,B00100000,
    B10000000,B00110000,B00100000,
    B10000000,B00110000,B00100000,
    B10000000,B01110000,B00100000,
    B10000000,B11100000,B00100000,
    B10000000,B11000000,B00100000,
    B10000000,B00000000,B00100000,
    B10000111,B11111110,B00100000,
    B10001111,B11111110,B00100000,
    B10011100,B00000000,B00100000,
    B10011000,B00000000,B00100000,
    B10011000,B00000000,B00100000,
    B10000000,B00000000,B00100000,
    B10000111,B11110110,B00100000,
    B10001111,B11110110,B00100000,
    B10011100,B00000000,B00100000,
    B10011000,B00000000,B00100000,
    B10011111,B11000000,B00100000,
    B10011111,B11100000,B01100000,
    B10000000,B01110000,B11100000,
    B10000000,B00110001,B11100000,
    B10000000,B01110011,B11100000,
    B10011111,B11100111,B11100000,
    B10011111,B11001111,B11100000,
    B01000000,B00011111,B11100000,
    B01100000,B00111111,B11100000,
    B00011111,B11111111,B11100000,
  };
 
  void MarlinUI::show_bootscreen() {
    lcd.setContrast(70); // don't know why, but at this point contrast setting were not loaded
    lcd.drawBitmapP(14, 1, start_bmp, 3, sizeof(start_bmp));
    uint8_t w = lcd.drawString(0, 5, SHORT_BUILD_VERSION);
    lcd.drawString(w+2, 5, STRING_CONFIG_H_AUTHOR);
    safe_delay(BOOTSCREEN_TIMEOUT);
    clear_lcd();
  }
#endif // SHOW_BOOTSCREEN

void lcd_moveto(const lcd_uint_t col, const lcd_uint_t row) { 
  //lcd.setPrintPos(col, row);
}

int lcd_put_u8str_max(const char * utf8_str, pixel_len_t max_length) {
  lcd.print(utf8_str);
  return 0;
}

int lcd_put_wchar_max(wchar_t c, pixel_len_t max_length) {
/*  char nc;
  if (c < 256)
    nc = (char)c;
  else
    nc = '?'; // utf8 char not supported
  lcd.draw_char(nc, true);
  return 1;*/
  return 0;
}

void host_action_cancel() {
  // tell host that printing was cancelled
  SERIAL_ERROR_MSG("Print aborted.");
}

void write_str_fill_width(uint8_t cx, uint8_t cy, const char *str, uint8_t width) {
  uint8_t adv = lcd.drawString(cx, cy, str);
  if (adv < width) // clean remaining
    lcd.clearRect(cx + adv, cy, width - adv);
}

uint8_t nk_draw_hotend_status(const int8_t e, const int16_t target, const float degree, 
  const uint8_t cx, const uint8_t cy, const uint8_t icon[], const uint8_t icon_width, const uint8_t icon_size) {

  lcd.drawBitmapP(cx, cy, icon, icon_width, icon_size);
  uint8_t left = cx + ICON_WIDTH;
  lcd.setPrintPos(left, cy);
  char buffer[15];
  sprintf_P(buffer, PSTR("%hu/%hu\x7F"), (uint16_t)degree, target);
  left += lcd.print(buffer);

  lcd.setColor(CI_FOREGROUND);
  uint16_t tr = (cx == 0) ? lcd.getWidth()/2 : lcd.getWidth();
  if (left < tr)
    lcd.clearRect(left, cy, tr - left);
  
  return left - cx;
}

void menu_main();
void menu_media();
void menu_preheat_m1();
void menu_preheat_m2();
void menu_change_filament();
void lcd_move_x();
void lcd_move_y();
void _menu_move_distance(const AxisEnum axis, const screenFunc_t func, const int8_t eindex=-1);

void edit_nozzle1() {
  MenuItem_int3::action(GET_TEXT(MSG_NOZZLE), 
    &thermalManager.temp_hotend[0].target, 0, heater_maxtemp[0] - 15, 
    []{ thermalManager.start_watching_hotend(0); ui.go_back(); });
}

#if HOTENDS > 1
void edit_nozzle2() {
  MenuItem_int3::action(GET_TEXT(MSG_NOZZLE),
    &thermalManager.temp_hotend[1].target, 0, heater_maxtemp[1] - 15, 
    []{ thermalManager.start_watching_hotend(1); ui.go_back(); });
}
#endif

#if HAS_HEATED_BED
void edit_bed() {
  MenuItem_int3::action(GET_TEXT(MSG_BED), 
    &thermalManager.temp_bed.target, 0, BED_MAXTEMP - 10, 
    []{ thermalManager.start_watching_bed(); ui.go_back(); });
}
#endif

#if HAS_FAN0
void edit_fan() {
  MenuItem_percent::action(GET_TEXT(MSG_FIRST_FAN_SPEED), 
    &thermalManager.fan_speed[0], 0, 255, 
    [] {thermalManager.set_fan_speed(0, thermalManager.fan_speed[0]); ui.go_back(); });
}
#endif

void edit_flow() {
  MenuItem_int3::action(GET_TEXT(MSG_FLOW),
    &planner.flow_percentage[active_extruder], 10, 999, 
    []{ planner.refresh_e_factor(active_extruder); ui.go_back(); });
}

void edit_feedrate() {
  MenuItem_int3::action(GET_TEXT(MSG_SPEED),
    &feedrate_percentage, 10, 999, 
    []{ ui.go_back(); });
}

void autohome() {
  MenuItem_gcode::action(GET_TEXT(MSG_AUTO_HOME), G28_STR);
}

void submenu_move_x() {
  MenuItem_submenu::action(GET_TEXT(MSG_MOVE_X), []{ _menu_move_distance(X_AXIS, lcd_move_x); });
}

void submenu_move_y() {
  MenuItem_submenu::action(GET_TEXT(MSG_MOVE_Y), []{ _menu_move_distance(Y_AXIS, lcd_move_y); });
}

void submenu_move_z() {
  MenuItem_submenu::action(GET_TEXT(MSG_MOVE_Z), []{ _menu_move_distance(Z_AXIS, lcd_move_z); });
}

void MarlinUI::status_screen() {
  #if ENABLED(ENCODER_RATE_MULTIPLIER)
  MarlinUI::enable_encoder_multiplier(false);
  #endif

  #if HAS_LCD_MENU

    // check rotary encoder
    #if HAS_ENCODER_ACTION
    int8_t pos = int16_t(encoderPosition) / ENCODER_STEPS_PER_MENU_ITEM;
    uint8_t newp = (menu_selected + S_LAST + pos) % S_LAST;
    menu_selected = selected_item(newp);

    // skip disabled items
    if (printingIsActive() || printingIsPaused()) {
      while (menu_selected == S_PRINT ||
             menu_selected == S_AUTOHOME ||
             menu_selected == S_PRHEAT1 ||
             menu_selected == S_PRHEAT2 ||
             menu_selected == S_COOLDOWN ||
             menu_selected == S_X ||
             menu_selected == S_Y ||
             menu_selected == S_Z) {
        newp = (menu_selected + S_LAST + ((pos > 0) ? 1 : -1)) % S_LAST;
        menu_selected = selected_item(newp);
      }
    } else {
      // not printing
      while (menu_selected == S_PAUSE ||
             menu_selected == S_STOP ||
             menu_selected == S_ZBABYSTEPPING) {
        newp = (menu_selected + S_LAST + ((pos > 0) ? 1 : -1)) % S_LAST;
        menu_selected = selected_item(newp);
      }
    }
    encoderPosition = 0;
    #endif
  
    if (use_click()) {
      switch (menu_selected) {
        case S_NOZZLE1:
          ui.goto_screen(edit_nozzle1);
          break;

        #if HOTENDS > 1
        case S_NOZZLE2:
          ui.goto_screen(edit_nozzle2);
          break;
        #endif

        #if HAS_HEATED_BED
        case S_BED:
          ui.goto_screen(edit_bed);
          break;
        #endif

        #if HAS_FAN0
        case S_FAN:
          ui.goto_screen(edit_fan);
          break;
        #endif

        case S_FLOW:
          ui.goto_screen(edit_flow);
          break;

        case S_PRINT:
          #if ENABLED(SDSUPPORT)
          ui.goto_screen(menu_media);
          #endif 
          break;

        case S_PAUSE:
          if (printingIsPaused())
            ui.resume_print();
          else
            ui.pause_print();
          break;

        case S_STOP:
          ui.abort_print();
          break;

        case S_FEEDRATE:
          ui.goto_screen(edit_feedrate);
          break;

        case S_AUTOHOME:
          autohome();
          break;

        case S_PRHEAT1:
          ui.goto_screen(menu_preheat_m1);
          break;

        case S_PRHEAT2:
          ui.goto_screen(menu_preheat_m2);
          break;

        case S_COOLDOWN:
          thermalManager.zero_fan_speeds();
          thermalManager.disable_all_heaters();
          break;

        #if HAS_FILAMENT_SENSOR
        case S_FILAMENT:
          ui.goto_screen(menu_change_filament);
          break;
        #endif

        #if ENABLED(BABYSTEPPING)
        case S_ZBABYSTEPPING:
          #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
          ui.goto_screen(lcd_babystep_zoffset);
          #else
          ui.goto_screen(lcd_babystep_z);
          #endif
          break;
        #endif

        case S_Z:
          submenu_move_z();
          break;

        case S_X:
          submenu_move_x();
          break;

        case S_Y:
          submenu_move_y();
          break;

        case S_MENU:
        default:
          ui.goto_screen(menu_main);
          break;
      }
      return;
    }
  #endif
  
  draw_status_screen();
}

void MarlinUI::draw_status_screen() {
  static bool last_blink = false;
  uint8_t cx = 0;
  uint8_t cy = 0;
  lcd.setPrintPos(cx, cy);
  lcd.setColor(CI_FOREGROUND);
  bool printing = (printingIsActive() || printingIsPaused());
  
  #define SET_COLOR_FOR(x) (menu_selected == x) ? lcd.setColor(CI_BACKGROUND) : lcd.setColor(CI_FOREGROUND)
  
  // line 0 is for buttons: menu, print, pause, stop, preheat
  // menu
  SET_COLOR_FOR(S_MENU);
  lcd.drawBitmapP(cx, cy, menu_icon, 1, sizeof(menu_icon));
  cx += ICON_WIDTH + 2;
  
  if (!printing) {
    // print
    SET_COLOR_FOR(S_PRINT);
    lcd.drawBitmapP(cx, cy, print_icon, 1, sizeof(print_icon));
    cx += ICON_WIDTH + 2;

    SET_COLOR_FOR(S_AUTOHOME);
    lcd.drawBitmapP(cx, cy, home_icon, 1, sizeof(home_icon));
    cx += ICON_WIDTH + 2;

    // preheat 1
    SET_COLOR_FOR(S_PRHEAT1);
    lcd.drawBitmapP(cx, cy, fire_icon, 1, sizeof(fire_icon));
    cx += ICON_WIDTH + 1;
    cx += lcd.drawString(cx, cy, "1");
    cx += 2;
    
    // preheat 2
    SET_COLOR_FOR(S_PRHEAT2);
    lcd.drawBitmapP(cx, cy, fire_icon, 1, sizeof(fire_icon));
    cx += ICON_WIDTH + 1;
    cx += lcd.drawString(cx, cy, "2");
    cx += 2;

    // cooldown
    SET_COLOR_FOR(S_COOLDOWN);
    lcd.drawBitmapP(cx, cy, cooldown_icon, 1, sizeof(cooldown_icon));
    cx += ICON_WIDTH + 2;
  }
  else {
    // pause or continue
    SET_COLOR_FOR(S_PAUSE);
    if (printingIsPaused())
      lcd.drawBitmapP(cx, cy, continue_icon, 1, sizeof(continue_icon));
    else
      lcd.drawBitmapP(cx, cy, pause_icon, 1, sizeof(pause_icon));
    cx += ICON_WIDTH + 2;

    // stop
    SET_COLOR_FOR(S_STOP);
    lcd.drawBitmapP(cx, cy, stop_icon, 1, sizeof(stop_icon));
    cx += ICON_WIDTH + 2;
  }

  // filament runnout
  #if HAS_FILAMENT_SENSOR
  SET_COLOR_FOR(S_FILAMENT);
  lcd.drawBitmapP(cx, cy, spool_icon, 1, sizeof(spool_icon));
  cx += ICON_WIDTH;
  if (runout.enabled && runout.filament_ran_out) {
    lcd.setPrintPos(cx + ICON_WIDTH, cy);
    cx += lcd.print("*");
  }
  #endif

  lcd.setColor(CI_FOREGROUND);
  lcd.clearRect(cx, cy, lcd.getWidth() - cx);

  // lines 1 and 2 are for temperature and z (when printing)
  // hotends
  cy = 1;
  cx = 0;
  SET_COLOR_FOR(S_NOZZLE1);
  nk_draw_hotend_status(0, thermalManager.degTargetHotend(0), 
        thermalManager.degHotend(0), cx, cy, nozzle_icon, 1, sizeof(nozzle_icon));

  #if HOTENDS > 1
  cx = 43;
  SET_COLOR_FOR(S_NOZZLE2);
  nk_draw_hotend_status(1, thermalManager.degTargetHotend(1), 
        thermalManager.degHotend(1), cx, cy, nozzle_icon, 1, sizeof(nozzle_icon));
  #endif

  // bed
  cy = 2;
  cx = 0;
  #if HAS_HEATED_BED
    SET_COLOR_FOR(S_BED);
    cx += nk_draw_hotend_status(H_BED, thermalManager.degTargetBed(),
      thermalManager.degBed(), cx, cy, bed_icon, 1, sizeof(bed_icon));
    cx++;
  #endif

  // show Z on line 2 when printing 
  if (printing && cx <= lcd.getWidth() - 25) {
    uint8_t width = lcd.stringWidth("Z") + 1;
    const char *txt = ftostr4sign(LOGICAL_Z_POSITION(current_position[Z_AXIS]));
    width += lcd.stringWidth(txt);
    
    uint8_t newcx = lcd.getWidth() - width;
    lcd.setColor(CI_FOREGROUND);
    lcd.clearRect(cx, cy, newcx - cx);
    cx = newcx;

    SET_COLOR_FOR(S_ZBABYSTEPPING);
    lcd.setPrintPos(cx, cy);
    cx += lcd.print("Z", true);
    cx += lcd.print(txt);
  }

  if (cx < lcd.getWidth()) {
    lcd.setColor(CI_FOREGROUND);
    lcd.clearRect(cx, cy, lcd.getWidth() - cx);
  }

  // line 3: feedrate, flow and fan
  const uint8_t line3w = 28;
  cy = 3;
  cx = 0;

  // feedrate
  SET_COLOR_FOR(S_FEEDRATE);
  lcd.drawBitmapP(cx, cy, feedrate_icon, 1, sizeof(feedrate_icon));
  cx += ICON_WIDTH;
  lcd.setPrintPos(cx, cy);
  cx += lcd.print(i16tostr3left(feedrate_percentage), true);
  cx += lcd.print("%");
  uint8_t rcx = cx;

  // fan0, right aligned
  uint8_t fcx = 0;
  #if HAS_FAN0
    const int16_t per = ((thermalManager.fan_speed[0] + 1) * 100) / 256;
    const char *aux = i16tostr3left(per);
    fcx = lcd.getWidth() - ICON_WIDTH - lcd.stringWidth(aux) - lcd.stringWidth("%") - 1;

    SET_COLOR_FOR(S_FAN);
    cx = fcx;
    lcd.drawBitmapP(cx, cy, fan_icon, 1, sizeof(fan_icon));
    cx += ICON_WIDTH;
    lcd.setPrintPos(cx, cy);
    lcd.print(aux, true);
    lcd.print("%");
  #endif

  // flow, centered
  uint8_t lcx = cx = line3w + (fcx == 0 ? 0 : (fcx - 2*line3w)/2);
  SET_COLOR_FOR(S_FLOW);
  lcd.drawBitmapP(cx, cy, flow_icon, 1, sizeof(flow_icon));
  cx += ICON_WIDTH;
  lcd.setPrintPos(cx, cy);
  cx += lcd.print(i16tostr3left(planner.flow_percentage[active_extruder]), true);
  cx += lcd.print("%");

  lcd.setColor(CI_FOREGROUND);
  lcd.clearRect(rcx, cy, lcx - rcx);
  lcd.clearRect(cx, cy, fcx - cx);

  // line 4: x, y and z when !printing; printing duration and progress otherwise
  cy = 4;
  cx = 0;

  if (!printing) {
    // x, y, z position
    lcd.setColor(CI_FOREGROUND);
    lcd.setPrintPos(cx, cy);
    SET_COLOR_FOR(S_Z);
    cx += lcd.print("Z", true);
    if (TEST(axis_homed, Z_AXIS))
      cx += lcd.print(ftostr4sign(LOGICAL_Z_POSITION(current_position[Z_AXIS])));
    else
      cx += lcd.print(" ?");  

    SET_COLOR_FOR(S_X);
    cx += lcd.print(" X", true);
    if (TEST(axis_homed, X_AXIS))
      cx += lcd.print(ftostr4sign(LOGICAL_X_POSITION(current_position[X_AXIS])));
    else
      cx += lcd.print(" ?");

    SET_COLOR_FOR(S_Y);
    cx += lcd.print(" Y", true);
    if (TEST(axis_homed, Y_AXIS))
      cx += lcd.print(ftostr4sign(LOGICAL_Y_POSITION(current_position[Y_AXIS])));
    else
      cx += lcd.print(" ?");

    lcd.setColor(CI_FOREGROUND);
    lcd.clearRect(cx, cy, LCD_PIXEL_WIDTH - cx);
  }
  else { // printingIsActive
    // sd printing and time
    #if EITHER(LCD_SET_PROGRESS_MANUALLY, SDSUPPORT)
      char buffer[20];
      uint8_t pos;

      // time elapsed
      duration_t elapsed = print_job_timer.duration();
      bool has_days = (elapsed.value >= 60*60*24L);
      pos = elapsed.toDigital(buffer, has_days);
      buffer[pos++] = ' ';

      // % done
      uint8_t progress_bar_percent = ui.get_progress_percent();
      const char *p = i16tostr3left(progress_bar_percent);
      while(*p) { buffer[pos++] = *p; p++; };
      buffer[pos++] = '%';
      buffer[pos++] = ' ';
      buffer[pos++] = 0;

      lcd.setColor(CI_FOREGROUND);
      lcd.drawBitmapP(cx, cy, clock_icon, 1, sizeof(clock_icon));
      cx += ICON_WIDTH;
      cx += lcd.drawString(ICON_WIDTH, cy, buffer);

      // progress bar
      lcd.setColor(CI_FOREGROUND);
      lcd.setPrintPos(cx, cy);
      lcd.drawPBar(lcd.getWidth() - cx, progress_bar_percent);
    #endif
  }

  // message
  cy = 5;
  lcd.setColor(CI_FOREGROUND);
  #if !ENABLED(STATUS_MESSAGE_SCROLLING)
    write_str_fill_width(0, cy, status_message, LCD_PIXEL_WIDTH);
  #else
    // string fit
    uint8_t len = lcd.stringWidth(status_message);
    if (len <= LCD_PIXEL_WIDTH) {
      write_str_fill_width(0, cy, status_message, LCD_PIXEL_WIDTH);
    }
    else { // string larger than screen width
      uint8_t rlen;
      const char *stat = status_and_len(rlen);
      write_str_fill_width(0, cy, stat, LCD_PIXEL_WIDTH);
      bool blink = get_blink();
      if (last_blink != blink) {
        last_blink = blink;
        advance_status_scroll();
      }
    }
  #endif
}

// Initialize or re-initialize the LCD
void MarlinUI::init_lcd() {
  lcd.begin();
  if (contrast == 0) contrast = DEFAULT_LCD_CONTRAST;
  refresh_contrast();
  drawing_screen = false;
  first_page = true;  // u8g concept, but scrolling in menus depend on this
}

// The kill screen is displayed for unrecoverable conditions
void MarlinUI::draw_kill_screen() {
  lcd.setColor(CI_BACKGROUND);
  lcd.clearScreen();
  lcd.drawString(0, 0, status_message);
  lcd.setPrintPos(0, 3);
  lcd.printP(GET_TEXT(MSG_HALTED));
  lcd.setPrintPos(0, 4);
  lcd.printP(GET_TEXT(MSG_PLEASE_RESET));
}

void MarlinUI::clear_lcd() {
  lcd.setColor(CI_FOREGROUND);
  lcd.clearScreen();
}

#if HAS_LCD_MENU

  #include "../menu/menu.h"

  uint8_t row_y1, row_y2;

  void __draw_text_line(const bool sel, const uint8_t row, PGM_P const pstr, const char * const data,
    const bool pgm, const bool einline, const bool pstr_pgm = true, const uint8_t *icon = NULL,
    const uint8_t icon_width = 2, const uint8_t icon_size = 0) {
    
    uint8_t cy = row;
    uint8_t cx = 0;

    if (sel) lcd.setColor(CI_BACKGROUND);
    else lcd.setColor(CI_FOREGROUND);

    if (icon) {
      lcd.drawBitmapP(cx, cy, icon, icon_width, icon_size);
      cx = ICON_WIDTH;
    }
    
    // print main text
    uint8_t w = cx;
    lcd.setPrintPos(cx, cy);
    if (pstr) {
      if (pstr_pgm) w += lcd.printP(pstr);
      else w += lcd.print(pstr);
    }

    // print data
    uint8_t dataw = 0;
    if (data) {
      if (!einline) {
        cy += 1;
        w = 0;
      }
      if (pgm) {
        dataw = lcd.stringWidthP(data);
        lcd.setPrintPos(LCD_PIXEL_WIDTH - dataw, cy);
        lcd.printP(data);
      } else {
        dataw = lcd.stringWidth(data);
        lcd.setPrintPos(LCD_PIXEL_WIDTH - dataw, cy);
        lcd.print(data);
      }
    }

    // fill remaining empty background
    int8_t wf = LCD_PIXEL_WIDTH - dataw - w;
    if (wf > 0) {
      lcd.clearRect(w, cy, wf);
    }

    /*if (!pstr) { // text draw next using lcd_print
      lcd.setPrintPos(cx, cy);
    }*/
  }

  void MenuItemBase::_draw(const bool sel, const uint8_t row, PGM_P const pstr, const char, const char post_char) {
    __draw_text_line(sel, row, pstr, NULL, false, true);
  }
  
  void MenuItem_static::draw(const uint8_t row, PGM_P const pstr, const uint8_t style/*=SS_DEFAULT*/, const char * const valstr/*=nullptr*/) {
    __draw_text_line(false, row, pstr, valstr, false, true);
  }

  void MenuEditItemBase::draw(const bool sel, const uint8_t row, PGM_P const pstr, const char* const data, const bool pgm) {
    __draw_text_line(sel, row, pstr, data, pgm, true);
  }
  
  void MenuEditItemBase::draw_edit_screen(PGM_P const pstr, const char* const value/*=nullptr*/) {
    //ui.encoder_direction_normal();    
    __draw_text_line(false, 0, pstr, value, false, false);
  }

  inline void draw_boxed_string(const uint8_t x, const uint8_t y, PGM_P const pstr, const bool inv) {
    if (inv) {
      lcd.setColor(CI_BACKGROUND);
      //lcd.drawFrame(x - 1, y - FONT_CHAR_HEIGHT, LCD_PIXEL_WIDTH/2-1, 8);
    } else {
      lcd.setColor(CI_FOREGROUND);
      //lcd.drawFrame(x - 1, y - FONT_CHAR_HEIGHT, LCD_PIXEL_WIDTH/2-1, 8);
    }
    lcd.setPrintPos(x+2, y);
    lcd.printP(pstr);
  }

  void MenuItem_confirm::draw_select_screen(PGM_P const yes, PGM_P const no, const bool yesno, PGM_P const pref, const char * const string/*=nullptr*/, PGM_P const suff/*=nullptr*/) {
    uint8_t cy = 0;
    lcd.setColor(CI_FOREGROUND);
    lcd.setPrintPos(0, cy);
    lcd.printP(pref);
    if (string) {
      cy++;
      lcd.drawString(0, cy, string);
    }
    if (suff) {
      cy++;
      lcd.setPrintPos(0, cy);
      lcd.printP(suff);
    }
    //ui.draw_select_screen_prompt(pref, string, suff);
    draw_boxed_string(1, 5, no, !yesno);
    draw_boxed_string(LCD_PIXEL_WIDTH/2+1, 5, yes, yesno);
  }

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
      uint8_t cx = LCD_PIXEL_WIDTH / 2 - ICON_WIDTH;
      uint8_t cy = 4;
      lcd.setColor(CI_FOREGROUND);
      
      // draw nozzle icon and temperatures
      nk_draw_hotend_status(extruder, thermalManager.degTargetHotend(extruder),
        thermalManager.degHotend(extruder), cx, cy, nozzle_icon, 1, sizeof(nozzle_icon));
    }

  #endif // ADVANCED_PAUSE_FEATURE

  #if ENABLED(SDSUPPORT)

    void MenuItem_sdbase::draw(const bool sel, const uint8_t row, PGM_P const, CardReader &theCard, const bool isDir) {
      const char *stat = ui.scrolled_filename(theCard, LCD_PIXEL_WIDTH/FONT_CHAR_WIDTH, row, sel);
      if (isDir)
        __draw_text_line(sel, row, stat, NULL, false, false, false, folder_icon, 1, sizeof(folder_icon));
      else
        __draw_text_line(sel, row, stat, NULL, false, false, false, file_icon, 1, sizeof(file_icon));
    }

  #endif // SDSUPPORT

  #if HAS_LCD_CONTRAST

  int16_t MarlinUI::contrast = DEFAULT_LCD_CONTRAST;

  void MarlinUI::set_contrast(const int16_t value) {
    contrast = constrain(value, LCD_CONTRAST_MIN, LCD_CONTRAST_MAX);
    lcd.setContrast(contrast);
  }

  #endif


#endif // HAS_LCD_MENU

#endif // NOKIA5110_LCD
