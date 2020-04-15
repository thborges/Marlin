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
 * Graphic screen implementation for SSD1331 OLED
 * by Thiago Borges de Oliveira, Nov-Dez 2019
 */

#pragma once

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(SSD1331_LCD)

#include "../../libs/numtostr.h"
#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"

#if ENABLED(SDSUPPORT)
  #include "../../libs/duration_t.h"
#endif

#ifdef __AVR__
  //#include "smallucg_HAL_AVR.h"
  #include "ssd1331_ga.h"
#else
  #error "Currently, SSD1331 LCD only works in AVR platform"
#endif

#define ACTION_ON_CANCEL

#if ENABLED(HARDWARE_SPI)
  //Ucglib_SSD1331_HWSPI_MRLN ucg(SLCD_A0_PIN, SLCD_CS_PIN, SLCD_RESET_PIN);
  SSD1331GA ucg(SLCD_CS_PIN, SLCD_RST_PIN);
#else
  SSD1331GA ucg(SLCD_CLK_PIN, SLCD_DAT_PIN, SLCD_CS_PIN, SLCD_RST_PIN);
  //#error "Currently, only hardware SPI is supported."
#endif

#define FONT_TEXT_NAME ucg_font_04b_03_hr

uint8_t constexpr row_height = FONT_CHAR_HEIGHT + 3;

// status screen block width: icon plus three chars and two spaces
#define BLOCK_WIDTH (ICON_WIDTH + FONT_CHAR_WIDTH*3 + 3)
#define BLOCK_TXT_WIDTH  (FONT_CHAR_WIDTH*3 + 3)

bool clear_screen = true;
bool MarlinUI::detected() { return true; }
void MarlinUI::set_font(const MarlinFont font_nr) { }

typedef struct { uint8_t r, g, b; } color;

// although using 16bits, icons occupies 12 pixels
// the remaining zeros are overlaped with text later
#define ICON_WIDTH 13
#define ICON_HEIGHT 14

typedef struct {
  union {
    uint16_t word;
    struct {
      uint8_t is_a_color: 1;
      uint8_t b: 5;
      uint8_t g: 5;
      uint8_t r: 5;
    };
  };
} icon_word;

/* color format on icons below is
 an uint16 ending on 1 (0b1), example: 
 a byte representing color change
 0b[ red ] [ green ] [ blue ] [is_a_colour]
 0b 00000    00000     00000  1
*/

#define YELLOW 0b0000000000100001 //blue
#define ORANGE 0b1111101011000001
#define AUX_ORANGE 248, 88, 0
#define RED    0b1111100000000001

#define TEXT_COLOR      127, 127,   0
#define TEXT_COLOR_WARM 248,  90,   0
#define TEXT_COLOR_HEAT 150,   0,  50
#define HIGHLIGHT_TEXT    0,   0, 128
#define PGBAR_COLOR       0,   0, 128
#define PGBARF_COLOR       AUX_ORANGE
#define MESSAGE_COLOR   127,   0, 127
#define EDIT_COLOR      HIGHLIGHT_TEXT

const uint16_t nozzle_icon[] PROGMEM = {
  YELLOW, 0b0011111100000000,
          0b0011111100000000,
          0b0011001100000000,
          0b0011001100000000,
          0b0011001100000000,
          0b1100000011000000,
          0b1100000011000000,
          0b1100000011000000,
          0b0111001110000000,
          0b0001111000000000,
          0b0000110000000000, 
  ORANGE, 0b0000100000000000,
          0b0111000000000000
};

const uint16_t bed_icon[] PROGMEM = {
  ORANGE, 0b0000100010000000,
          0b0001000100000000,
          0b0010001000000000,
          0b0010001000000000,
          0b0001000100000000,
          0b0000100010000000,
          0b0000100010000000,
          0b0001000100000000,
          0b0000000000000000,
          0b0000000000000000,
  YELLOW, 0b1111111111100000,
          0b1111111111100000,
          0b1100000001100000
};

