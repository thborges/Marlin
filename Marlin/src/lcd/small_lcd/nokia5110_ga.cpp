/*
 * A simple driver to PCD8544 a.k.a. NOKIA5110 LCD display controller
 * 
 * Author: Thiago Borges de Oliveira (thborges@gmail.com)
 * 
 * CC BY 4.0, Licensed under a Creative Commons Attribution 4.0 International license: 
 * http://creativecommons.org/licenses/by/4.0/
 */

#include "nokia5110_ga.h"

#if ENABLED(NOKIA5110_LCD)

NOKIA5110GA::NOKIA5110GA() {
  SET_OUTPUT(SLCD_CLK_PIN);
  SET_OUTPUT(SLCD_DAT_PIN);
  SET_OUTPUT(SLCD_CS_PIN);
  SET_OUTPUT(SLCD_DC_PIN);
  WRITE(SLCD_CLK_PIN, HIGH);
  WRITE(SLCD_DAT_PIN, HIGH);
  WRITE(SLCD_CS_PIN, HIGH);
  WRITE(SLCD_DC_PIN, HIGH);

  curr_x = curr_y = 0;
  curr_color = CI_FOREGROUND;
    
  curr_font_offsets = font_04B_03_offs;
  curr_font_widths = font_04B_03_widths;
  curr_font_bitmaps = font_04B_03_bitmaps;
}

void NOKIA5110GA::begin() {

  // init display
  WRITE(SLCD_CS_PIN, LOW);
  for (uint8_t c=0; c < NOKIA5110_init_len; c++)
      send(pgm_read_byte(&NOKIA5110_init[c]), false);
  WRITE(SLCD_CS_PIN, HIGH);

  clearScreen();
}

void NOKIA5110GA::clearScreen() {
  WRITE(SLCD_CS_PIN, LOW);
  curr_x = 0;
  curr_y = 0;
  send(0x80 | curr_x, false);  // Column
  send(0x40 | curr_y, false);  // Row
	for(int i = 0; i < getWidth(); i+= 1) {
    for(int j = 0; j < getHeight(); j+=8) {
      send(0);
    }
  }
  WRITE(SLCD_CS_PIN, HIGH);
}

inline void NOKIA5110GA::send(uint8_t d, bool is_data) {
  if (is_data) {
    if (curr_color == CI_BACKGROUND)
      d ^= 0b11111111; // flip d
    WRITE(SLCD_DC_PIN, HIGH);
  }
  else
    WRITE(SLCD_DC_PIN, LOW);

  for (uint8_t bit = 0x80; bit; bit >>= 1) {
    WRITE(SLCD_CLK_PIN, LOW);
    if (d & bit) 
      WRITE(SLCD_DAT_PIN, HIGH);
    else
      WRITE(SLCD_DAT_PIN, LOW);
     WRITE(SLCD_CLK_PIN, HIGH);
  }
}

void NOKIA5110GA::setColor(const COLOR_INDEX idx) {
  curr_color = idx;
}

inline int8_t NOKIA5110GA::get_char_width(const char c) {
  if (c >= 128 || c < 32)
    return 0;
  else if (c == ' ') // is a space
    return 2;
  else
    return pgm_read_byte(&curr_font_widths[c-32]);
}

void NOKIA5110GA::draw_char(const char c, bool sep) {
  
  uint8_t w = get_char_width(c);
  if (w == 0 || (curr_x + w) > getWidth())
    return;

  if (c == ' ') {
    curr_x += w + sep;
    send(0); send(0);
    if (sep) send(0);
    return;
  }
  else {
    uint16_t offset = pgm_read_word(&curr_font_offsets[c-32]);
    while(w--) {
      uint8_t bits = pgm_read_byte(&curr_font_bitmaps[offset + w]);
      send(bits);
      curr_x++;
    }
  }

  if (sep) {
    send(0);
    curr_x++;
  }
}

