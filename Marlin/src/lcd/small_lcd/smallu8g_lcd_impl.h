/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * smallu8g_lcd_impl.h
 *
 * Graphics LCD implementation for small LCDS compatible with u8glib
 * Based on DOGM and ST7920 screen implementations
 *
 * With the use of:
 * u8glib by Oliver Kraus
 * https://github.com/olikraus/U8glib_Arduino
 * License: http://opensource.org/licenses/BSD-3-Clause
 */

#pragma once

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(SMALL_LCD)

#include "../../libs/numtostr.h"
#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"

#if ENABLED(SDSUPPORT)
  #include "../../libs/duration_t.h"
#endif

#if ENABLED(NOKIA5110_LCD)
  #define U8G_HW_SPI_2X
  #define LCD_PIXEL_WIDTH 84
  #define LCD_PIXEL_HEIGHT 48
  #define LCD_PAGE_HEIGHT 8

  uint8_t u8g_com_arduino_hw_spi_fn_fixpcd8544(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
  {
    uint8_t result = u8g_com_arduino_hw_spi_fn(u8g, msg, arg_val, arg_ptr);
    if (msg == U8G_COM_MSG_INIT) {
        SPCR = 0;
        // change scale to fclk/16
        SPCR =  (1<<SPE) | (1<<MSTR)|(0<<SPR1)|(1<<SPR0)|(0<<CPOL)|(0<<CPHA);
    }
    return result;
  }

  U8G_PB_DEV(u8g_dev_pcd8544_84x48_hw_spi_fix, LCD_PIXEL_WIDTH, LCD_PIXEL_HEIGHT, LCD_PAGE_HEIGHT,
    u8g_dev_pcd8544_84x48_hw_spi.dev_fn, u8g_com_arduino_hw_spi_fn_fixpcd8544);

  class U8GLIB_PDC8544_MRLN : public U8GLIB {
  public:
    U8GLIB_PDC8544_MRLN(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_pcd8544_84x48_sw_spi, sck, mosi, cs, a0, reset) {}
    U8GLIB_PDC8544_MRLN(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_pcd8544_84x48_hw_spi_fix, cs, a0, reset) {
    }
  };

  #if ENABLED(HARDWARE_SPI)
    U8GLIB_PDC8544_MRLN u8g(SLCD_CS_PIN, SLCD_A0_PIN);
  #else
    U8GLIB_PDC8544_MRLN u8g(SLCD_CLK_PIN, SLCD_DAT_PIN, SLCD_CS_PIN, SLCD_A0_PIN);
  #endif
#endif

// The current graphical page being rendered
u8g_page_t &page = ((u8g_pb_t *)((u8g.getU8g())->dev->dev_mem))->p;
// For selective rendering within a Y range
#define PAGE_UNDER(yb) (u8g.getU8g()->current_page.y0 <= (yb))
#define PAGE_CONTAINS(ya, yb) (PAGE_UNDER(yb) && u8g.getU8g()->current_page.y1 >= (ya))

//#include "dogm_font_data_ISO10646_1.h"
//#define FONT_TEXT_NAME ISO10646_1_5x7
//#define FONT_TEXT_NAME u8g_font_5x7r
#define FONT_TEXT_NAME u8g_font_04b_03r
#define FONT_CHAR_HEIGHT 7
#define FONT_CHAR_WIDTH 5

#define MENU_FONT_WIDTH FONT_CHAR_WIDTH
#define MENU_FONT_HEIGHT 7

// status screen block width: icon plus three chars and a space
#define BLOCK_WIDTH (ICON_WIDTH + FONT_CHAR_WIDTH*3 + 1)

bool MarlinUI::detected() { return true; }
void MarlinUI::set_font(const MarlinFont font_nr) { }

#if HAS_LCD_CONTRAST

  int16_t MarlinUI::contrast = DEFAULT_LCD_CONTRAST;

  void MarlinUI::set_contrast(const int16_t value) {
    contrast = constrain(value, LCD_CONTRAST_MIN, LCD_CONTRAST_MAX);
    u8g.setContrast(contrast);
  }

#endif

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
  void MarlinUI::show_bootscreen() {
    constexpr uint8_t offy = (LCD_PIXEL_HEIGHT - START_BMPHEIGHT - 2*FONT_CHAR_HEIGHT) / 2;

    const uint8_t width = LCD_PIXEL_WIDTH,
                  offx = (width - (START_BMPWIDTH)) / 2;

    u8g.firstPage();
    do {
      u8g.drawBitmapP(offx, offy, START_BMP_BYTEWIDTH, START_BMPHEIGHT, start_bmp);
      const uint8_t txt1X = (width - (sizeof(SHORT_BUILD_VERSION) - 1) * (FONT_CHAR_WIDTH)) / 2;
      u8g.drawStr(txt1X, START_BMPHEIGHT + offy, SHORT_BUILD_VERSION);
      #ifdef STRING_CONFIG_H_AUTHOR
        const uint8_t txt2X = (width - (sizeof(STRING_CONFIG_H_AUTHOR) - 1) * (FONT_CHAR_WIDTH)) / 2;
        u8g.drawStr(txt2X, START_BMPHEIGHT + offy + FONT_CHAR_HEIGHT, STRING_CONFIG_H_AUTHOR);
      #endif
    } while (u8g.nextPage());

    safe_delay(100);
  }

#endif // SHOW_BOOTSCREEN

void smallu8g_draw_hotend_status(const int8_t e, const int16_t target, const float degree, 
  const uint8_t cx, const uint8_t cy, const uint8_t icon[]) {

  const bool idle = thermalManager.hotend_idle[e].timed_out || target == 0;
  u8g.drawBitmapP(cx, cy, 2, 14, icon);
  u8g.drawStr(cx + ICON_WIDTH, cy, i16tostr3left(degree));
  if (idle) {
    u8g.drawStr(cx + ICON_WIDTH, cy+FONT_CHAR_HEIGHT - 1, "o"); //upper o, i.e., ˚C
    u8g.drawStr(cx + ICON_WIDTH + FONT_CHAR_WIDTH, cy+FONT_CHAR_HEIGHT, "C");
  }
  else
    u8g.drawStr(cx + ICON_WIDTH, cy+FONT_CHAR_HEIGHT, i16tostr3left(target));
}

void MarlinUI::draw_status_screen() {
  static bool last_blink = false;
  u8g.firstPage();
  do {
    uint8_t cx = 0;
    uint8_t cy = 0;

    #define SCR_BLOCK_POS_CTRL do { \
      cx += BLOCK_WIDTH; \
      if (LCD_PIXEL_WIDTH - cx < BLOCK_WIDTH) { \
        cy += ICON_HEIGHT+2; \
        cx = 0; \
      } \
    } while (0)

    // hotends
    HOTEND_LOOP() {
      smallu8g_draw_hotend_status(e, thermalManager.degTargetHotend(e), 
        thermalManager.degHotend(e), cx, cy, nozzle_icon);
      SCR_BLOCK_POS_CTRL;
    }

    // bed
    #if HAS_HEATED_BED
    smallu8g_draw_hotend_status(H_BED, thermalManager.degTargetBed(),
      thermalManager.degBed(), cx, cy, bed_icon);
    SCR_BLOCK_POS_CTRL;
    #endif

    // fan0
    #if HAS_FAN0
    u8g.drawBitmapP(cx, cy, 2, 14, fan_icon);
    const int16_t per = ((thermalManager.fan_speed[0] + 1) * 100) / 256;
    u8g.setPrintPos(cx + ICON_WIDTH, cy);
    u8g.print(i16tostr3left(per));
    if (per < 100) u8g.print("%");
    else u8g.drawStr(cx + ICON_WIDTH, cy + FONT_CHAR_HEIGHT, " %");
    SCR_BLOCK_POS_CTRL;
    #endif

    // feedrate
    u8g.drawBitmapP(cx, cy, 2, 14, feedrate_icon);
    u8g.setPrintPos(cx + ICON_WIDTH, cy);
    u8g.print(i16tostr3left(feedrate_percentage));
    if (feedrate_percentage < 100) u8g.print("%");
    else u8g.drawStr(cx + ICON_WIDTH, cy + FONT_CHAR_HEIGHT, " %");
    SCR_BLOCK_POS_CTRL;

    // sd printing and time
    #if ENABLED(SDSUPPORT) || ENABLED(LCD_SET_PROGRESS_MANUALLY)
      #if DISABLED(LCD_SET_PROGRESS_MANUALLY)
        const uint8_t progress_bar_percent = card.percentDone();
      #endif
      u8g.drawFrame(cx, cy, 5, 14);  // frame
      int16_t done_height = progress_bar_percent * 0.13 + 0.12;
      NOLESS(done_height, 1);
      u8g.drawBox(cx+1, cy + 14 - done_height, 3, done_height); // solid part
      u8g.setPrintPos(cx + 7, cy);
      u8g.print(i16tostr3left(progress_bar_percent));
      u8g.print('%');
      // time elapsed
      duration_t elapsed = print_job_timer.duration();
      bool has_days = (elapsed.value >= 60*60*24L);
      if (has_days) {
        // day goes right to percent
        u8g.print(i16tostr3left(elapsed.day()));
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
      u8g.drawStr(0, cy, status_message);
    #else
      // string fit
      if (u8g.getStrWidth(status_message) <= LCD_PIXEL_WIDTH) {
        u8g.drawStr(0, cy, status_message);
      }
      else { // string larger than screen width
        uint8_t rlen;
        const char *stat = status_and_len(rlen);
        u8g.drawStr(0, cy, stat);
        bool blink = get_blink();
        if (last_blink != blink) {
          last_blink = blink;
          advance_status_scroll();
        }
      }
    #endif

  } while (u8g.nextPage());
}

// Initialize or re-initialize the LCD
void MarlinUI::init_lcd() {

  #if PIN_EXISTS(LCD_RESET)
    u8g.begin();
  #endif

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
void MarlinUI::draw_kill_screen() {
  const uint8_t h4 = u8g.getHeight() / 4;
  u8g.firstPage();
  do {
    u8g.drawStr(0, h4 * 1, status_message);
    u8g.drawStr(0, h4 * 2, GET_TEXT(MSG_HALTED));
    u8g.drawStr(0, h4 * 3, GET_TEXT(MSG_PLEASE_RESET));
  } while (u8g.nextPage());
}

void MarlinUI::clear_lcd() { }

#if HAS_LCD_MENU

  #include "../menu/menu.h"

  uint8_t row_y1, row_y2;
  uint8_t constexpr row_height = FONT_CHAR_HEIGHT + 1;

  void print_pgm_str(const char* pstr) {
    while (char c = pgm_read_byte(pstr)) {
      u8g.print(c);
      pstr++;
    }
  }

  void __draw(const bool sel, const uint8_t row, PGM_P const pstr, const char * const data,
    const bool pgm, const bool einline, const bool center) {
    uint8_t cy = row * row_height;
    uint8_t cx = 0;

    if (!PAGE_CONTAINS(cy, cy + row_height)) return;

    if (sel) {
      u8g.drawBox(0, cy, LCD_PIXEL_WIDTH, row_height);
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
      u8g.setColorIndex(sel ? 1 : 0);
      u8g.drawBox(cx-2, cy, vallen, FONT_CHAR_HEIGHT);
      u8g.setColorIndex(sel ? 0 : 1);

      if (pgm) {
        u8g.setPrintPos(cx, cy);
        print_pgm_str(data);
      }
      else
         u8g.drawStr(cx, cy, (char*)data);
    }

    if (sel) // restore
      u8g.setColorIndex(1);
  }

  void MenuItemBase::_draw(const bool sel, const uint8_t row, PGM_P const pstr, const char, const char post_char) {
    __draw(sel, row, pstr, NULL, false, true, false);
  }
  
  void MenuItem_static::draw(const uint8_t row, PGM_P const pstr, const uint8_t style/*=SS_DEFAULT*/, const char * const valstr/*=nullptr*/) {
    __draw(false, row, pstr, valstr, false, true, false);
  }

  void MenuEditItemBase::draw(const bool sel, const uint8_t row, PGM_P const pstr, const char* const data, const bool pgm) {
    __draw(sel, row, pstr, data, pgm, true, false);
  }
  
  void MenuEditItemBase::draw_edit_screen(PGM_P const pstr, const char* const value/*=nullptr*/) {
    __draw(false, 0, pstr, value, false, false, false);
  }

  inline void draw_boxed_string(const u8g_uint_t x, const u8g_uint_t y, PGM_P const pstr, const bool inv) {
    if (inv) {
      u8g.setColorIndex(1);
      u8g.drawBox(x - 1, y - FONT_CHAR_HEIGHT, LCD_PIXEL_WIDTH/2-1, y);
      u8g.setColorIndex(0);
    }
    u8g.setPrintPos(x, y - FONT_CHAR_HEIGHT);
    print_pgm_str(pstr);
    if (inv) u8g.setColorIndex(1);
  }

  void MenuItem_confirm::draw_select_screen(PGM_P const yes, PGM_P const no, const bool yesno, PGM_P const pref, const char * const string/*=nullptr*/, PGM_P const suff/*=nullptr*/) {
    ui.draw_select_screen_prompt(pref, string, suff);
    draw_boxed_string(1, LCD_PIXEL_HEIGHT - 1, no, !yesno);
    draw_boxed_string(LCD_PIXEL_WIDTH/2+1, LCD_PIXEL_HEIGHT - 1, yes, yesno);
  }

  #if ENABLED(ADVANCED_PAUSE_FEATURE)

    void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
      uint8_t cx = LCD_PIXEL_WIDTH / 2 - BLOCK_WIDTH;
      uint8_t cy = LCD_PIXEL_HEIGHT / 2;
      // clean
      u8g.setColorIndex(0);
      u8g.drawBox(0, cy, LCD_PIXEL_WIDTH, LCD_PIXEL_HEIGHT);
      u8g.setColorIndex(1);
      // draw nozzle icon and temperatures
      smallu8g_draw_hotend_status(extruder, thermalManager.degTargetHotend(extruder),
        thermalManager.degHotend(extruder), cx, cy, nozzle_icon);
    }

  #endif // ADVANCED_PAUSE_FEATURE

  #if ENABLED(SDSUPPORT)

    void MenuItem_sdbase::draw(const bool sel, const uint8_t row, PGM_P const, CardReader &theCard, const bool isDir) {
      uint8_t cy = row * row_height + 1;
      uint8_t cx = 0;

      if (!PAGE_CONTAINS(cy, cy + FONT_CHAR_HEIGHT - 1)) return;

      if (sel) {
        u8g.drawBox(0, cy, LCD_PIXEL_WIDTH, row_height);
        u8g.setColorIndex(0);
        cx = 1;
      }

      const char *stat = ui.scrolled_filename(theCard, LCD_PIXEL_WIDTH/FONT_CHAR_WIDTH, row, sel);
      u8g.drawStr(cx, cy, stat);

      if (isDir) {
        cx = LCD_PIXEL_WIDTH - (FONT_CHAR_WIDTH) * 2;
        u8g.drawStr(cx, cy, " >");
      }

      if (sel) // restore
        u8g.setColorIndex(1);
    }

  #endif // SDSUPPORT

#endif // HAS_LCD_MENU

#endif // SMALL_LCD
