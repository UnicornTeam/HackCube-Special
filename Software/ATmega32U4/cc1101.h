#ifndef _CC1101_H
#define _CC1101_H

#include "Arduino.h"
#include "cc1101_spi.h"
#include "ccpacket.h"


enum CFREQ {
    CFREQ_868 = 0,
    CFREQ_915,
    CFREQ_433,
    CFREQ_315,
    CFREQ_870,
    CFREQ_LAST
};

/**
 *  frequencies
 */

/**
 * Frequency channels
 */
#define NUMBER_OF_FCHANNELS      10

/**
 * Type of transfers
 */
#define WRITE_BURST              0x40
#define READ_SINGLE              0x80
#define READ_BURST               0xC0

/**
 * Type of register
 */
#define CC1101_CONFIG_REGISTER   READ_SINGLE
#define CC1101_STATUS_REGISTER   READ_BURST

/**
 * PATABLE & FIFO's
 */

#define CC1101_PATABLE           0x3E        // PATABLE address
#define CC1101_TXFIFO            0x3F        // TX FIFO address
#define CC1101_RXFIFO            0x3F        // RX FIFO address

/**
 * Command strobes
 */
#define CC1101_SRES              0x30        // Reset CC1101 chip
#define CC1101_SFSTXON           0x31        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1). If in RX (with CCA):
// Go to a wait state where only the synthesizer is running (for quick RX / TX turnaround).
#define CC1101_SXOFF             0x32        // Turn off crystal oscillator
#define CC1101_SCAL              0x33        // Calibrate frequency synthesizer and turn it off. SCAL can be strobed from IDLE mode without
// setting manual calibration mode (MCSM0.FS_AUTOCAL=0)
#define CC1101_SRX               0x34        // Enable RX. Perform calibration first if coming from IDLE and MCSM0.FS_AUTOCAL=1
#define CC1101_STX               0x35        // In IDLE state: Enable TX. Perform calibration first if MCSM0.FS_AUTOCAL=1.
// If in RX state and CCA is enabled: Only go to TX if channel is clear
#define CC1101_SIDLE             0x36        // Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable
#define CC1101_SWOR              0x38        // Start automatic RX polling sequence (Wake-on-Radio) as described in Section 19.5 if
// WORCTRL.RC_PD=0
#define CC1101_SPWD              0x39        // Enter power down mode when CSn goes high
#define CC1101_SFRX              0x3A        // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
#define CC1101_SFTX              0x3B        // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
#define CC1101_SWORRST           0x3C        // Reset real time clock to Event1 value
#define CC1101_SNOP              0x3D        // No operation. May be used to get access to the chip status byte

/**
 * CC1101 configuration registers
 */
#define CC1101_IOCFG2            0x00        // GDO2 Output Pin Configuration
#define CC1101_IOCFG1            0x01        // GDO1 Output Pin Configuration
#define CC1101_IOCFG0            0x02        // GDO0 Output Pin Configuration
#define CC1101_FIFOTHR           0x03        // RX FIFO and TX FIFO Thresholds
#define CC1101_SYNC1             0x04        // Sync Word, High Byte
#define CC1101_SYNC0             0x05        // Sync Word, Low Byte
#define CC1101_PKTLEN            0x06        // Packet Length
#define CC1101_PKTCTRL1          0x07        // Packet Automation Control
#define CC1101_PKTCTRL0          0x08        // Packet Automation Control
#define CC1101_ADDR              0x09        // Device Address
#define CC1101_CHANNR            0x0A        // Channel Number
#define CC1101_FSCTRL1           0x0B        // Frequency Synthesizer Control
#define CC1101_FSCTRL0           0x0C        // Frequency Synthesizer Control
#define CC1101_FREQ2             0x0D        // Frequency Control Word, High Byte
#define CC1101_FREQ1             0x0E        // Frequency Control Word, Middle Byte
#define CC1101_FREQ0             0x0F        // Frequency Control Word, Low Byte
#define CC1101_MDMCFG4           0x10        // Modem Configuration
#define CC1101_MDMCFG3           0x11        // Modem Configuration
#define CC1101_MDMCFG2           0x12        // Modem Configuration
#define CC1101_MDMCFG1           0x13        // Modem Configuration
#define CC1101_MDMCFG0           0x14        // Modem Configuration
#define CC1101_DEVIATN           0x15        // Modem Deviation Setting
#define CC1101_MCSM2             0x16        // Main Radio Control State Machine Configuration
#define CC1101_MCSM1             0x17        // Main Radio Control State Machine Configuration
#define CC1101_MCSM0             0x18        // Main Radio Control State Machine Configuration
#define CC1101_FOCCFG            0x19        // Frequency Offset Compensation Configuration
#define CC1101_BSCFG             0x1A        // Bit Synchronization Configuration
#define CC1101_AGCCTRL2          0x1B        // AGC Control
#define CC1101_AGCCTRL1          0x1C        // AGC Control
#define CC1101_AGCCTRL0          0x1D        // AGC Control
#define CC1101_WOREVT1           0x1E        // High Byte Event0 Timeout
#define CC1101_WOREVT0           0x1F        // Low Byte Event0 Timeout
#define CC1101_WORCTRL           0x20        // Wake On Radio Control
#define CC1101_FREND1            0x21        // Front End RX Configuration
#define CC1101_FREND0            0x22        // Front End TX Configuration
#define CC1101_FSCAL3            0x23        // Frequency Synthesizer Calibration
#define CC1101_FSCAL2            0x24        // Frequency Synthesizer Calibration
#define CC1101_FSCAL1            0x25        // Frequency Synthesizer Calibration
#define CC1101_FSCAL0            0x26        // Frequency Synthesizer Calibration
#define CC1101_RCCTRL1           0x27        // RC Oscillator Configuration
#define CC1101_RCCTRL0           0x28        // RC Oscillator Configuration
#define CC1101_FSTEST            0x29        // Frequency Synthesizer Calibration Control
#define CC1101_PTEST             0x2A        // Production Test
#define CC1101_AGCTEST           0x2B        // AGC Test
#define CC1101_TEST2             0x2C        // Various Test Settings
#define CC1101_TEST1             0x2D        // Various Test Settings
#define CC1101_TEST0             0x2E        // Various Test Settings

