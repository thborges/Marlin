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
 * simplelcd_impl.h
 *
 * Graphics LCD implementation for small LCDS compatible with u8glib
 * Based on DOGM and ST7920 screen light implementations
 *
 * With the use of:
 * u8glib by Oliver Kraus
 * https://github.com/olikraus/U8glib_Arduino
 * License: http://opensource.org/licenses/BSD-3-Clause
 */

#ifndef SIMPLELCD_IMPL_H
#define SIMPLELCD_IMPL_H

#include "MarlinConfig.h"
#include <U8glib.h>

#if ENABLED(SIMPLEU8LCD_NOKIA5110)
  U8GLIB_PCD8544 u8g(SLCD_CLK_PIN, SLCD_DAT_PIN, SLCD_CS_PIN, SLCD_A0_PIN);
  #define LCD_PIXEL_WIDTH 84
  #define LCD_PIXEL_HEIGHT 48
#endif

#include "utf_mapper.h"

// The current graphical page being rendered
u8g_page_t &page = ((u8g_pb_t *)((u8g.getU8g())->dev->dev_mem))->p;

//#include "dogm_font_data_ISO10646_1.h"
//#define FONT_TEXT_NAME ISO10646_1_5x7
//#define FONT_TEXT_NAME u8g_font_5x7r
#define FONT_TEXT_NAME u8g_font_04b_03r
#define FONT_CHAR_HEIGHT 7
#define FONT_CHAR_WIDTH 5

#define FONT_MENU FONT_TEXT_NAME
#define lcd_setFont(F) u8g.setFont(F)

int16_t lcd_contrast; // Initialized by settings.load()

const unsigned char start_bmp[] PROGMEM = {
  B00011111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,
  B01100000,B00000000,B00000000,B00000000,B00000000,B00000001,B11111111,
  B01000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11111111,
  B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,
  B10000011,B11001111,B00000000,B00000000,B00001100,B00110000,B00111111,
  B10000111,B11111111,B10000000,B00000000,B00001100,B00110000,B00011111,
  B10000110,B01111001,B10000000,B00000000,B00001100,B00000000,B00001111,
  B10001100,B00110000,B11000111,B10000011,B10001100,B00110000,B11100111,
  B10001100,B00110000,B11001111,B11000111,B11001100,B00110001,B11110011,
  B10001100,B00110000,B11011100,B11101100,B11101100,B00110011,B10111001,
  B10001100,B00110000,B11011000,B01101100,B01101100,B00110011,B00011001,
  B10001100,B00110000,B11010000,B01101100,B00001100,B00110011,B00011001,
  B10001100,B00110000,B11011000,B01101100,B00001100,B00110011,B00011001,
  B10001100,B00110000,B11011100,B01101100,B00001110,B00111011,B00011001,
  B10001100,B00110000,B11001111,B01111100,B00000111,B10011111,B00011001,
  B10001100,B00110000,B11000111,B01111100,B00000011,B10001111,B00011001,
  B01000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000010,
  B01100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000110,
  B00011111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111000
};

#define START_BMPWIDTH 56
#define START_BMP_BYTEWIDTH ((START_BMPWIDTH + 7) / 8)
#define START_BMPHEIGHT (sizeof(start_bmp) / (START_BMP_BYTEWIDTH))

// although using 16bits, icons occupies 11 pixels
// the remaining zeros are overlaped with text later
#define ICON_WIDTH 12
#define ICON_HEIGHT 14

const uint8_t nozzle_icon[] PROGMEM = {
  0b00000000,0b00000000,
  0b00111111,0b00000000,
  0b00111111,0b00000000,
  0b00110011,0b00000000,
  0b00110011,0b00000000,
  0b00110011,0b00000000,
  0b11000000,0b11000000,
  0b11000000,0b11000000,
  0b11000000,0b11000000,
  0b01110011,0b10000000,
  0b00011110,0b00000000,
  0b00001100,0b00000000,
  0b00001000,0b00000000,
  0b01110000,0b00000000
};

const uint8_t bed_icon[] PROGMEM = {
  0b00000000,0b00000000,
  0b00001000,0b10000000,
  0b00010001,0b00000000,
  0b00100010,0b00000000,
  0b00100010,0b00000000,
  0b00010001,0b00000000,
  0b00001000,0b10000000,
  0b00001000,0b10000000,
  0b00010001,0b00000000,
  0b00000000,0b00000000,
  0b00000000,0b00000000,
  0b11111111,0b11100000,
  0b11111111,0b11100000,
  0b11000000,0b01100000
};

