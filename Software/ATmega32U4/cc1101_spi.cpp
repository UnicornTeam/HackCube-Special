/**
 * Copyright (c) 2011 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 03/03/2011
 */

#include "cc1101_spi.h"

/**
 * init
 * 
 * SPI initialization
 */

void SPI::init() 
{

  pinMode(SPI_MOSI, OUTPUT);
  pinMode(SPI_MISO, INPUT);
  pinMode(SPI_SCK, OUTPUT);
  digitalWrite(SPI_SCK, HIGH);
  digitalWrite(SPI_MOSI, LOW);

  // SPI speed = clk/4
  SPCR = _BV(SPE) | _BV(MSTR);
}

/**
 * send
 * 
 * Send byte via SPI
 * 
 * 'value'	Value to be sent
 * 
 * Return:
 * 	Response received from SPI slave
 */
byte SPI::send(byte value) 
{
  SPDR = value;                          // Transfer byte via SPI
  wait_Spi();                            // Wait until SPI operation is terminated

  return SPDR;
}