const uint16_t fan_icon[] PROGMEM = {
  YELLOW, 0b0000000000000000,
          0b0010000010000000,
          0b0111000111000000,
          0b1111000111100000,
          0b0111101111000000,
  ORANGE, 0b0000111000000000,
          0b0000111000000000,
  YELLOW, 0b0111101111000000,
          0b1111000111100000,
          0b0111000111000000,
          0b0010000010000000
};

// it is supposed to be a velocimeter
const uint16_t feedrate_icon[] PROGMEM = {
  YELLOW, 0b0000000000000000,
          0b0001111100000000,
          0b0000000011000000,
          0b0100000011100000,
          0b1000000000000000,
          0b1000000001110000,
          0b0000001001110000, // these two pixels in the center
          0b1000010001110000, // are replaced later
  ORANGE, 0b0000110000000000,
          0b0011100000000000,
          0b0100100000000000,
          0b0011000000000000
};

const uint16_t folder_icon[] PROGMEM = {
  ORANGE, 0b1110000000000000,
          0b1001100000000000,
          0b1000100000000000,
          0b1001111100000000,
          0b1010001000000000,
          0b1100010000000000,
          0b1111100000000000,
};

const uint16_t file_icon[] PROGMEM = {
  YELLOW, 0b1111110000000000,
          0b1000111000000000,
          0b1000111100000000,
          0b1000000100000000,
          0b1000000100000000,
          0b1000000100000000,
          0b1111111100000000,
};

void drawBitmapP(uint8_t off_x, uint8_t off_y, const uint16_t *bmp, const uint8_t bmp_bytes) {
  uint8_t b = 0;
  do {
    uint16_t w = pgm_read_word(bmp+b);
    icon_word *iw = (icon_word*)&w;
    if (iw->is_a_color)
      ucg.setColor(iw->r << 3, iw->g << 3, iw->b << 3);
    else {
      uint8_t r = 0;
      while(iw->word) {
        while((iw->word & 0x8000) == 0) { r++; iw->word <<= 1; }
        uint8_t on = r;
        while((iw->word & 0x8000) != 0) { r++; iw->word <<= 1; }
        uint8_t off = r-1;
        ucg.drawLine(off_x+on, off_y, off_x+off, off_y, CI_FOREGROUND);
      }
      off_y++;
    }
    b++;
  } while (b < bmp_bytes / sizeof(uint16_t));
}