const uint8_t fan_icon[] PROGMEM = {
  0b00000000,0b00000000,
  0b00100000,0b10000000,
  0b01110001,0b11000000,
  0b11110001,0b11100000,
  0b01111011,0b11000000,
  0b00001110,0b00000000,
  0b01111011,0b11000000,
  0b11110001,0b11100000,
  0b01110001,0b11000000,
  0b00100000,0b10000000,
  0b00000000,0b00000000,
  0b00000000,0b00000000,
  0b00000000,0b00000000,
  0b00000000,0b00000000
};
// a 16x16 fan
/*
b0000011100000000
b0000111110000000
b0001111110001100
b0001111110011110
b0000111110111111
b0000011111111111
b0000001111111111
b0111001111111111
b1111111111001110
b1111111111100000
b1111111111110000
b1111110111111000
b0111100111111000
b0011000111111000
b0000000111110000
b0000000011100000
*/

// it is supposed to be a velocimeter
const uint8_t feedrate_icon[] PROGMEM = {
  0b00000000,0b00000000,
  0b00011111,0b00000000,
  0b00000001,0b10000000,
  0b01000000,0b11000000,
  0b10000000,0b00000000,
  0b10000000,0b11100000,
  0b00000000,0b11100000,
  0b10000100,0b11100000,
  0b00001100,0b11100000,
  0b00111000,0b00000000,
  0b01001000,0b00000000,
  0b00110000,0b00000000,
  0b00000000,0b00000000,
  0b00000000,0b00000000
};

void lcd_implementation_clear() { }

#if ENABLED(SHOW_BOOTSCREEN)
  void lcd_bootscreen() {
    constexpr uint8_t offy = (LCD_PIXEL_HEIGHT - START_BMPHEIGHT - 2*FONT_CHAR_HEIGHT) / 2;

    const uint8_t width = LCD_PIXEL_WIDTH,
                  offx = (width - (START_BMPWIDTH)) / 2;

    u8g.firstPage();
    do {
      u8g.drawBitmapP(offx, offy, START_BMP_BYTEWIDTH, START_BMPHEIGHT, start_bmp);
      const uint8_t txt1X = (width - (sizeof(STRING_SPLASH_LINE1) - 1) * (FONT_CHAR_WIDTH)) / 2;
      u8g.drawStr(txt1X, START_BMPHEIGHT + offy, STRING_SPLASH_LINE1);
      #ifdef STRING_SPLASH_LINE2
        const uint8_t txt2X = (width - (sizeof(STRING_SPLASH_LINE2) - 1) * (FONT_CHAR_WIDTH)) / 2;
        u8g.drawStr(txt2X, START_BMPHEIGHT + offy + FONT_CHAR_HEIGHT, STRING_SPLASH_LINE2);
      #endif
    } while (u8g.nextPage());

    safe_delay(BOOTSCREEN_TIMEOUT);
  }

#endif // SHOW_BOOTSCREEN

char need_full_refresh = true;

