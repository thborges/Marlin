/*
 * A Driver to SSD1331 Color OLED Display
 * Uses the device buit-in Graphics Acceleration feature
 * 
 * Author: Thiago Borges de Oliveira (thborges@gmail.com)
 * 
 * Based on Colour Graphics Library developed by David Johnson-Davies
 * http://www.technoblogy.com/show?2EA7
 * 
 * CC BY 4.0, Licensed under a Creative Commons Attribution 4.0 International license: 
 * http://creativecommons.org/licenses/by/4.0/
 */

#include "ssd1331_ga.h"

#define sendX(x) send((x) > 95 ? 95 : x)
#define sendY(y) send((y) > 63 ? 63 : y)
#define SEND_COLOR(i) send(curr_color[i].red & 0b00111110); send(curr_color[i].green & 0b00111111); send(curr_color[i].blue & 0b00111110)
//#define SEND_COLOR(i) send(curr_color[i].red); send(curr_color[i].green); send(curr_color[i].blue)

#define HARDWARE_SPI

#if !ENABLED(HARDWARE_SPI)
SSD1331GA::SSD1331GA(const uint8_t CLK, const uint8_t DAT, const uint8_t CS, const uint8_t RST) {
    
    CLK_PIN = CLK;
    DATA_PIN = DAT;
    data_port_clk = portOutputRegister(digitalPinToPort(CLK_PIN));
    data_mask_clk = digitalPinToBitMask(CLK_PIN);
    data_port_dat = portOutputRegister(digitalPinToPort(DATA_PIN));
    data_mask_dat = digitalPinToBitMask(DATA_PIN);

#else
SSD1331GA::SSD1331GA(const uint8_t CS, const uint8_t RST) {
#endif

    CS_PIN = CS;
    RST_PIN = RST;
    data_port_cs = portOutputRegister(digitalPinToPort(CS_PIN));
    data_mask_cs = digitalPinToBitMask(CS_PIN);
    data_port_rst = portOutputRegister(digitalPinToPort(RST_PIN));
    data_mask_rst = digitalPinToBitMask(RST_PIN);

    curr_x = curr_y = 0;
    
    curr_font_offsets = font_04B_03_offs;
    curr_font_widths = font_04B_03_widths;
    curr_font_bitmaps = font_04B_03_bitmaps;
    
    curr_color[CI_BACKGROUND] = {0,0,0};
    curr_color[CI_FOREGROUND] = {255,255,255};
}

void SSD1331GA::begin() {

#if DISABLED(HARDWARE_SPI)
  pinMode(CLK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  digitalWrite(CLK_PIN, HIGH);
  digitalWrite(DATA_PIN, HIGH);
#endif

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  pinMode(RST_PIN, OUTPUT);
  digitalWrite(RST_PIN, HIGH);
  digitalWrite(RST_PIN, LOW);
  digitalWrite(RST_PIN, HIGH);
  delay(1);

#if ENABLED(HARDWARE_SPI)
  spiBegin();
#endif

  // init display
  *data_port_cs &= ~data_mask_cs;            // CS low
  for (uint8_t c=0; c < SSD1331_init_len; c++)
      send(pgm_read_byte(&SSD1331_init[c]));
  
  // clear display
  send(0x25);                               // Clear Window
  send(0); send(0); send(95); send(63);
  delay(10);

  *data_port_cs |= data_mask_cs;            // CS high
}

inline void SSD1331GA::send(const uint8_t d) {
  #if ENABLED(HARDWARE_SPI)
    spiSend(d);
  #else
    for (uint8_t bit = 0x80; bit; bit >>= 1) {
      *data_port_clk &= ~data_mask_clk; // clk low
      if (d & bit) 
        *data_port_dat |= data_mask_dat;
      else
        *data_port_dat &= ~data_mask_dat;
      *data_port_clk |= data_mask_clk; // clk high
    }
  #endif
}

void SSD1331GA::setColor(const COLOR_INDEX idx, const uint8_t red, const uint8_t green, const uint8_t blue) {
    curr_color[idx].red = red>>2;
    curr_color[idx].green = green>>2;
    curr_color[idx].blue = blue>>2;
}

inline int8_t SSD1331GA::get_char_width(const char c) {
  if (c >= 128 || c < 32)
    return 0;
  else if (c == ' ') // is a space
    return 2;
  else
    return pgm_read_byte(&curr_font_widths[c-32]);
}

void SSD1331GA::draw_char(const char c, bool sep) {
  
  uint8_t w = get_char_width(c);
  if (w == 0 || (curr_x + w + sep) >= 96)
    return;

  send(0x25); // clear char box
  sendX(curr_x); sendY(curr_y); 
  if (sep) sendX(curr_x+w);
  else sendX(curr_x+w-1);
  sendY(curr_y+7);

  if (c == ' ') {
    curr_x += w + sep;
    return;
  }

  uint16_t offset = pgm_read_word(&curr_font_offsets[c-32]);
  while(w--) {
    uint8_t bits = pgm_read_byte(&curr_font_bitmaps[offset + w]);
    uint8_t r = 0;
    while (bits) {
      // count zeros
      while ((bits & 1) == 0) {r++; bits = bits >> 1; }
      uint8_t on = r;
      // count ones
      while ((bits & 1) != 0) {r++; bits = bits >> 1; }
      uint8_t off = r-1;
      // draw a line
      send(0x21);
      sendX(curr_x); sendY(curr_y+on); sendX(curr_x); sendY(curr_y+off);
      SEND_COLOR(CI_FOREGROUND);
    }
    curr_x++;
  }
  if (sep)
    curr_x++;
}

uint8_t SSD1331GA::printP(const char *pstr) {
  *data_port_cs &= ~data_mask_cs;           // CS low
  uint8_t oldx = curr_x;
  char c = pgm_read_byte(pstr++);
  while(c) {
    char cn = pgm_read_byte(pstr++);
    draw_char(c, cn != 0);
    c = cn;
  }  
  *data_port_cs |= data_mask_cs;            // CS high
  return curr_x - oldx;
}
    
uint8_t SSD1331GA::print(const char *str) {
  *data_port_cs &= ~data_mask_cs;           // CS low
  uint8_t oldx = curr_x;
  while(str[0]) {
    draw_char(str[0], str[1] != 0);
    str++;
  }
  *data_port_cs |= data_mask_cs;            // CS high
  return curr_x - oldx;
}

uint8_t SSD1331GA::drawString(const uint8_t x, const uint8_t y, const char *str) {
  setPrintPos(x, y);
  return print(str);
}

uint8_t SSD1331GA::stringWidth(const char *str) {
  int w = 0;
  while(str[0]) {
    w += 1 + get_char_width(str[0]);
    str++;
  }
  return w - 1;
}

uint8_t SSD1331GA::stringWidthP(const char *pstr) {
  int w = 0;
  char c = pgm_read_byte(pstr++);
  while(c) {
    w += 1 + get_char_width(c);
    c = pgm_read_byte(pstr++);
  }
  return w - 1;
}

void SSD1331GA::drawRect(const uint8_t x, const uint8_t y, const uint8_t w, const uint8_t h, const bool filled) {
  *data_port_cs &= ~data_mask_cs;         // CS low
  send(0x26); // Enable or disable fill
  send(filled ? 0x1 : 0x0);   
  send(0x22); // Draw rectangle
  sendX(x); sendY(y); sendX(x+w); sendY(y+h);
  SEND_COLOR(CI_FOREGROUND);
  SEND_COLOR(CI_BACKGROUND);
  *data_port_cs |= data_mask_cs;            // CS high
}

void SSD1331GA::drawLine(const uint8_t x0, const uint8_t y0, const uint8_t x1, const uint8_t y1, const COLOR_INDEX idx) {
  *data_port_cs &= ~data_mask_cs;           // CS low
  send(0x21);                             // Draw Line
  sendX(x0); sendY(y0); sendX(x1); sendY(y1);
  SEND_COLOR(idx);
  *data_port_cs |= data_mask_cs;            // CS high
}

void SSD1331GA::setFont(const uint16_t *font_offsets, const uint8_t *font_widths, const uint8_t *font_bitmaps) {
  curr_font_offsets = font_offsets;
  curr_font_widths = font_widths;
  curr_font_bitmaps = font_bitmaps;
}

void SSD1331GA::clearRect(const uint8_t x, const uint8_t y, const uint8_t w, const uint8_t h) {
  *data_port_cs &= ~data_mask_cs;           // CS low
  send(0x25);                               // Clear Window
  sendX(x); sendY(y); sendX(x+w); sendY(y+h);
  *data_port_cs |= data_mask_cs;            // CS high
}