/*
 * Status registers
 */
#define CC1101_PARTNUM           0x30        // Chip ID
#define CC1101_VERSION           0x31        // Chip ID
#define CC1101_FREQEST           0x32        // Frequency Offset Estimate from Demodulator
#define CC1101_LQI               0x33        // Demodulator Estimate for Link Quality
#define CC1101_RSSI              0x34        // Received Signal Strength Indication
#define CC1101_MARCSTATE         0x35        // Main Radio Control State Machine State
#define CC1101_WORTIME1          0x36        // High Byte of WOR Time
#define CC1101_WORTIME0          0x37        // Low Byte of WOR Time
#define CC1101_PKTSTATUS         0x38        // Current GDOx Status and Packet Status
#define CC1101_VCO_VC_DAC        0x39        // Current Setting from PLL Calibration Module
#define CC1101_TXBYTES           0x3A        // Underflow and Number of Bytes
#define CC1101_RXBYTES           0x3B        // Overflow and Number of Bytes
#define CC1101_RCCTRL1_STATUS    0x3C        // Last RC Oscillator Calibration Result
#define CC1101_RCCTRL0_STATUS    0x3D        // Last RC Oscillator Calibration Result 

/**
 * Macros
 */
// Read CC1101 Config register
#define readConfigReg(regAddr)    readReg(regAddr, CC1101_CONFIG_REGISTER)
// Read CC1101 Status register
#define readStatusReg(regAddr)    readReg(regAddr, CC1101_STATUS_REGISTER)
// Enter Rx state
#define setRxState()              cmdStrobe(CC1101_SRX)
// Enter Tx state
#define setTxState()              cmdStrobe(CC1101_STX)
// Enter IDLE state
#define setIdleState()            cmdStrobe(CC1101_SIDLE)
// Flush Rx FIFO
#define flushRxFifo()             cmdStrobe(CC1101_SFRX)
// Flush Tx FIFO
#define flushTxFifo()             cmdStrobe(CC1101_SFTX)
// Disable address check
#define disableAddressCheck()     writeReg(CC1101_PKTCTRL1, 0x04)
// Enable address check
#define enableAddressCheck()      writeReg(CC1101_PKTCTRL1, 0x06)
// Disable CCA
#define disableCCA()              writeReg(CC1101_MCSM1, 0)
// Enable CCA
#define enableCCA()               writeReg(CC1101_MCSM1, CC1101_DEFVAL_MCSM1)
// Set PATABLE single byte
#define setTxPowerAmp(setting)    paTableByte = setting
// PATABLE values
//#define PA_LowPower               0x60
#define PA_LongDistance           0xC2