static void lcd_implementation_status_screen() {
  u8g.firstPage();
  do {
    uint8_t cx = 0;
    uint8_t cy = 0;
    //const char *celsius = "\xb0""C";
    const uint8_t block_width = ICON_WIDTH + FONT_CHAR_WIDTH*3 + 1; // icon plus three chars

    #define SCR_BLOCK_POS_CTRL do { \
      cx += block_width; \
      if (LCD_PIXEL_WIDTH - cx < block_width) { \
        cy += ICON_HEIGHT+2; \
        cx = 0; \
      } \
    } while (0)

    // hotends
    HOTEND_LOOP() {
      const int16_t targ = thermalManager.degTargetHotend(e);
      const bool idle = thermalManager.is_heater_idle(e) || targ == 0;
      u8g.drawBitmapP(cx, cy, 2, 14, nozzle_icon);
      u8g.drawStr(cx + ICON_WIDTH, cy, itostr3left(thermalManager.degHotend(e)));
      if (idle) {
        u8g.drawStr(cx + ICON_WIDTH, cy+FONT_CHAR_HEIGHT - 1, "o"); //upper o, i.e., ˚C
        u8g.drawStr(cx + ICON_WIDTH + FONT_CHAR_WIDTH, cy+FONT_CHAR_HEIGHT, "C");
      }
      else
        u8g.drawStr(cx + ICON_WIDTH, cy+FONT_CHAR_HEIGHT, itostr3left(targ));
      SCR_BLOCK_POS_CTRL;
    }

    // bed
    #if HAS_HEATED_BED
    const int16_t targ = thermalManager.degTargetBed();
    const bool idle = thermalManager.is_bed_idle() || targ == 0;
    u8g.drawBitmapP(cx, cy, 2, 14, bed_icon);
    u8g.drawStr(cx + ICON_WIDTH, cy, itostr3left(thermalManager.degBed()));
    if (idle) {
      u8g.drawStr(cx + ICON_WIDTH, cy+FONT_CHAR_HEIGHT - 1, "o"); //upper o, i.e., ˚C
      u8g.drawStr(cx + ICON_WIDTH + FONT_CHAR_WIDTH, cy+FONT_CHAR_HEIGHT, "C");
    }
    else
      u8g.drawStr(cx + ICON_WIDTH, cy+FONT_CHAR_HEIGHT, itostr3left(targ));
    SCR_BLOCK_POS_CTRL;
    #endif

    // fan0
    #if HAS_FAN0
    u8g.drawBitmapP(cx, cy, 2, 14, fan_icon);
    const int16_t per = ((fanSpeeds[0] + 1) * 100) / 256;
    u8g.setPrintPos(cx + ICON_WIDTH, cy);
    u8g.print(itostr3left(per));
    if (per < 100) u8g.print("%");
    else u8g.drawStr(cx + ICON_WIDTH, cy + FONT_CHAR_HEIGHT, " %");
    SCR_BLOCK_POS_CTRL;
    #endif

    // feedrate
    u8g.drawBitmapP(cx, cy, 2, 14, feedrate_icon);
    u8g.setPrintPos(cx + ICON_WIDTH, cy);
    u8g.print(itostr3left(feedrate_percentage));
    if (feedrate_percentage < 100) u8g.print("%");
    else u8g.drawStr(cx + ICON_WIDTH, cy + FONT_CHAR_HEIGHT, " %");
    SCR_BLOCK_POS_CTRL;

    // sd printing and time
    #if ENABLED(SDSUPPORT) || ENABLED(LCD_SET_PROGRESS_MANUALLY)
      #if DISABLED(LCD_SET_PROGRESS_MANUALLY)
        const uint8_t progress_bar_percent = card.percentDone();
      #endif
      u8g.drawFrame(cx, cy, 5, 14);  // frame
      uint16_t done_height = progress_bar_percent * 0.13 + 0.12;
      NOLESS(done_height, 1);
      u8g.drawBox(cx+1, cy + 14 - done_height, 3, done_height); // solid part
      u8g.setPrintPos(cx + 7, cy);
      u8g.print(itostr3left(progress_bar_percent));
      u8g.print('%');
      // time elapsed
      duration_t elapsed = print_job_timer.duration();
      bool has_days = (elapsed.value >= 60*60*24L);
      if (has_days) {
        // day goes right to percent
        u8g.print(itostr3left(elapsed.day()));
        u8g.print('d');
      }
      char buffer[10];
      elapsed.toDigital(buffer, false);
      u8g.setPrintPos(cx + 7, cy + FONT_CHAR_HEIGHT);
      u8g.print(buffer);
    #endif

    // x,y,z position
    char *aux;
    #define PRINT_IGN_SPACES(S) do { \
      aux = S; \
      while (*aux == ' ') aux++; \
      u8g.print(aux); \
    } while (0)

    static char xstring[5], ystring[5], zstring[7];
    if (page.page == 0) {
      strcpy(xstring, ftostr4sign(LOGICAL_X_POSITION(current_position[X_AXIS])));
      strcpy(ystring, ftostr4sign(LOGICAL_Y_POSITION(current_position[Y_AXIS])));
      strcpy(zstring, ftostr52sp(LOGICAL_Z_POSITION(current_position[Z_AXIS])));
    }
    cy = LCD_PIXEL_HEIGHT - FONT_CHAR_HEIGHT*2 - 3;
    u8g.setPrintPos(0, cy);
    u8g.print("Z");
    if (!TEST(axis_homed, Z_AXIS)) u8g.print(" ? ");
    else PRINT_IGN_SPACES(zstring);
    u8g.print(" X");
    if (!TEST(axis_homed, X_AXIS)) u8g.print(" ? ");
    else PRINT_IGN_SPACES(xstring);
    u8g.print(" Y");
    if (!TEST(axis_homed, Y_AXIS)) u8g.print(" ? ");
    else PRINT_IGN_SPACES(ystring);

    // message
    cy = LCD_PIXEL_HEIGHT - FONT_CHAR_HEIGHT - 2;
    #if !ENABLED(STATUS_MESSAGE_SCROLLING)
      u8g.drawStr(0, cy, lcd_status_message);
    #else
      // string fit
      if (u8g.getStrWidth(lcd_status_message) <= LCD_PIXEL_WIDTH) {
        u8g.drawStr(0, cy, lcd_status_message);
      }
      else { // string larger than screen width
        const char *stat = lcd_status_message + status_scroll_offset;
        u8g.drawStr(0, cy, stat);

        if (page.page == 0 && lcd_blink()) {
          if (u8g.getStrWidth(stat) <= LCD_PIXEL_WIDTH)
            status_scroll_offset = 0; // message displayed, restart from 0
          else
            status_scroll_offset++; // message not displayed completely
        }
      }
    #endif

  } while (u8g.nextPage());
}

