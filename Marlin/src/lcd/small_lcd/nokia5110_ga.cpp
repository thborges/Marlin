/*
 * A Driver to NOKIA5110 Display
 * 
 * Author: Thiago Borges de Oliveira (thborges@gmail.com)
 * 
 * CC BY 4.0, Licensed under a Creative Commons Attribution 4.0 International license: 
 * http://creativecommons.org/licenses/by/4.0/
 */

#include "nokia5110_ga.h"

#if ENABLED(NOKIA5110_LCD)

NOKIA5110GA::NOKIA5110GA(const uint8_t CLK, const uint8_t DAT, const uint8_t CS, const uint8_t DC) {
    
    CLK_PIN = CLK;
    DATA_PIN = DAT;
    data_port_clk = portOutputRegister(digitalPinToPort(CLK_PIN));
    data_mask_clk = digitalPinToBitMask(CLK_PIN);
    data_port_dat = portOutputRegister(digitalPinToPort(DATA_PIN));
    data_mask_dat = digitalPinToBitMask(DATA_PIN);

    CS_PIN = CS;
    DC_PIN = DC;
    data_port_cs = portOutputRegister(digitalPinToPort(CS_PIN));
    data_mask_cs = digitalPinToBitMask(CS_PIN);
    data_port_dc = portOutputRegister(digitalPinToPort(DC_PIN));
    data_mask_dc = digitalPinToBitMask(DC_PIN);

    curr_x = curr_y = 0;
    curr_color = CI_FOREGROUND;
    
    curr_font_offsets = font_04B_03_offs;
    curr_font_widths = font_04B_03_widths;
    curr_font_bitmaps = font_04B_03_bitmaps;
}

void NOKIA5110GA::begin() {

  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  digitalWrite(CLK_PIN, HIGH);
  digitalWrite(DATA_PIN, HIGH);

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  pinMode(DC_PIN, OUTPUT);
  digitalWrite(DC_PIN, HIGH);
  delay(1);

  // init display
  *data_port_cs &= ~data_mask_cs;            // CS low
  for (uint8_t c=0; c < NOKIA5110_init_len; c++)
      send(pgm_read_byte(&NOKIA5110_init[c]), false);
  *data_port_cs |= data_mask_cs;            // CS high

  clearScreen();
}

void NOKIA5110GA::clearScreen() {
  *data_port_cs &= ~data_mask_cs;            // CS low
  curr_x = 0;
  curr_y = 0;
  send(0x80 | curr_x, false);  // Column
  send(0x40 | curr_y, false);  // Row
	for(int i = 0; i < getWidth(); i+= 1) {
    for(int j = 0; j < getHeight(); j+=8) {
      send(0);
    }
  }
  *data_port_cs |= data_mask_cs;            // CS high
}

inline void NOKIA5110GA::send(uint8_t d, bool is_data) {
  if (is_data) {
    if (curr_color == CI_BACKGROUND)
      d ^= 0b11111111; // flip d
    *data_port_dc |= data_mask_dc; // data HIGH
  }
  else
    *data_port_dc &= ~data_mask_dc; // command LOW

  for (uint8_t bit = 0x80; bit; bit >>= 1) {
    *data_port_clk &= ~data_mask_clk; // clk low
    if (d & bit) 
      *data_port_dat |= data_mask_dat;
    else
      *data_port_dat &= ~data_mask_dat;
    *data_port_clk |= data_mask_clk; // clk high
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
  *data_port_cs &= ~data_mask_cs;           // CS low
  uint8_t oldx = curr_x;
  send(0x80 | curr_x, false);  // Column
  send(0x40 | curr_y, false);  // Row
  char c = pgm_read_byte(pstr++);
  while(c) {
    char cn = pgm_read_byte(pstr++);
    draw_char(c, cn != 0);
    c = cn;
  }  
  *data_port_cs |= data_mask_cs;            // CS high
  return curr_x - oldx;
}
    
uint8_t NOKIA5110GA::print(const char *str) {
  *data_port_cs &= ~data_mask_cs;           // CS low
  uint8_t oldx = curr_x;
  send(0x80 | curr_x, false);  // Column
  send(0x40 | curr_y, false);  // Row
  while(str[0]) {
    draw_char(str[0], str[1] != 0);
    str++;
  }
  *data_port_cs |= data_mask_cs;            // CS high
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
  *data_port_cs &= ~data_mask_cs;           // CS low
  send(0x80 | curr_x, false);  // Column
  send(0x40 | curr_y, false);  // Row

  uint8_t filled = ((float)p/100.0*(w-2));
  for(uint8_t i = 0; i < w; i++) {
    if (i == 0 || i == w-1 || i <= filled)
      send(0b01111111);
    else
      send(0b01000001);
  }
  *data_port_cs |= data_mask_cs;            // CS high
}

void NOKIA5110GA::drawBitmapP(uint8_t off_x, uint8_t off_y, const uint8_t *bmp, const uint8_t byte_width, const uint8_t bmp_bytes) {
  *data_port_cs &= ~data_mask_cs;           // CS low
  send(0x80 | off_x, false);  // Column
  send(0x40 | off_y, false);  // Row
  send(0x22, false); // vertical mode

  uint8_t b = 0;
  do {
    for(int j = byte_width-1; j >= 0; j--) {
      uint8_t w = pgm_read_byte(bmp+b+j);
      if (curr_color == CI_BACKGROUND)
        w ^= 0b11111111; // flip w
      send(w);
    }
    b += byte_width;
    off_x++;
    send(0x40 | off_y, false);
    send(0x80 | off_x, false);  // Column
  } while (b < bmp_bytes / sizeof(uint8_t));
  
  send(0x20, false); // horizontal mode
  *data_port_cs |= data_mask_cs;            // CS high
}

void NOKIA5110GA::setFont(const uint16_t *font_offsets, const uint8_t *font_widths, const uint8_t *font_bitmaps) {
  curr_font_offsets = font_offsets;
  curr_font_widths = font_widths;
  curr_font_bitmaps = font_bitmaps;
}

void NOKIA5110GA::clearRect(const uint8_t x, const uint8_t y, uint8_t w) {
  *data_port_cs &= ~data_mask_cs;           // CS low
  send(0x80 | x, false);  // Column
  send(0x40 | y, false);  // Row
  if ((y + w) > getWidth())
    w = getWidth() - y;
  while(w--)
    send(0);
  *data_port_cs |= data_mask_cs;            // CS high
}

#endif