#define CC1101_DEFVAL_IOCFG2           0x29
#define CC1101_DEFVAL_IOCFG1           0x2E
#define CC1101_DEFVAL_IOCFG0           0x06
#define CC1101_DEFVAL_FIFOTHR          0x47
#define CC1101_DEFVAL_SYNC1            0xD3
#define CC1101_DEFVAL_SYNC0            0x91
#define CC1101_DEFVAL_PKTLEN           0x05
#define CC1101_DEFVAL_PKTCTRL1         0x04
#define CC1101_DEFVAL_PKTCTRL0         0x30  //Òì²½Ä£Ê½
#define CC1101_DEFVAL_ADDR             0x00
#define CC1101_DEFVAL_CHANNR           0x00
#define CC1101_DEFVAL_FSCTRL1          0x06
#define CC1101_DEFVAL_FSCTRL0          0x00
// Carrier frequency = 868 MHz
#define CC1101_DEFVAL_FREQ2_868  0x21        // Frequency Control Word, High Byte
#define CC1101_DEFVAL_FREQ1_868  0x62        // Frequency Control Word, Middle Byte
#define CC1101_DEFVAL_FREQ0_868  0x76        // Frequency Control Word, Low Byte
// Carrier frequency = 902 MHz
#define CC1101_DEFVAL_FREQ2_915  0x22        // Frequency Control Word, High Byte
#define CC1101_DEFVAL_FREQ1_915  0xB1        // Frequency Control Word, Middle Byte
#define CC1101_DEFVAL_FREQ0_915  0x3B        // Frequency Control Word, Low Byte
// Carrier frequency = 433 MHz
#define CC1101_DEFVAL_FREQ2_433  0x10        // Frequency Control Word, High Byte
#define CC1101_DEFVAL_FREQ1_433  0xB0        // Frequency Control Word, Middle Byte
#define CC1101_DEFVAL_FREQ0_433  0x71        // Frequency Control Word, Low Byte

#define CC1101_DEFVAL_FREQ2_870  0x21        // Frequency Control Word, High Byte
#define CC1101_DEFVAL_FREQ1_870  0x76        // Frequency Control Word, Middle Byte
#define CC1101_DEFVAL_FREQ0_870  0x27        // Frequency Control Word, Low Byte


#define CC1101_DEFVAL_FREQ2_315            0x0C
#define CC1101_DEFVAL_FREQ1_315            0x1D
#define CC1101_DEFVAL_FREQ0_315            0x42

#define CC1101_DEFVAL_MDMCFG4          0xF8
#define CC1101_DEFVAL_MDMCFG3          0x9B
//#define CC1101_DEFVAL_MDMCFG4          0xC9
//#define CC1101_DEFVAL_MDMCFG3          0xFF
#define CC1101_DEFVAL_MDMCFG2          0x33
#define CC1101_DEFVAL_MDMCFG1          0x22
#define CC1101_DEFVAL_MDMCFG0          0xF8
#define CC1101_DEFVAL_DEVIATN          0x00
#define CC1101_DEFVAL_MCSM2            0x07
//#define CC1101_DEFVAL_MCSM1      0x30        // Main Radio Control State Machine Configuration
#define CC1101_DEFVAL_MCSM1            0x30
#define CC1101_DEFVAL_MCSM0            0x18
#define CC1101_DEFVAL_FOCCFG           0x16
#define CC1101_DEFVAL_BSCFG            0x6C
#define CC1101_DEFVAL_AGCCTRL2         0x03
//#define CC1101_DEFVAL_AGCCTRL2         0x00
#define CC1101_DEFVAL_AGCCTRL1         0x40
#define CC1101_DEFVAL_AGCCTRL0         0x91
#define CC1101_DEFVAL_WOREVT1          0x87
#define CC1101_DEFVAL_WOREVT0          0x6B
#define CC1101_DEFVAL_WORCTRL          0xFB
#define CC1101_DEFVAL_FREND1           0x56
#define CC1101_DEFVAL_FREND0           0x11
#define CC1101_DEFVAL_FSCAL3           0xE9
#define CC1101_DEFVAL_FSCAL2           0x2A
#define CC1101_DEFVAL_FSCAL1           0x00
#define CC1101_DEFVAL_FSCAL0           0x1F
#define CC1101_DEFVAL_RCCTRL1          0x41
#define CC1101_DEFVAL_RCCTRL0          0x00
#define CC1101_DEFVAL_FSTEST           0x59
#define CC1101_DEFVAL_PTEST            0x7F
#define CC1101_DEFVAL_AGCTEST          0x3F
#define CC1101_DEFVAL_TEST2            0x81
#define CC1101_DEFVAL_TEST1            0x35
#define CC1101_DEFVAL_TEST0            0x0B

#define CC1101_Data_IOCFG2     0x29
#define CC1101_Data_IOCFG1     0x2E
#define CC1101_Data_IOCFG0     0x06
#define CC1101_Data_FIFOTHR    0x47
#define CC1101_Data_SYNC1      0xD3
#define CC1101_Data_SYNC0      0x91
#define CC1101_Data_PKTLEN     0xFF

#define CC1101_Data_PKTCTRL1   0x00
#define CC1101_Data_PKTCTRL0   0x30