// Initialize or re-initialize the LCD
static void lcd_implementation_init() {

  #if PIN_EXISTS(LCD_RESET)
    u8g.begin();
  #endif

  u8g.setContrast(lcd_contrast);
  u8g.setFont(FONT_TEXT_NAME);
  u8g.setFontPosTop();
  u8g.setColorIndex(1); // Instructs the display to draw with a pixel on.

  #if ENABLED(LCD_SCREEN_ROT_90)
    u8g.setRot90();   // Rotate screen by 90°
  #elif ENABLED(LCD_SCREEN_ROT_180)
    u8g.setRot180();  // Rotate screen by 180°
  #elif ENABLED(LCD_SCREEN_ROT_270)
    u8g.setRot270();  // Rotate screen by 270°
  #endif
}

// The kill screen is displayed for unrecoverable conditions
void lcd_kill_screen() {
  const uint8_t h4 = u8g.getHeight() / 4;
  u8g.firstPage();
  do {
    u8g.drawStr(0, h4 * 1, lcd_status_message);
    u8g.drawStrP(0, h4 * 2, PSTR(MSG_HALTED));
    u8g.drawStrP(0, h4 * 3, PSTR(MSG_PLEASE_RESET));
  } while (u8g.nextPage());
}

#if ENABLED(ULTIPANEL)

  uint8_t row_y1, row_y2;
  uint8_t constexpr row_height = FONT_CHAR_HEIGHT + 1;