#if ENABLED(SHOW_BOOTSCREEN)
  const color MARLIN_BOOT_BITMAP_COLOURS[] PROGMEM = {
    {0,0,26}, {0,0,171}, {30,0,237}, {0,0,97}, {2,1,58}, {0,0,133}, {36,42,39}, {0,249,197},
    {0,101,80}, {0,171,137}, {84,86,83}, {156,158,159}, {203,205,202}, {244,247,244}, {92,91,197}, 
    {0,0,0}};

  // a kind of indexed image, using 255 as color separator (x,y,x,y,...,255,x,y,x,y...,255)
  const uint8_t MARLIN_BOOT_BITMAP[] PROGMEM = {
    74,0,72,1,74,3,69,7,77,10,83,12,83,14,79,16,83,17,59,18,60,19,83,19,84,19,86,
    20,89,20,59,21,53,22,87,22,90,22,63,23,59,24,61,24,64,24,62,25,64,25,91,25,59,
    27,85,27,63,28,67,28,73,28,53,29,86,30,89,31,89,32,89,33,89,34,89,35,89,36,89,
    37,91,38,90,40,85,44,90,46,67,48,86,49,91,49,84,51,92,52,88,55,90,60,92,62,255,
    73,1,72,2,75,8,82,10,77,11,70,12,71,12,72,12,75,12,79,12,82,12,68,13,76,13,77,
    13,79,14,80,14,69,15,78,15,80,15,81,15,80,16,81,16,85,16,64,17,86,17,58,19,59,
    19,86,19,53,20,57,20,69,20,48,21,49,21,57,21,69,21,69,22,70,22,69,23,70,23,85,
    23,88,23,58,24,70,24,85,24,59,25,57,26,67,26,69,26,89,26,69,27,72,27,89,27,54,
    28,86,28,68,29,72,29,91,34,91,35,91,36,90,37,84,44,75,45,76,45,77,45,79,45,80,
    45,89,45,69,46,89,47,90,48,90,51,91,51,92,54,92,55,89,56,92,60,91,62,255,73,2,
    72,3,73,3,71,4,72,4,73,4,71,5,72,5,73,5,70,6,71,6,72,6,73,6,74,6,70,7,71,7,72,
    7,73,7,74,7,70,8,71,8,72,8,73,8,74,8,70,9,71,9,72,9,73,9,74,9,75,9,70,10,71,10,
    72,10,73,10,74,10,75,10,76,10,83,10,84,10,70,11,71,11,72,11,73,11,74,11,75,11,
    76,11,80,11,81,11,76,12,77,12,78,12,69,13,70,13,71,13,72,13,73,13,74,13,75,13,
    78,13,79,13,67,14,68,14,69,14,70,14,71,14,72,14,73,14,74,14,75,14,76,14,77,14,
    78,14,66,15,67,15,68,15,79,15,83,15,65,16,66,16,84,16,85,17,86,18,87,19,54,20,
    55,20,56,20,87,20,88,20,50,21,51,21,52,21,53,21,54,21,55,21,56,21,88,21,58,22,
    59,22,60,22,88,22,89,22,61,23,62,23,89,23,63,24,69,24,89,24,90,24,57,25,58,25,
    69,25,89,25,90,25,56,26,68,26,90,26,55,27,62,27,63,27,64,27,65,27,66,27,67,27,
    90,27,69,28,72,28,90,28,69,29,71,29,90,29,90,30,90,31,90,32,90,33,90,34,90,35,
    90,36,71,45,72,45,73,45,74,45,81,45,82,45,83,45,84,45,70,46,89,46,83,49,84,49,
    90,49,84,50,85,50,86,50,90,50,86,51,87,51,87,52,88,52,91,52,88,53,89,53,91,53,
    89,54,90,54,91,54,89,55,90,55,91,55,90,56,91,56,92,56,90,57,91,57,92,57,90,58,
    91,58,92,58,91,59,92,59,91,60,91,61,255,71,3,74,5,75,7,69,9,69,10,81,10,69,11,
    69,12,81,12,80,13,65,15,77,15,84,15,64,16,82,16,63,17,61,18,83,18,85,18,57,19,
    88,19,47,20,48,20,49,20,58,20,45,21,46,21,70,21,55,22,56,22,61,22,85,22,59,23,
    60,23,62,24,88,24,56,25,61,25,70,25,58,26,60,26,61,26,66,26,85,26,86,26,91,26,
    60,27,64,28,89,28,86,29,92,38,89,39,90,39,89,40,85,43,84,46,69,47,82,48,82,49,
    85,49,91,50,89,52,92,53,92,61,255,74,4,69,8,86,10,83,11,66,14,76,15,82,15,68,
    16,83,16,65,17,81,17,84,17,62,18,63,18,56,19,69,19,42,20,44,21,54,22,57,24,86,
    25,59,26,62,26,63,26,64,26,65,26,56,27,53,28,68,28,70,29,89,29,89,30,92,34,92,
    35,89,38,89,41,84,43,89,43,72,46,83,46,83,48,89,49,83,50,86,52,87,53,89,57,91,
    63,255,70,5,76,9,85,10,79,11,82,11,73,12,74,12,80,12,81,13,81,14,82,14,67,16,
    82,17,60,18,87,18,43,20,44,20,45,20,46,20,50,20,51,20,52,20,47,21,58,21,87,21,
    89,21,57,22,90,23,60,25,68,25,85,25,55,26,54,27,61,27,68,27,70,27,71,27,86,27,
    91,27,65,28,66,28,70,28,71,28,91,28,91,29,91,30,91,31,91,32,91,33,91,37,90,38,
    85,42,83,44,89,44,70,45,78,45,71,46,68,47,90,47,89,48,87,50,85,51,88,51,90,52,
    90,53,88,54,90,59,255,62,21,80,22,80,23,83,25,75,27,82,27,39,31,76,31,41,32,70,
    32,51,33,87,33,43,34,48,34,39,35,53,35,58,35,39,36,59,36,61,36,75,36,39,37,79,
    37,39,38,63,38,70,38,83,38,88,38,39,39,63,39,70,39,88,39,39,40,52,40,57,40,63,
    40,71,40,83,40,36,41,39,41,63,41,68,41,83,41,41,42,57,42,71,42,81,42,40,43,41,
    43,42,43,43,43,44,43,45,43,46,43,47,43,48,43,49,43,50,43,51,43,52,43,53,43,54,
    43,55,43,56,43,57,43,58,43,59,43,60,43,61,43,62,43,63,43,64,43,65,43,66,43,67,
    43,68,43,69,43,70,43,71,43,72,43,73,43,74,43,75,43,76,43,77,43,78,43,79,43,80,
    43,81,43,38,44,255,63,21,63,22,83,27,79,28,80,28,81,28,82,28,83,28,82,29,83,29,
    84,29,86,34,86,35,255,64,21,62,22,64,22,81,23,81,24,82,24,82,26,76,27,77,27,78,
    27,79,27,80,27,81,27,84,27,74,28,80,29,85,33,87,34,87,35,87,36,86,37,255,82,25,
    83,26,75,28,76,28,77,28,78,28,84,28,81,29,86,33,85,34,86,36,87,37,87,38,255,38,
    30,37,31,70,31,44,32,47,32,73,32,36,33,40,33,36,34,42,34,72,34,73,34,36,35,62,
    35,66,35,76,35,36,36,67,36,82,36,36,37,52,37,57,37,63,37,65,37,75,37,36,38,54,
    38,66,38,67,38,77,38,36,39,52,39,77,39,36,40,60,40,74,40,77,40,53,41,57,41,77,
    41,40,42,45,42,46,42,50,42,51,42,55,42,56,42,58,42,59,42,60,42,61,42,62,42,70,
    42,72,42,73,42,74,42,75,42,76,42,80,42,84,42,37,43,39,43,255,39,30,38,31,42,32,
    43,32,48,32,49,32,72,32,37,33,79,33,37,34,44,34,47,34,49,34,80,34,37,35,54,35,
    73,35,80,35,81,35,37,36,41,36,81,36,37,37,41,37,59,37,61,37,67,37,77,37,83,37,
    37,38,41,38,52,38,61,38,75,38,37,39,41,39,54,39,61,39,75,39,37,40,41,40,55,40,
    56,40,68,40,70,40,37,41,41,41,71,41,83,42,82,43,39,44,40,44,41,44,42,44,43,44,
    44,44,45,44,46,44,47,44,48,44,49,44,50,44,51,44,52,44,53,44,54,44,55,44,56,44,
    57,44,58,44,59,44,60,44,61,44,62,44,63,44,64,44,65,44,66,44,67,44,68,44,69,44,
    70,44,71,44,255,40,30,41,30,42,30,43,30,44,30,45,30,46,30,47,30,48,30,49,30,50,
    30,51,30,52,30,53,30,54,30,55,30,56,30,57,30,58,30,59,30,60,30,61,30,62,30,63,
    30,64,30,65,30,66,30,67,30,68,30,69,30,70,30,71,30,72,30,73,30,74,30,75,30,76,
    30,68,31,77,31,37,32,78,32,84,32,45,33,46,33,68,33,73,33,84,33,51,34,68,34,84,
    34,41,35,57,35,68,35,72,35,77,35,84,35,46,36,50,36,53,36,56,36,64,36,68,36,73,
    36,78,36,84,36,46,37,50,37,54,37,68,37,73,37,81,37,84,37,46,38,50,38,59,38,68,
    38,73,38,81,38,84,38,46,39,50,39,59,39,68,39,73,39,84,39,46,40,50,40,61,40,73,
    40,75,40,84,40,46,41,50,41,84,41,37,42,38,43,83,43,255,77,30,78,30,79,30,80,30,
    81,30,82,30,83,30,84,30,69,31,78,31,79,31,80,31,81,31,82,31,83,31,84,31,68,32,
    69,32,79,32,80,32,81,32,82,32,83,32,41,33,42,33,43,33,44,33,47,33,48,33,49,33,
    50,33,69,33,72,33,80,33,81,33,82,33,83,33,40,34,41,34,45,34,46,34,50,34,69,34,
    81,34,82,34,83,34,40,35,45,35,46,35,50,35,51,35,55,35,56,35,63,35,64,35,65,35,
    69,35,78,35,79,35,82,35,83,35,40,36,45,36,51,36,54,36,55,36,57,36,58,36,62,36,
    63,36,65,36,66,36,69,36,72,36,76,36,77,36,79,36,80,36,83,36,40,37,45,37,51,37,
    53,37,58,37,62,37,66,37,69,37,72,37,76,37,80,37,40,38,45,38,51,38,53,38,58,38,
    62,38,69,38,72,38,76,38,80,38,40,39,45,39,51,39,53,39,58,39,62,39,69,39,72,39,
    76,39,80,39,81,39,40,40,45,40,51,40,53,40,54,40,58,40,59,40,62,40,69,40,72,40,
    76,40,80,40,81,40,40,41,45,41,51,41,54,41,55,41,56,41,58,41,59,41,60,41,61,41,
    62,41,69,41,70,41,72,41,73,41,74,41,75,41,76,41,80,41,81,41,255,72,44,73,44,74,
    44,75,44,76,44,77,44,78,44,79,44,80,44,81,44,82,44,255,};

  void draw_logo_bitmap(uint8_t offx, uint8_t offy, const uint8_t bitmap[], const color colors[]) {
    uint16_t i=0;
    color cc;
    int c = 0;
    do {
      memcpy_P(&cc, &colors[c], sizeof(color));
      if (cc.r == 0 && cc.g == 0 && cc.b == 0)
        break;
      c++;

      ucg.setColor(cc.r, cc.g, cc.b);
      uint8_t x=0,y;
      do {
          x = pgm_read_byte_near(&bitmap[i++]);
          if (x == 255) break;
          y = pgm_read_byte_near(&bitmap[i++]);
          ucg.drawPixel(x+offx, y+offy);
      } while(1);
    } while (1);
  }
 
  void MarlinUI::show_bootscreen() {
    draw_logo_bitmap(0, 0, MARLIN_BOOT_BITMAP, MARLIN_BOOT_BITMAP_COLOURS);
    ucg.setColor(TEXT_COLOR);
    ucg.drawString(0, 47, SHORT_BUILD_VERSION);
    ucg.drawString(0, 56, STRING_CONFIG_H_AUTHOR);
    safe_delay(500);
    clear_lcd();
  }
