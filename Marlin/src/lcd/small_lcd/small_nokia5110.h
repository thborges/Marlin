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
 * smallucg_lcd_impl.h
 *
 * Graphic screen implementation for NOKIA 5110
 * by Thiago Borges de Oliveira, May 2020
 */

#pragma once

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(NOKIA5110_LCD)

#include "../../libs/numtostr.h"
#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"
#include "nokia5110_ga.h"

#if ENABLED(SDSUPPORT)
  #include "../../libs/duration_t.h"
#endif

#ifdef __STM32F1__
  //#include "smallucg_HAL_AVR.h"
  #include "nokia5110_ga.h"
#else
  #error "Currently, NOKIA5110 LCD only works with STM32F1 boards"
#endif

NOKIA5110GA ucg(SLCD_CLK_PIN, SLCD_DAT_PIN, SLCD_CS_PIN, SLCD_DC_PIN);

#define FONT_TEXT_NAME ucg_font_04b_03_hr

// status screen block width: icon plus three chars and two spaces
#define BLOCK_WIDTH (ICON_WIDTH + FONT_CHAR_WIDTH*3 + 1)
#define BLOCK_TXT_WIDTH  (FONT_CHAR_WIDTH*3 + 1)

bool MarlinUI::detected() { return true; }
void MarlinUI::set_font(const MarlinFont font_nr) { }

#define ICON_WIDTH 12
#define ICON_HEIGHT 14

const uint8_t nozzle_icon[] PROGMEM = {
0b00000001,0b11000000,
0b00100011,0b11000000,
0b00100010,0b00111110,
0b00100110,0b00111110,
0b00011100,0b00000110,
0b00001100,0b00000110,
0b00000110,0b00111110,
0b00000010,0b00111110,
0b00000011,0b11000000,
0b00000001,0b11000000,
};

const uint8_t bed_icon[] PROGMEM = {
0b00111000,0b00000000,
0b00111000,0b00000000,
0b00011000,0b00011000,
0b00011001,0b00100100,
0b00011000,0b11000010,
0b00011000,0b00000000,
0b00011000,0b00011000,
0b00011001,0b00100100,
0b00011000,0b11000010,
0b00111000,0b00000000,
0b00111000,0b00000000,
0b00000000,0b00000000,
};

const uint8_t fan_icon[] PROGMEM = {
0b00000000,0b01000000,
0b00000100,0b11001000,
0b00001100,0b10011100,
0b00000110,0b10110000,
0b00000011,0b11100000,
0b00110001,0b11111100,
0b00011111,0b11100110,
0b00000001,0b11110000,
0b00000011,0b01011000,
0b00001110,0b01001100,
0b00000100,0b01001000,
0b00000000,0b11000000,
0b00000000,0b10000000,
};

// it is supposed to be a velocimeter
const uint8_t feedrate_icon[] PROGMEM = {
0b00000000,0b10110000,
0b00000000,0b00001000,
0b00001110,0b00000000,
0b00001110,0b00000010,
0b00001111,0b00000010,
0b00000001,0b10000110,
0b00000000,0b01000110,
0b00000000,0b00000110,
0b00000000,0b00001100,
0b00000001,0b11101100,
0b00000001,0b11101000,
0b00000001,0b11100000,
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
    ucg.setContrast(70); // don't know why, but at this point contrast setting were not loaded
    ucg.drawBitmapP(14, 1, start_bmp, 3, sizeof(start_bmp));
    uint8_t w = ucg.drawString(0, 5, SHORT_BUILD_VERSION);
    ucg.drawString(w+2, 5, STRING_CONFIG_H_AUTHOR);
    safe_delay(BOOTSCREEN_TIMEOUT);
    clear_lcd();
  }
#endif // SHOW_BOOTSCREEN

void lcd_moveto(const lcd_uint_t col, const lcd_uint_t row) { 
  //ucg.setPrintPos(col, row);
}

int lcd_put_u8str_max(const char * utf8_str, pixel_len_t max_length) {
  ucg.print(utf8_str);
  return 0;
}

int lcd_put_wchar_max(wchar_t c, pixel_len_t max_length) {
/*  char nc;
  if (c < 256)
    nc = (char)c;
  else
    nc = '?'; // utf8 char not supported
  ucg.draw_char(nc, true);
  return 1;*/
  return 0;
}

void host_action_cancel() {
  // tell host that printing was cancelled
  SERIAL_ERROR_MSG("Print aborted.");
}

void write_str_fill_width(uint8_t cx, uint8_t cy, const char *str, uint8_t width) {
  uint8_t adv = ucg.drawString(cx, cy, str);
  if (adv < width) // clean remaining
    ucg.clearRect(cx + adv, cy, width - adv);
}