//static void lcd_implementation_drawmenu_all(const bool isSelected, const uint8_t row, const char* pstr,
//const char* const data, const bool pgm, const bool einline, const bool center)

  #define lcd_implementation_drawmenu_back(sel, row, pstr, dummy) lcd_implementation_drawmenu_generic(sel, row, pstr, "<", false, true, false)
  #define lcd_implementation_drawmenu_submenu(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, ">", false, true, false)
  #define lcd_implementation_drawmenu_gcode(sel, row, pstr, gcode) lcd_implementation_drawmenu_generic(sel, row, pstr, NULL, false, true, false)
  #define lcd_implementation_drawmenu_function(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, NULL, false, true, false)

  // Macros for edit items
  #define DRAWMENU_SETTING_EDIT_GENERIC(_src) lcd_implementation_drawmenu_generic(sel, row, pstr, _src, false, true, false)
  #define DRAW_BOOL_SETTING(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, (*(data))?PSTR(MSG_ON):PSTR(MSG_OFF), true, true, false)

  void print_pgm_str(const char* pstr) {
    while (char c = pgm_read_byte(pstr)) {
      u8g.print(c);
      pstr++;
    }
  }

  static void lcd_implementation_drawmenu_generic(const bool isSelected, const uint8_t row, const char* pstr,
    const char* const data, const bool pgm, const bool einline, const bool center) {

    uint8_t cy = row * row_height;
    uint8_t cx = 0;

    if (isSelected) {
      u8g.drawBox(0, cy, LCD_PIXEL_WIDTH, FONT_CHAR_HEIGHT+1);
      u8g.setColorIndex(0);
      cx = 1;
    }

    if (center)
       cx = (LCD_PIXEL_WIDTH - strlen_P(pstr) * FONT_CHAR_WIDTH) / 2;

    u8g.setPrintPos(cx, cy);
    print_pgm_str(pstr);

    if (data) {
      if (!einline)
        cy += row_height;

      const uint8_t vallen = (pgm ? u8g.getStrWidthP((unsigned char*)data) : u8g.getStrWidth((char*)data));
      cx = LCD_PIXEL_WIDTH - vallen - 1;

      // clear. workaround for large menu text drawing over value (Z Probe Offset ....)
      u8g.setColorIndex(isSelected ? 1 : 0);
      u8g.drawBox(cx-2, cy, vallen, FONT_CHAR_HEIGHT);
      u8g.setColorIndex(isSelected ? 0 : 1);

      if (pgm) {
        u8g.setPrintPos(cx, cy);
        print_pgm_str(data);
      }
      else
         u8g.drawStr(cx, cy, (char*)data);
    }

    if (isSelected) // restore
      u8g.setColorIndex(1);
  }

  static void lcd_implementation_drawmenu_static(const uint8_t row, const char* pstr, const bool center=true, const bool invert=false, const char* valstr=NULL) {
    lcd_implementation_drawmenu_generic(invert, row, pstr, valstr, false, false, center);
  }

  static void lcd_implementation_drawedit(const char* const pstr, const char* const value=NULL) {
     lcd_implementation_drawmenu_generic(false, 0, pstr, value, false, false, false);
  }

  #if ENABLED(ADVANCED_PAUSE_FEATURE)

    static void lcd_implementation_hotend_status(const uint8_t row, const uint8_t extruder=active_extruder) {
      uint8_t cy = row * row_height;
      // TODO: Find correct pos, currently overlaping
      u8g.setPrintPos(LCD_PIXEL_WIDTH - 11 * (FONT_CHAR_WIDTH), cy);
      u8g.print('E');
      u8g.print((char)('1' + extruder));
      u8g.print(' ');
      u8g.print(itostr3(thermalManager.degHotend(extruder)));
      u8g.print('/');

      if (lcd_blink() || !thermalManager.is_heater_idle(extruder))
        u8g.print(itostr3(thermalManager.degTargetHotend(extruder)));
    }

  #endif // ADVANCED_PAUSE_FEATURE

  #if ENABLED(SDSUPPORT)

    static void _drawmenu_sd(const bool isSelected, const uint8_t row, const char* const pstr, CardReader& theCard, const bool isDir) {
      UNUSED(pstr);

      uint8_t cy = row * row_height;
      uint8_t cx = 0;

      if (isSelected) {
        u8g.drawBox(0, cy, LCD_PIXEL_WIDTH, FONT_CHAR_HEIGHT+1);
        u8g.setColorIndex(0);
        cx = 1;
      }

      u8g.setPrintPos(cx, cy);

      const char *stat = theCard.longest_filename();
      #if !ENABLED(SCROLL_LONG_FILENAMES)
        u8g.print(stat);
      #else
        if (theCard.longFilename[0]) {
          if (isSelected) {
            uint8_t name_hash = row;
            for (uint8_t l = FILENAME_LENGTH; l--;)
              name_hash = ((name_hash << 1) | (name_hash >> 7)) ^ theCard.filename[l];  // rotate, xor
            if (filename_scroll_hash != name_hash) {                            // If the hash changed...
              filename_scroll_hash = name_hash;                                 // Save the new hash
              filename_scroll_pos = 0;                                          // Reset scroll to the start
              uint8_t w = u8g.getStrWidth(theCard.longFilename);
              filename_scroll_max = w < LCD_PIXEL_WIDTH ? 0 : 2 + (w - LCD_PIXEL_WIDTH) / FONT_CHAR_WIDTH;
            }
            stat += filename_scroll_pos;
          }
        }
      #endif
      u8g.print(stat);

      if (isDir) {
        cx = LCD_PIXEL_WIDTH - (FONT_CHAR_WIDTH) * 2;
        u8g.drawStr(cx, cy, " >");
      }

      if (isSelected) // restore
        u8g.setColorIndex(1);
    }

    #define lcd_implementation_drawmenu_sdfile(sel, row, pstr, theCard) _drawmenu_sd(sel, row, pstr, theCard, false)
    #define lcd_implementation_drawmenu_sddirectory(sel, row, pstr, theCard) _drawmenu_sd(sel, row, pstr, theCard, true)

  #endif // SDSUPPORT


#endif // ULTIPANEL

#endif