#endif // SHOW_BOOTSCREEN

void lcd_moveto(const lcd_uint_t col, const lcd_uint_t row) {  }

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
    ucg.clearRect(cx + adv, cy, width - adv - 1, FONT_CHAR_HEIGHT);
}

void smallucg_draw_hotend_status(const int8_t e, const int16_t target, const float degree, 
  const uint8_t cx, const uint8_t cy, const uint16_t icon[], const uint8_t icon_size) {

  const bool idle = thermalManager.hotend_idle[e].timed_out || target == 0;
  if (clear_screen) drawBitmapP(cx, cy, icon, icon_size);

  if (degree >= 60) ucg.setColor(TEXT_COLOR_HEAT);
  else if (degree >= 40) ucg.setColor(TEXT_COLOR_WARM);
  else ucg.setColor(TEXT_COLOR);

  uint8_t left = cx + ICON_WIDTH;
  if (idle)
    //font has speciall coded upper o, i.e., ˚ in pos 127
    write_str_fill_width(left, cy+FONT_CHAR_HEIGHT, "\x7F""C", BLOCK_TXT_WIDTH);
  else
    write_str_fill_width(left, cy+FONT_CHAR_HEIGHT, i16tostr3left(target), BLOCK_TXT_WIDTH);
  
  write_str_fill_width(left, cy, i16tostr3left(degree), BLOCK_TXT_WIDTH);
}

