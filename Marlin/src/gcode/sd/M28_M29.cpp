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

#include "../../inc/MarlinConfig.h"
#include "../../module/temperature.h"

#if ENABLED(SDSUPPORT)

#include "../gcode.h"
#include "../../sd/cardreader.h"

#define BCT_COMPRESSION

#if ENABLED(BCT_COMPRESSION)
  #include "../../libs/heatshrink/heatshrink_decoder.h"
#endif

#if NUM_SERIAL > 1
  #include "../queue.h"
#endif

#define BCT_WAIT_BYTE_TIMEOUT 5000
#define BCT_MAX_RAW_SIZE 512
#define BCT_EOT 4
#define BCT_ACK 6
#define BCT_NCK 21
#define BCT_CAN 24

inline uint16_t serial_read_bytes(uint8_t *buff, uint16_t count, uint8_t *chksum) {

  (*chksum) = 0;
  for(uint16_t i = 0; i < count; i++) {
    int c;
    millis_t start = millis();
    while ((c = MYSERIAL0.read()) < 0) {// wait for char
      thermalManager.manage_heater();
      if ((millis() - start) > BCT_WAIT_BYTE_TIMEOUT) {
        // timeout, finalize transmission
        SERIAL_CHAR(BCT_EOT);
        SERIAL_ECHOLN("Timeout waiting for binary data.");
        return i;
      }
    }
    buff[i] = c;
    (*chksum) ^= c;
  }
  return count;
}

int16_t poll_output(heatshrink_decoder *hsd, uint8_t *output_buffer, size_t *polled) {
  HSD_poll_res pres;
  do {
    size_t consumed;
    pres = heatshrink_decoder_poll(hsd, &output_buffer[*polled], 
      BCT_MAX_RAW_SIZE - *polled, &consumed);
    *polled += consumed;
    if (*polled == BCT_MAX_RAW_SIZE) {
      int16_t ret = card.write(output_buffer, BCT_MAX_RAW_SIZE);
      if (ret < 0)
        return ret;
      *polled = 0;
    }
  } while (pres == HSDR_POLL_MORE);
  return 0;
}

inline void receive_raw_file() {

  struct [[gnu::packed]] block_header {
    union {
      struct {
        uint16_t size;
        uint8_t last_block;
        uint8_t chksum;
      };
      char csize[2];
    };
  } bh;
  uint8_t input_buffer[BCT_MAX_RAW_SIZE];

  // decoder setup
  heatshrink_decoder hsd;
  uint8_t output_buffer[BCT_MAX_RAW_SIZE];
  heatshrink_decoder_reset(&hsd);

  size_t polled = 0;
  do {
    uint8_t chksum;
    uint16_t bread = serial_read_bytes((uint8_t*)&bh, sizeof(bh), &chksum);
    if (bread != sizeof(bh))
      break;

    bread = serial_read_bytes(input_buffer, bh.size, &chksum);
    if (bread != bh.size)
      break;
    
    if (chksum != bh.chksum) { // checksum differ, request again
      SERIAL_CHAR(BCT_NCK);
      continue;
    }

    // decompress bytes received
    size_t sunk = 0, consumed;
    while (sunk < bread) {
      heatshrink_decoder_sink(&hsd, &input_buffer[sunk], BCT_MAX_RAW_SIZE - sunk, &consumed);
      sunk += consumed;
      if (poll_output(&hsd, output_buffer, &polled) < 0) {
        SERIAL_CHAR(BCT_CAN);
        return;
      }
    }
    // block processed successfully
    SERIAL_CHAR(BCT_ACK);

    if (bh.last_block) {
      HSD_finish_res fres = heatshrink_decoder_finish(&hsd);
      if (fres == HSDR_FINISH_MORE) {
        if (poll_output(&hsd, output_buffer, &polled) < 0)
          break;
      }
      if (polled > 0) { // write remaining bytes in output_buffer, if any
        if (card.write(output_buffer, polled) < 0)
          break;
      }

      // signal end of transmission
      SERIAL_CHAR(BCT_EOT);
      return;
    }
  } while (1); // receiving file content

  // on error, report cancel transmission
  SERIAL_CHAR(BCT_CAN);
}

/**
 * M28: Start SD Write
 */
void GcodeSuite::M28() {

  #if ENABLED(BINARY_FILE_TRANSFER)

    bool binary_mode = false;
    char *p = parser.string_arg;
    if (p[0] == 'B' && NUMERIC(p[1])) {
      binary_mode = p[1] > '0';
      p += 2;
      while (*p == ' ') ++p;
    }

    // Binary transfer mode
    if ((card.flag.binary_mode = binary_mode)) {
      SERIAL_ECHO_MSG("Switching to Binary Protocol");
      #if NUM_SERIAL > 1
        card.transfer_port_index = queue.port[queue.index_r];
      #endif
    }
    else
      card.openFileWrite(p);

  #else

    // raw data protocol, enabled by M28 !file_name
    bool saving_raw = false;
    char *file = parser.string_arg;
    if (file[0] == '!') {
      saving_raw = true;
      file++;
    }

    card.openFileWrite(file);

    if (card.flag.saving && saving_raw) {
      receive_raw_file();
      card.closefile();
    }
  #endif
}

/**
 * M29: Stop SD Write
 * (Processed in write-to-file routine)
 */
void GcodeSuite::M29() {
  card.flag.saving = false;
}

#endif // SDSUPPORT