void smallucg_draw_hotend_status(const int8_t e, const int16_t target, const float degree, 
  const uint8_t cx, const uint8_t cy, const uint8_t icon[], const uint8_t icon_width, const uint8_t icon_size) {

  const bool idle = thermalManager.hotend_idle[e].timed_out || target == 0;
  ucg.drawBitmapP(cx, cy, icon, icon_width, icon_size);

  uint8_t left = cx + ICON_WIDTH;
  if (idle)
    //font has speciall coded upper o, i.e., ˚ in pos 127
    write_str_fill_width(left, cy+1, "\x7F""C", BLOCK_TXT_WIDTH);
  else
    write_str_fill_width(left, cy+1, i16tostr3left(target), BLOCK_TXT_WIDTH);
  
  write_str_fill_width(left, cy, i16tostr3left(degree), BLOCK_TXT_WIDTH);
}

void MarlinUI::draw_status_screen() {
  static bool last_blink = false;
  uint8_t cx = 0;
  uint8_t cy = 0;
  ucg.setPrintPos(cx, cy);
  ucg.setColor(CI_FOREGROUND);

  #define SCR_BLOCK_POS_CTRL do { \
    cx += BLOCK_WIDTH; \
    if (LCD_PIXEL_WIDTH - cx < BLOCK_WIDTH) { \
      cy += 2; \
      cx = 0; \
    } \
  } while (0)

  // hotends
  HOTEND_LOOP() {
  smallucg_draw_hotend_status(e, thermalManager.degTargetHotend(e), 
    thermalManager.degHotend(e), cx, cy, nozzle_icon, 2, sizeof(nozzle_icon));
  }
  SCR_BLOCK_POS_CTRL;

  // bed
  #if HAS_HEATED_BED
    smallucg_draw_hotend_status(H_BED, thermalManager.degTargetBed(),
      thermalManager.degBed(), cx, cy, bed_icon, 2, sizeof(bed_icon));
    SCR_BLOCK_POS_CTRL;
  #endif

  // fan0
  #if HAS_FAN0
    ucg.drawBitmapP(cx, cy, fan_icon, 2, sizeof(fan_icon));
    ucg.drawString(cx + ICON_WIDTH, cy + 1, "%");
    const int16_t per = ((thermalManager.fan_speed[0] + 1) * 100) / 256;
    write_str_fill_width(cx + ICON_WIDTH, cy, i16tostr3left(per), BLOCK_TXT_WIDTH);
    SCR_BLOCK_POS_CTRL;
  #endif

  // feedrate
  ucg.drawBitmapP(cx, cy, feedrate_icon, 2, sizeof(feedrate_icon));
  ucg.drawString(cx + ICON_WIDTH + 1, cy + 1, "%");
  write_str_fill_width(cx + ICON_WIDTH + 1, cy, i16tostr3left(feedrate_percentage), BLOCK_TXT_WIDTH-1);
  SCR_BLOCK_POS_CTRL;

  // sd printing and time
  #if EITHER(LCD_SET_PROGRESS_MANUALLY, SDSUPPORT)
    uint8_t progress_bar_percent = ui.get_progress_percent();
    const char *p = i16tostr3left(progress_bar_percent);
    char buffer[20]; uint8_t i = 0;
    while(*p) { buffer[i++] = *p; p++; };
    buffer[i++] = '%';
    buffer[i++] = ' ';
    // time elapsed
    duration_t elapsed = print_job_timer.duration();
    elapsed.toDigital(&buffer[i], true);
    write_str_fill_width(cx, cy, buffer, BLOCK_WIDTH*2);
    // progress bar
    ucg.setPrintPos(cx, cy+1);
    ucg.drawPBar(BLOCK_WIDTH*2, progress_bar_percent);
  #endif

  // x,y,z position
  uint8_t w = 0;
  cy = 4;
  ucg.setPrintPos(0, cy);
  w += ucg.print("Z");
  if (TEST(axis_homed, Z_AXIS))
    w += ucg.print(ftostr4sign(LOGICAL_X_POSITION(current_position[Z_AXIS])));
  else
    w += ucg.print(" ?");  

  if (!printingIsActive()) {
    // print X and Y when not printing
    w += ucg.print(" X");
    if (TEST(axis_homed, X_AXIS))
      w += ucg.print(ftostr4sign(LOGICAL_X_POSITION(current_position[X_AXIS])));
    else
      w += ucg.print(" ?");
    w += ucg.print(" Y");
    if (TEST(axis_homed, Y_AXIS))
      w += ucg.print(ftostr4sign(LOGICAL_X_POSITION(current_position[Y_AXIS])));
    else
      w += ucg.print(" ?");
  }
  ucg.clearRect(w, cy, LCD_PIXEL_WIDTH - w);

  // message
  cy = 5;
  #if !ENABLED(STATUS_MESSAGE_SCROLLING)
    write_str_fill_width(0, cy, status_message, LCD_PIXEL_WIDTH-1);
  #else
    // string fit
    uint8_t len = ucg.stringWidth(status_message);
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
  ucg.begin();
  refresh_contrast();
  drawing_screen = false;
  first_page = true;  // u8g concept, but scrolling in menus depend on this
}