void MarlinUI::draw_status_screen() {
  static bool last_blink = false;
  static uint8_t screen_frag = 0;
  uint8_t cx = 0;
  uint8_t cy = 0;

  #define SCR_BLOCK_POS_CTRL do { \
    cx += BLOCK_WIDTH; \
    if (LCD_PIXEL_WIDTH - cx < BLOCK_WIDTH) { \
      cy += ICON_HEIGHT+5; \
      cx = 0; \
    } \
  } while (0)

  // hotends
  HOTEND_LOOP() {
    if (clear_screen || screen_frag == 0) {
      smallucg_draw_hotend_status(e, thermalManager.degTargetHotend(e), 
        thermalManager.degHotend(e), cx, cy, nozzle_icon, sizeof(nozzle_icon));
    }
    SCR_BLOCK_POS_CTRL;
  }

  // bed
  #if HAS_HEATED_BED
    if (clear_screen || screen_frag == 0) {
      smallucg_draw_hotend_status(H_BED, thermalManager.degTargetBed(),
        thermalManager.degBed(), cx, cy, bed_icon, sizeof(bed_icon));
    }
    SCR_BLOCK_POS_CTRL;
  #endif

  // fan0
  #if HAS_FAN0
  if (clear_screen || screen_frag == 0) {
    if (clear_screen) drawBitmapP(cx, cy, fan_icon, sizeof(fan_icon));
    ucg.setColor(TEXT_COLOR);
    if (clear_screen) ucg.drawString(cx + ICON_WIDTH, cy + FONT_CHAR_HEIGHT, "%");
    const int16_t per = ((thermalManager.fan_speed[0] + 1) * 100) / 256;
    write_str_fill_width(cx + ICON_WIDTH, cy, i16tostr3left(per), BLOCK_TXT_WIDTH);
  }
  SCR_BLOCK_POS_CTRL;
  #endif

  // feedrate
  if (clear_screen || screen_frag == 1) {
    if (clear_screen) {
      drawBitmapP(cx, cy, feedrate_icon, sizeof(feedrate_icon));
      ucg.setColor(AUX_ORANGE);
      ucg.drawPixel(cx+6, cy+6);
      ucg.drawPixel(cx+5, cy+7);
    }
    ucg.setColor(TEXT_COLOR);
    if (clear_screen) ucg.drawString(cx + ICON_WIDTH + 1, cy + FONT_CHAR_HEIGHT, "%");
    write_str_fill_width(cx + ICON_WIDTH + 1, cy, i16tostr3left(feedrate_percentage), BLOCK_TXT_WIDTH-1);
  }
  SCR_BLOCK_POS_CTRL;

  // sd printing and time
  #if EITHER(LCD_SET_PROGRESS_MANUALLY, SDSUPPORT)
  if (clear_screen || screen_frag == 1) {
    if (clear_screen) {
      ucg.setColor(PGBAR_COLOR);
      ucg.drawFrame(cx, cy, 5, 14);  // frame
    }
    uint8_t progress_bar_percent = ui.get_progress_percent();    
    int8_t undone = 13 - (progress_bar_percent * 0.13);
    if (undone > 0)
      ucg.clearRect(cx+1, cy+1, 2, undone-1); // empty part
    if (undone < 13) {
      ucg.setColor(CI_FOREGROUND, PGBARF_COLOR);
      ucg.setColor(CI_BACKGROUND, PGBARF_COLOR);
      ucg.drawBox(cx+1, cy+undone+1, 2, 13-undone-1); // filled part
      ucg.setColor(CI_BACKGROUND, 0, 0, 0);
    }
    cx += 8;
    ucg.setPrintPos(cx, cy);
    ucg.setColor(TEXT_COLOR);
    uint8_t wx = ucg.print(i16tostr3left(progress_bar_percent));
    ucg.drawString(cx + wx + 1, cy, "%");
    // time elapsed
    duration_t elapsed = print_job_timer.duration();
    bool has_days = (elapsed.value >= 60*60*24L);
    if (has_days) {
      // day goes right to percent
      ucg.print(i16tostr3left(elapsed.day()));
      ucg.print("d");
    }
    char buffer[10];
    elapsed.toDigital(buffer, false);
    ucg.setPrintPos(cx, cy + FONT_CHAR_HEIGHT);
    ucg.print(buffer);
  } // screen_frag == 1
  #endif

  if (clear_screen || screen_frag == 2) {
  // x,y,z position
  uint8_t w = 0;
  cy = LCD_PIXEL_HEIGHT - 2*(FONT_CHAR_HEIGHT+1);
  ucg.setPrintPos(0, cy);
  ucg.setColor(HIGHLIGHT_TEXT);
  w += ucg.print("Z ");
  ucg.setColor(TEXT_COLOR);
  w += ucg.print(ftostr41sign(LOGICAL_X_POSITION(current_position[Z_AXIS])));

  if (!printingIsActive()) {
    // print X and Y when not printing
    ucg.setColor(HIGHLIGHT_TEXT);
    w += ucg.print(" X ");
    ucg.setColor(TEXT_COLOR);
    w += ucg.print(ftostr41sign(LOGICAL_X_POSITION(current_position[X_AXIS])));
    ucg.setColor(HIGHLIGHT_TEXT);
    w += ucg.print(" Y ");
    ucg.setColor(TEXT_COLOR);
    w += ucg.print(ftostr41sign(LOGICAL_X_POSITION(current_position[Y_AXIS])));
  }
  ucg.clearRect(w, cy, LCD_PIXEL_WIDTH - w - 1, FONT_CHAR_HEIGHT);

  // message
  cy = LCD_PIXEL_HEIGHT - (FONT_CHAR_HEIGHT+1);
  ucg.setColor(MESSAGE_COLOR);
  #if !ENABLED(STATUS_MESSAGE_SCROLLING)
    write_str_fill_width(0, cy, status_message, LCD_PIXEL_WIDTH-1);
  #else
    // string fit
    uint8_t len = ucg.stringWidth(status_message);
    if (len <= LCD_PIXEL_WIDTH) {
      write_str_fill_width(0, cy, status_message, LCD_PIXEL_WIDTH-1);
    }
    else { // string larger than screen width
      uint8_t rlen;
      const char *stat = status_and_len(rlen);
      write_str_fill_width(0, cy, stat, LCD_PIXEL_WIDTH-1);
      bool blink = get_blink();
      if (last_blink != blink) {
        last_blink = blink;
        advance_status_scroll();
      }
    }
  #endif
  } // screen_frag == 2

  screen_frag++;
  if (screen_frag == 3) screen_frag = 0;
  clear_screen = false;
}

