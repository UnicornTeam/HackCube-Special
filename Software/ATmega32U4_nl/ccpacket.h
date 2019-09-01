#ifndef _CCPACKET_H
#define _CCPACKET_H

#include "Arduino.h"

/**
 * Buffer and data lengths
 */
#define CC1101_BUFFER_LEN        64
#define CC1101_DATA_LEN          CC1101_BUFFER_LEN - 3

/**
 * Class: CCPACKET
 * 
 * Description:
 * CC1101 data packet class
 */
class CCPACKET
{
  public:
    /**
     * Data length
     */
    byte length;

    /**
     * Data buffer
     */
    byte data[CC1101_DATA_LEN];

    /**
     * CRC OK flag
     */
    boolean crc_ok;

    /**
     * Received Strength Signal Indication
     */
    byte rssi;

    /**
     * Link Quality Index
     */
    byte lqi;
};

#endif