#define CC1101_Data_ADDR       0x00
#define CC1101_Data_CHANNR     0x00
#define CC1101_Data_FSCTRL1    0x06
#define CC1101_Data_FSCTRL0    0x00
// Carrier frequency = 868 MHz
#define CC1101_Data_FREQ2_868  0x21        // Frequency Control Word, High Byte
#define CC1101_Data_FREQ1_868  0x62        // Frequency Control Word, Middle Byte
#define CC1101_Data_FREQ0_868  0x76        // Frequency Control Word, Low Byte
// Carrier frequency = 902 MHz
#define CC1101_Data_FREQ2_915  0x22        // Frequency Control Word, High Byte
#define CC1101_Data_FREQ1_915  0xB1        // Frequency Control Word, Middle Byte
#define CC1101_Data_FREQ0_915  0x3B        // Frequency Control Word, Low Byte
// Carrier frequency = 433 MHz
#define CC1101_Data_FREQ2_433  0x10        // Frequency Control Word, High Byte
#define CC1101_Data_FREQ1_433  0xB0        // Frequency Control Word, Middle Byte
#define CC1101_Data_FREQ0_433  0x71        // Frequency Control Word, Low Byte

#define CC1101_Data_FREQ2_315            0x0C
#define CC1101_Data_FREQ1_315            0x1D
#define CC1101_Data_FREQ0_315            0x2f

#define CC1101_Data_MDMCFG4    0xF8
#define CC1101_Data_MDMCFG3    0x93
#define CC1101_Data_MDMCFG2    0x33
#define CC1101_Data_MDMCFG1    0x22
#define CC1101_Data_MDMCFG0    0xF8
#define CC1101_Data_DEVIATN    0x00
#define CC1101_Data_MCSM2      0x07
//#define CC1101_Data_MCSM1      0x30        // Main Radio Control State Machine Configuration
#define CC1101_Data_MCSM1            0x20
#define CC1101_Data_MCSM0            0x18
#define CC1101_Data_FOCCFG           0x16
#define CC1101_Data_BSCFG            0x6C

#define CC1101_Data_AGCCTRL2         0x00
#define CC1101_Data_AGCCTRL1         0x40

#define CC1101_Data_AGCCTRL0         0x91
#define CC1101_Data_WOREVT1          0x87
#define CC1101_Data_WOREVT0          0x6B
#define CC1101_Data_WORCTRL          0xFB
#define CC1101_Data_FREND1           0x56
#define CC1101_Data_FREND0           0x11
#define CC1101_Data_FSCAL3           0xE9
#define CC1101_Data_FSCAL2           0x2A
#define CC1101_Data_FSCAL1           0x00
#define CC1101_Data_FSCAL0           0x1F
#define CC1101_Data_RCCTRL1          0x41
#define CC1101_Data_RCCTRL0          0x00
#define CC1101_Data_FSTEST           0x59
#define CC1101_Data_PTEST            0x7F
#define CC1101_Data_AGCTEST          0x3F
#define CC1101_Data_TEST2            0x81
#define CC1101_Data_TEST1            0x35
#define CC1101_Data_TEST0            0x0B


class CC1101 {
private:
    SPI spi;

    void setDefaultRegs_2(void);

    void setTmpsRegs(void);

public:

    void readBurstReg(byte *buffer, byte regAddr, byte len);

    byte rfState;
    int SS_PIN = 10;
    int GDO0_PIN = 2;
    byte paTableByte;
    byte carrierFreq;
    byte channel;
    byte syncWord[2];
    byte devAddress;

    CC1101(void);

    byte ReadByte(char *name, byte regadd);

    byte *ReadByte(char *name, byte regadd, int num);

    byte *ReadByte(byte regadd, int num);

    void PrintConfig();


    void writeBurstReg(byte regAddr, byte *buffer, byte len);

    /*
    void SendPacket(byte * txbuffer,int length);
      void SendPacket_1(byte * txbuffer,int length);
      byte RecPacket(byte * rxBuffer);

      void init_2(void);
      void init_Hackpwn(void);


      boolean sendData(CCPACKET packet);
    boolean sendData_1(CCPACKET packet);
    void halRfSendData(CCPACKET txData);
    void Send_Data(CCPACKET packet);
    byte receiveData(CCPACKET *packet);
    byte receiveData_1(CCPACKET *packet);
    byte receive(CCPACKET *packet);
      */
    void setCarrierFreq(byte freq);

    byte readrssi();

    void cmdStrobe(byte cmd);

    void wakeUp(void);

    byte readReg(byte regAddr, byte regType);

    void writeReg(byte regAddr, byte value);

    void reset(void);

    void init(void);

    void initTmps(void);


    void setSyncWord(uint8_t syncH, uint8_t syncL);

    void setSyncWord(byte *sync);


    void setDevAddress(byte addr);

    void setChannel(byte chnl);

    void setPowerDownState();

    void Write_Cmd(uint8_t cmd);

};

#endif