// Initialize or re-initialize the LCD
void MarlinUI::init_lcd() {

  ucg.begin();

  ucg.setColor(CI_FOREGROUND, 0, 0, 255); // default color blue
  ucg.setColor(CI_BACKGROUND, 0, 0, 0); // background color black
  ucg.clearScreen();

  drawing_screen = false;
  first_page = true;  // u8g concept, but scrolling in menus depend on this
}

// The kill screen is displayed for unrecoverable conditions
void MarlinUI::draw_kill_screen() {
  const uint8_t h4 = ucg.getHeight() / 4;
  ucg.clearScreen();
  ucg.setColor(TEXT_COLOR_HEAT);
  ucg.drawString(0, h4 * 1, status_message);
  ucg.setPrintPos(0, h4*2);
  ucg.printP(GET_TEXT(MSG_HALTED));
  ucg.setPrintPos(0, h4*3);
  ucg.printP(GET_TEXT(MSG_PLEASE_RESET));
}

void MarlinUI::clear_lcd() {
  ucg.clearScreen();
  clear_screen = true;
}

#if HAS_LCD_MENU

  #include "../menu/menu.h"

  uint8_t row_y1, row_y2;

  void __draw_text_line(const bool sel, const uint8_t row, PGM_P const pstr, const char * const data,
    const bool pgm, const bool einline, const bool pstr_pgm = true, const uint16_t *icon = NULL,
    const uint8_t icon_size = 0) {
    
    uint8_t cy = row * row_height + 1;
    uint8_t cx = 2;

    if (icon) {
      ucg.clearRect(cx-1, cy, 8/*ICON_WIDTH-1*/, row_height-1);
      drawBitmapP(cx, cy, icon, icon_size);
      cx = ICON_WIDTH;
    }
    
    // print main text
    if (sel) ucg.setColor(HIGHLIGHT_TEXT);
    else ucg.setColor(TEXT_COLOR);

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
        cy += row_height;
        w = 0;
        ucg.setColor(EDIT_COLOR);
      }
      if (pgm) {
        dataw = ucg.stringWidthP(data);
        ucg.setPrintPos(LCD_PIXEL_WIDTH - dataw - 2, cy);
        ucg.printP(data);
      } else {
        dataw = ucg.stringWidth(data);
        ucg.setPrintPos(LCD_PIXEL_WIDTH - dataw - 2, cy);
        ucg.print(data);
      }
    }

    // fill remaining empty background
    int8_t wf = LCD_PIXEL_WIDTH - dataw - w - 2;
    if (wf > 0) {
      ucg.clearRect(w, cy, wf-1, row_height-3);
      delay(1);
    }

    // selection border
    enum COLOR_INDEX idx = sel ? CI_FOREGROUND : CI_BACKGROUND;
    const uint8_t x0 = 0, y0 = cy-1, x1 = LCD_PIXEL_WIDTH-1, y1 = cy+row_height-2;
    ucg.drawLine(x0, y0, x0, y1, idx); // drawFrame not working!
    delay(2);
    ucg.drawLine(x0, y1, x1, y1, idx);
    delay(2);
    ucg.drawLine(x1, y1, x1, y0, idx);
    delay(2);
    ucg.drawLine(x0, y0, x1, y0, idx);
    delay(2);

    if (!pstr) { // text draw next using lcd_print
      ucg.setPrintPos(cx, cy);
    }
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
      ucg.setColor(HIGHLIGHT_TEXT);
      ucg.drawFrame(x - 1, y - FONT_CHAR_HEIGHT, LCD_PIXEL_WIDTH/2-1, row_height);
    } else {
      ucg.setColor(TEXT_COLOR);
      ucg.drawFrame(x - 1, y - FONT_CHAR_HEIGHT, LCD_PIXEL_WIDTH/2-1, row_height);
    }
    ucg.setPrintPos(x+2, y - FONT_CHAR_HEIGHT + 1);
    ucg.printP(pstr);
  }

  void MenuItem_confirm::draw_select_screen(PGM_P const yes, PGM_P const no, const bool yesno, PGM_P const pref, const char * const string/*=nullptr*/, PGM_P const suff/*=nullptr*/) {
    uint8_t cy = 0;
    ucg.setColor(TEXT_COLOR);
    ucg.setPrintPos(0, cy);
    ucg.printP(pref);
    if (string) {
      cy += FONT_CHAR_HEIGHT+1;
      ucg.drawString(0, cy, string);
    }
    if (suff) {
      cy += FONT_CHAR_HEIGHT+1;
      ucg.setPrintPos(0, cy);
      ucg.printP(suff);
    }
    //ui.draw_select_screen_prompt(pref, string, suff);
    draw_boxed_string(1, LCD_PIXEL_HEIGHT - 5, no, !yesno);
    draw_boxed_string(LCD_PIXEL_WIDTH/2+1, LCD_PIXEL_HEIGHT - 5, yes, yesno);
  }

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
      uint8_t cx = LCD_PIXEL_WIDTH / 2 - BLOCK_WIDTH;
      uint8_t cy = LCD_PIXEL_HEIGHT / 2;
      
      // draw nozzle icon and temperatures
      smallucg_draw_hotend_status(extruder, thermalManager.degTargetHotend(extruder),
        thermalManager.degHotend(extruder), cx, cy, nozzle_icon, sizeof(nozzle_icon));
    }

  #endif // ADVANCED_PAUSE_FEATURE

  #if ENABLED(SDSUPPORT)

    void MenuItem_sdbase::draw(const bool sel, const uint8_t row, PGM_P const, CardReader &theCard, const bool isDir) {
      const char *stat = ui.scrolled_filename(theCard, LCD_PIXEL_WIDTH/FONT_CHAR_WIDTH, row, sel);
      if (isDir)
        __draw_text_line(sel, row, stat, NULL, false, false, false, folder_icon, sizeof(folder_icon));
      else
        __draw_text_line(sel, row, stat, NULL, false, false, false, file_icon, sizeof(file_icon));
    }

  #endif // SDSUPPORT

#endif // HAS_LCD_MENU

#endif // SSD1331_LCD
