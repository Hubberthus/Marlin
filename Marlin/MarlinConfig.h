/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef MARLIN_CONFIG_H
#define MARLIN_CONFIG_H

#ifdef ESP8266

#include "EEPROM.h"

inline void eeprom_write_byte(unsigned char* pos, uint8_t value) {
	EEPROM.write((int)pos, value);
	EEPROM.commit();
}

inline uint8_t eeprom_read_byte(unsigned char* pos) {
	return EEPROM.read((int)pos);
}

inline void eeprom_read_block(void *data, const void *address, size_t len){
  int i;
  uint8_t *tmp = (uint8_t*)data;

  for(i = 0; i < len; i++){
	tmp[i] = EEPROM.read((int)address + i);
  }
}

inline void eeprom_update_block(const void *data, void *address, size_t len) {
  int i;
  uint8_t *tmp = (uint8_t*)data;

  for(i = 0; i < len; i++){
    EEPROM.write((int)address + i, tmp[i]);
  }
  EEPROM.commit();
}

#endif

#include "fastio.h"
#include "macros.h"
#include "boards.h"
#include "Version.h"
#include "Configuration.h"
#include "Conditionals_LCD.h"
#include "Configuration_adv.h"
#include "pins.h"
#ifndef USBCON
  #define HardwareSerial_h // trick to disable the standard HWserial
#endif
#include "Arduino.h"
#include "Conditionals_post.h"
#include "SanityCheck.h"

#endif // MARLIN_CONFIG_H