// The kill screen is displayed for unrecoverable conditions
void MarlinUI::draw_kill_screen() {
  ucg.setColor(CI_BACKGROUND);
  ucg.clearScreen();
  ucg.drawString(0, 0, status_message);
  ucg.setPrintPos(0, 3);
  ucg.printP(GET_TEXT(MSG_HALTED));
  ucg.setPrintPos(0, 4);
  ucg.printP(GET_TEXT(MSG_PLEASE_RESET));
}

void MarlinUI::clear_lcd() {
  ucg.setColor(CI_FOREGROUND);
  ucg.clearScreen();
}

#if HAS_LCD_MENU

  #include "../menu/menu.h"

  uint8_t row_y1, row_y2;

  void __draw_text_line(const bool sel, const uint8_t row, PGM_P const pstr, const char * const data,
    const bool pgm, const bool einline, const bool pstr_pgm = true, const uint8_t *icon = NULL,
    const uint8_t icon_width = 2, const uint8_t icon_size = 0) {
    
    uint8_t cy = row;
    uint8_t cx = 0;

    if (sel) ucg.setColor(CI_BACKGROUND);
    else ucg.setColor(CI_FOREGROUND);

    if (icon) {
      ucg.drawBitmapP(cx, cy, icon, icon_width, icon_size);
      cx = ICON_WIDTH;
    }
    
    // print main text
    uint8_t w = cx;
    ucg.setPrintPos(cx, cy);
    if (pstr) {
      if (pstr_pgm) w += ucg.printP(pstr);
      else w += ucg.print(pstr);
    }

    // print data
    uint8_t dataw = 0;
    if (data) {
      if (!einline) {
        cy += 1;
        w = 0;
      }
      if (pgm) {
        dataw = ucg.stringWidthP(data);
        ucg.setPrintPos(LCD_PIXEL_WIDTH - dataw, cy);
        ucg.printP(data);
      } else {
        dataw = ucg.stringWidth(data);
        ucg.setPrintPos(LCD_PIXEL_WIDTH - dataw, cy);
        ucg.print(data);
      }
    }

    // fill remaining empty background
    int8_t wf = LCD_PIXEL_WIDTH - dataw - w;
    if (wf > 0) {
      ucg.clearRect(w, cy, wf);
    }

    /*if (!pstr) { // text draw next using lcd_print
      ucg.setPrintPos(cx, cy);
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
      ucg.setColor(CI_BACKGROUND);
      //ucg.drawFrame(x - 1, y - FONT_CHAR_HEIGHT, LCD_PIXEL_WIDTH/2-1, 8);
    } else {
      ucg.setColor(CI_FOREGROUND);
      //ucg.drawFrame(x - 1, y - FONT_CHAR_HEIGHT, LCD_PIXEL_WIDTH/2-1, 8);
    }
    ucg.setPrintPos(x+2, y);
    ucg.printP(pstr);
  }

  void MenuItem_confirm::draw_select_screen(PGM_P const yes, PGM_P const no, const bool yesno, PGM_P const pref, const char * const string/*=nullptr*/, PGM_P const suff/*=nullptr*/) {
    uint8_t cy = 0;
    ucg.setColor(CI_FOREGROUND);
    ucg.setPrintPos(0, cy);
    ucg.printP(pref);
    if (string) {
      cy++;
      ucg.drawString(0, cy, string);
    }
    if (suff) {
      cy++;
      ucg.setPrintPos(0, cy);
      ucg.printP(suff);
    }
    //ui.draw_select_screen_prompt(pref, string, suff);
    draw_boxed_string(1, 5, no, !yesno);
    draw_boxed_string(LCD_PIXEL_WIDTH/2+1, 5, yes, yesno);
  }

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
      uint8_t cx = LCD_PIXEL_WIDTH / 2 - BLOCK_WIDTH;
      uint8_t cy = 4;
      ucg.setColor(CI_FOREGROUND);
      
      // draw nozzle icon and temperatures
      smallucg_draw_hotend_status(extruder, thermalManager.degTargetHotend(extruder),
        thermalManager.degHotend(extruder), cx, cy, nozzle_icon, 2, sizeof(nozzle_icon));
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
    ucg.setContrast(contrast);
  }

  #endif


#endif // HAS_LCD_MENU

#endif // NOKIA5110_LCD