uint8_t NOKIA5110GA::printP(const char *pstr) {
  WRITE(SLCD_CS_PIN, LOW);
  uint8_t oldx = curr_x;
  send(0x80 | curr_x, false);  // Column
  send(0x40 | curr_y, false);  // Row
  char c = pgm_read_byte(pstr++);
  while(c) {
    char cn = pgm_read_byte(pstr++);
    draw_char(c, cn != 0);
    c = cn;
  }  
  WRITE(SLCD_CS_PIN, HIGH);
  return curr_x - oldx;
}
    
uint8_t NOKIA5110GA::print(const char *str, const bool space) {
  WRITE(SLCD_CS_PIN, LOW);
  uint8_t oldx = curr_x;
  send(0x80 | curr_x, false);  // Column
  send(0x40 | curr_y, false);  // Row
  while(str[0]) {
    draw_char(str[0], str[1] != 0 || space);
    str++;
  }
  WRITE(SLCD_CS_PIN, HIGH);
  return curr_x - oldx;
}

uint8_t NOKIA5110GA::drawString(const uint8_t x, const uint8_t y, const char *str) {
  setPrintPos(x, y);
  return print(str);
}

uint8_t NOKIA5110GA::stringWidth(const char *str) {
  int w = 0;
  while(str[0]) {
    w += 1 + get_char_width(str[0]);
    str++;
  }
  return w - 1;
}

uint8_t NOKIA5110GA::stringWidthP(const char *pstr) {
  int w = 0;
  char c = pgm_read_byte(pstr++);
  while(c) {
    w += 1 + get_char_width(c);
    c = pgm_read_byte(pstr++);
  }
  return w - 1;
}

void NOKIA5110GA::drawPBar(const uint8_t w, const uint8_t p) {
  WRITE(SLCD_CS_PIN, LOW);
  send(0x80 | curr_x, false);  // Column
  send(0x40 | curr_y, false);  // Row

  uint8_t filled = ((float)p/100.0*(w-2));
  for(uint8_t i = 0; i < w; i++) {
    if (i == 0 || i == w-1 || i <= filled)
      send(0b01111111);
    else
      send(0b01000001);
  }
  WRITE(SLCD_CS_PIN, HIGH);
}

void NOKIA5110GA::drawBitmapP(uint8_t off_x, uint8_t off_y, const uint8_t *bmp, const uint8_t byte_width, const uint8_t bmp_bytes) {
  WRITE(SLCD_CS_PIN, LOW);
  send(0x80 | off_x, false);  // Column
  send(0x40 | off_y, false);  // Row
  send(0x22, false); // vertical mode

  uint8_t b = 0;
  do {
    for(int j = byte_width-1; j >= 0; j--) {
      uint8_t w = pgm_read_byte(bmp+b+j);
      send(w);
    }
    b += byte_width;
    off_x++;
    send(0x40 | off_y, false);
    send(0x80 | off_x, false);  // Column
  } while (b < bmp_bytes / sizeof(uint8_t));
  
  send(0x20, false); // horizontal mode
  WRITE(SLCD_CS_PIN, HIGH);
}

void NOKIA5110GA::setFont(const uint16_t *font_offsets, const uint8_t *font_widths, const uint8_t *font_bitmaps) {
  curr_font_offsets = font_offsets;
  curr_font_widths = font_widths;
  curr_font_bitmaps = font_bitmaps;
}

void NOKIA5110GA::clearRect(const uint8_t x, const uint8_t y, uint8_t w) {
  WRITE(SLCD_CS_PIN, LOW);
  send(0x80 | x, false);  // Column
  send(0x40 | y, false);  // Row
  if ((y + w) > getWidth())
    w = getWidth() - y;
  while(w--)
    send(0);
  WRITE(SLCD_CS_PIN, HIGH);
}

void NOKIA5110GA::setContrast(const int16_t c) {
  WRITE(SLCD_CS_PIN, LOW);
  send(0x21, false);
  send(0x80 | c, false);
  send(0x20, false);
  WRITE(SLCD_CS_PIN, HIGH);
}
#endif