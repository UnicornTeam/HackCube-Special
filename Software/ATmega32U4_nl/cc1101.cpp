#include "cc1101.h"

//#define cc1101_Select()  bitClear(PORT_SPI_SS, BIT_SPI_SS) //digitalWrite(10,HIGH)
#define cc1101_Select()  digitalWrite(SS_PIN,LOW)
// Deselect (SPI) CC1101
//#define cc1101_Deselect()  bitSet(PORT_SPI_SS, BIT_SPI_SS)//digitalWrite(10,LOW)
#define cc1101_Deselect()  digitalWrite(SS_PIN,HIGH)
//#define wait_Miso()  while(bitRead(PORT_SPI_MISO, BIT_SPI_MISO))
#define wait_Miso() 
// Get GDO0 pin state
#define getGDO0state()  bitRead(GDO0_PIN, BIT_GDO0)
// Wait until GDO0 line goes high
#define wait_GDO0_high()  while(!getGDO0state())
// Wait until GDO0 line goes low
#define wait_GDO0_low()  while(getGDO0state())

//const byte paTable[8] = {0x00, 0xc2, 0x00, 0x00, 0xc2, 0xc2, 0xc2, 0xc2};
const byte paTable[8] = {0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


CC1101::CC1101(void)
{

}


void CC1101::writeReg(byte regAddr, byte value) 
{
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  spi.send(regAddr);                    // Send register address
  spi.send(value);                      // Send value
  cc1101_Deselect();                    // Deselect CC1101
}

/**
 * writeBurstReg
 * 
 * Write multiple registers into the CC1101 IC via SPI
 * 
 * 'regAddr'	Register address
 * 'buffer'	Data to be writen
 * 'len'	Data length
 */
void CC1101::writeBurstReg(byte regAddr, byte* buffer, byte len)
{
  byte addr, i;
  
  addr = regAddr | WRITE_BURST;         // Enable burst transfer
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  spi.send(addr);                       // Send register address
  
  for(i=0 ; i<len ; i++)
    spi.send(buffer[i]);                // Send value

  cc1101_Deselect();                    // Deselect CC1101  
}

/**
 * cmdStrobe
 * 
 * Send command strobe to the CC1101 IC via SPI
 * 
 * 'cmd'	Command strobe
 */     
void CC1101::cmdStrobe(byte cmd) 
{
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  spi.send(cmd);                        // Send strobe command
  cc1101_Deselect();                    // Deselect CC1101
}

/**
 * readReg
 * 
 * Read CC1101 register via SPI
 * 
 * 'regAddr'	Register address
 * 'regType'	Type of register: CC1101_CONFIG_REGISTER or CC1101_STATUS_REGISTER
 * 
 * Return:
 * 	Data byte returned by the CC1101 IC
 */
byte CC1101::readReg(byte regAddr, byte regType) 
{
  byte addr, val;

  addr = regAddr | regType;
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  spi.send(addr);                       // Send register address
  val = spi.send(0x00);                 // Read result
  cc1101_Deselect();                    // Deselect CC1101

  return val;
}




/**
 * readBurstReg
 * 
 * Read burst data from CC1101 via SPI
 * 
 * 'buffer'	Buffer where to copy the result to
 * 'regAddr'	Register address
 * 'len'	Data length
 */
void CC1101::readBurstReg(byte * buffer, byte regAddr, byte len) 
{
  byte addr, i;
  
  addr = regAddr | READ_BURST;
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  spi.send(addr);                       // Send register address
  for(i=0 ; i<len ; i++)
    buffer[i] = spi.send(0x00);         // Read result byte by byte
  cc1101_Deselect();                    // Deselect CC1101
}

byte CC1101::ReadByte(char * name ,byte regadd){
Serial.print(name);
Serial.print("    ");
Serial.println(readStatusReg(regadd),HEX);

}
byte  * CC1101::ReadByte(char * name ,byte regadd,int num){
byte  data[num];
signed int i;
Serial.print(name);
Serial.print("    ");
while(num--){
 data[i++]=readStatusReg(regadd++);
  Serial.print(data[(i)-1],HEX);
  Serial.print(" ");
}
Serial.println("");
return data;
}
byte  * CC1101::ReadByte(byte regadd,int num){
byte  data[num];
signed int i;

while(num--){
 data[i++]=readStatusReg(regadd++);
}
return data;
}

 

void CC1101::PrintConfig(){
	ReadByte("vr",CC1101_VERSION);
    ReadByte("CH",CC1101_CHANNR);
    ReadByte("FREQ",CC1101_FREQ2,3);
    ReadByte("Modulation",CC1101_FREND0);
    ReadByte("Data Rate",CC1101_MDMCFG4,2);
	ReadByte("PKTCTRL0 ",CC1101_PKTCTRL0,1);
//PKTCTRL0
}

/**
 * reset
 * 
 * Reset CC1101
 */
void CC1101::reset(void) 
{
  
  cc1101_Deselect();                    // Deselect CC1101
  delayMicroseconds(5);
  cc1101_Select();                      // Select CC1101
  delayMicroseconds(10);
  cc1101_Deselect();                    // Deselect CC1101
  delayMicroseconds(41);
  cc1101_Select();                      // Select CC1101

  //wait_Miso();                          // Wait until MISO goes low
  delay(10);
  spi.send(CC1101_SRES);                // Send reset command strobe
  //wait_Miso();                          // Wait until MISO goes low
  delay(10);
  cc1101_Deselect();                    // Deselect CC1101

 //setRegsFromEeprom();                  // Take user settings from EEPROM
}

void CC1101::wakeUp(void)
{         
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  cc1101_Deselect();                    // Deselect CC1101
}





void CC1101::init(void) 
{
	pinMode(SS_PIN, OUTPUT);
	spi.init();                          
	pinMode(GDO0, INPUT);                 
	delay(4);
	reset();                              
             
	setDefaultRegs_2();//数据包模式
	writeBurstReg(CC1101_PATABLE, (byte*)paTable, 8);
}


void CC1101::initTmps(void) 
{
  pinMode(SS_PIN, OUTPUT);
  spi.init();                          
  pinMode(GDO0, INPUT);                 
  delay(4);
  reset();                              
             
  setTmpsRegs();//数据包模式
  writeBurstReg(CC1101_PATABLE, (byte*)paTable, 8);
}


void CC1101::setSyncWord(uint8_t syncH, uint8_t syncL) 
{
  if ((syncWord[0] != syncH) || (syncWord[1] != syncL))
  {
    writeReg(CC1101_SYNC1, syncH);
    writeReg(CC1101_SYNC0, syncL);
    syncWord[0] = syncH;
    syncWord[1] = syncL;
    // Save in EEPROM
  }
}

void CC1101::setSyncWord(byte *sync) 
{
  CC1101::setSyncWord(sync[0], sync[1]);
}

/**
 * setDevAddress
 * 
 * Set device address
 * 
 * 'addr'	Device address
 * 'save' If TRUE, save parameter in EEPROM
 */
void CC1101::setDevAddress(byte addr) 
{
  if (devAddress != addr)
  {
    writeReg(CC1101_ADDR, addr);
    devAddress = addr;
    // Save in EEPROM
  }
}

/**
 * setChannel
 * 
 * Set frequency channel
 * 
 * 'chnl'	Frequency channel
 * 'save' If TRUE, save parameter in EEPROM
 */
void CC1101::setChannel(byte chnl) 
{
  if (channel != chnl)
  {
    writeReg(CC1101_CHANNR,  chnl);
    channel = chnl;
    // Save in EEPROM
  }
}

void CC1101::setPowerDownState() 
{
  // Comming from RX state, we need to enter the IDLE state first
  cmdStrobe(CC1101_SIDLE);
  // Enter Power-down state
  cmdStrobe(CC1101_SPWD);
}


void CC1101::Write_Cmd(uint8_t cmd)
{
  digitalWrite(SPI_SS,LOW);
  spi.send(0x00 | cmd);   // Schrijf opdrachtbesturingswoord
  digitalWrite(SPI_SS,HIGH);                           // CSn staat hoog en beëindigt het datatransmissieproces
}




byte CC1101::readrssi() 
{
  byte addr, val;
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  spi.send(CC1101_RSSI|CC1101_STATUS_REGISTER); 
   val = spi.send(0x00);                 // Read result
   cc1101_Deselect();  
  return val;
 }



void CC1101::setCarrierFreq(byte freq)
{
  switch(freq)
  {
    case CFREQ_915:
      writeReg(CC1101_FREQ2,  CC1101_Data_FREQ2_915);
      writeReg(CC1101_FREQ1,  CC1101_Data_FREQ1_915);
      writeReg(CC1101_FREQ0,  CC1101_Data_FREQ0_915);
      break;
    case CFREQ_433:
      writeReg(CC1101_FREQ2,  CC1101_Data_FREQ2_433);
      writeReg(CC1101_FREQ1,  CC1101_Data_FREQ1_433);
      writeReg(CC1101_FREQ0,  CC1101_Data_FREQ0_433);
      break;

    case CFREQ_868:
      writeReg(CC1101_FREQ2,  CC1101_Data_FREQ2_868);
      writeReg(CC1101_FREQ1,  CC1101_Data_FREQ1_868);
      writeReg(CC1101_FREQ0,  CC1101_Data_FREQ0_868);
      break;
    default:
      writeReg(CC1101_FREQ2,  CC1101_Data_FREQ2_315);
      writeReg(CC1101_FREQ1,  CC1101_Data_FREQ1_315);
      writeReg(CC1101_FREQ0,  CC1101_Data_FREQ0_315);
  }
   
  carrierFreq = freq;  
}



void CC1101::setTmpsRegs(void) 
{
  writeReg(CC1101_IOCFG2,  CC1101_DEFVAL_IOCFG2);
  writeReg(CC1101_IOCFG1,  CC1101_DEFVAL_IOCFG1);
  writeReg(CC1101_IOCFG0,  CC1101_DEFVAL_IOCFG0);
  writeReg(CC1101_FIFOTHR,  CC1101_DEFVAL_FIFOTHR);
  writeReg(CC1101_PKTLEN,  CC1101_DEFVAL_PKTLEN);
  writeReg(CC1101_PKTCTRL1,  CC1101_DEFVAL_PKTCTRL1);
  writeReg(CC1101_PKTCTRL0,  CC1101_DEFVAL_PKTCTRL0);

  // Set default synchronization word
  setSyncWord(CC1101_DEFVAL_SYNC1, CC1101_DEFVAL_SYNC0);

  // Set default device address
  setDevAddress(CC1101_DEFVAL_ADDR);
  // Set default frequency channel
  setChannel(CC1101_DEFVAL_CHANNR);
  
  writeReg(CC1101_FSCTRL1,  CC1101_DEFVAL_FSCTRL1);
  writeReg(CC1101_FSCTRL0,  CC1101_DEFVAL_FSCTRL0);

  // Set default carrier frequency = 868 MHz
  setCarrierFreq(CFREQ_868);

  writeReg(CC1101_MDMCFG4,  CC1101_DEFVAL_MDMCFG4);
  writeReg(CC1101_MDMCFG3,  CC1101_DEFVAL_MDMCFG3);
  writeReg(CC1101_MDMCFG2,  CC1101_DEFVAL_MDMCFG2);
  writeReg(CC1101_MDMCFG1,  CC1101_DEFVAL_MDMCFG1);
  writeReg(CC1101_MDMCFG0,  CC1101_DEFVAL_MDMCFG0);
  writeReg(CC1101_DEVIATN,  CC1101_DEFVAL_DEVIATN);
  writeReg(CC1101_MCSM2,  CC1101_DEFVAL_MCSM2);
  writeReg(CC1101_MCSM1,  CC1101_DEFVAL_MCSM1);
  writeReg(CC1101_MCSM0,  CC1101_DEFVAL_MCSM0);
  writeReg(CC1101_FOCCFG,  CC1101_DEFVAL_FOCCFG);
  writeReg(CC1101_BSCFG,  CC1101_DEFVAL_BSCFG);
  writeReg(CC1101_AGCCTRL2,  CC1101_DEFVAL_AGCCTRL2);
  writeReg(CC1101_AGCCTRL1,  CC1101_DEFVAL_AGCCTRL1);
  writeReg(CC1101_AGCCTRL0,  CC1101_DEFVAL_AGCCTRL0);
  writeReg(CC1101_WOREVT1,  CC1101_DEFVAL_WOREVT1);
  writeReg(CC1101_WOREVT0,  CC1101_DEFVAL_WOREVT0);
  writeReg(CC1101_WORCTRL,  CC1101_DEFVAL_WORCTRL);

  writeReg(CC1101_FREND1,  CC1101_DEFVAL_FREND1);
  writeReg(CC1101_FREND0,  CC1101_DEFVAL_FREND0);
  writeReg(CC1101_FSCAL3,  CC1101_DEFVAL_FSCAL3);
  writeReg(CC1101_FSCAL2,  CC1101_DEFVAL_FSCAL2);
  writeReg(CC1101_FSCAL1,  CC1101_DEFVAL_FSCAL1);
  writeReg(CC1101_FSCAL0,  CC1101_DEFVAL_FSCAL0);
  writeReg(CC1101_RCCTRL1,  CC1101_DEFVAL_RCCTRL1);
  writeReg(CC1101_RCCTRL0,  CC1101_DEFVAL_RCCTRL0);
  writeReg(CC1101_FSTEST,  CC1101_DEFVAL_FSTEST);
  writeReg(CC1101_PTEST,  CC1101_DEFVAL_PTEST);
  writeReg(CC1101_AGCTEST,  CC1101_DEFVAL_AGCTEST);
  writeReg(CC1101_TEST2,  CC1101_DEFVAL_TEST2);
  writeReg(CC1101_TEST1,  CC1101_DEFVAL_TEST1);
  writeReg(CC1101_TEST0,  CC1101_DEFVAL_TEST0);
}


void CC1101::setDefaultRegs_2(void){
 
 writeReg(CC1101_IOCFG2,  CC1101_Data_IOCFG2);
  writeReg(CC1101_IOCFG1,  CC1101_Data_IOCFG1);
  writeReg(CC1101_IOCFG0,  CC1101_Data_IOCFG0);
  writeReg(CC1101_FIFOTHR,  CC1101_Data_FIFOTHR);
  writeReg(CC1101_PKTLEN,  CC1101_Data_PKTLEN);
  writeReg(CC1101_PKTCTRL1,  CC1101_Data_PKTCTRL1);
  writeReg(CC1101_PKTCTRL0,  CC1101_Data_PKTCTRL0);
  // Set default synchronization word
  setSyncWord(CC1101_Data_SYNC1, CC1101_Data_SYNC0);
  // Set default device address
  setDevAddress(CC1101_Data_ADDR);
  // Set default frequency channel
  setChannel(CC1101_Data_CHANNR);
  writeReg(CC1101_FSCTRL1,  CC1101_Data_FSCTRL1);
  writeReg(CC1101_FSCTRL0,  CC1101_Data_FSCTRL0);
  // Set default carrier frequency = 868 MHz
  setCarrierFreq(CFREQ_433);

  writeReg(CC1101_MDMCFG4,  CC1101_Data_MDMCFG4);
  writeReg(CC1101_MDMCFG3,  CC1101_Data_MDMCFG3);
  writeReg(CC1101_MDMCFG2,  CC1101_Data_MDMCFG2);
  writeReg(CC1101_MDMCFG1,  CC1101_Data_MDMCFG1);
  writeReg(CC1101_MDMCFG0,  CC1101_Data_MDMCFG0);
  writeReg(CC1101_DEVIATN,  CC1101_Data_DEVIATN);
  writeReg(CC1101_MCSM2,  CC1101_Data_MCSM2);
  writeReg(CC1101_MCSM1,  CC1101_Data_MCSM1);
  writeReg(CC1101_MCSM0,  CC1101_Data_MCSM0);
  writeReg(CC1101_FOCCFG,  CC1101_Data_FOCCFG);
  writeReg(CC1101_BSCFG,  CC1101_Data_BSCFG);
  writeReg(CC1101_AGCCTRL2,  CC1101_Data_AGCCTRL2);
  writeReg(CC1101_AGCCTRL1,  CC1101_Data_AGCCTRL1);
  writeReg(CC1101_AGCCTRL0,  CC1101_Data_AGCCTRL0);
  writeReg(CC1101_WOREVT1,  CC1101_Data_WOREVT1);
  writeReg(CC1101_WOREVT0,  CC1101_Data_WOREVT0);
  writeReg(CC1101_WORCTRL,  CC1101_Data_WORCTRL);

  writeReg(CC1101_FREND1,  CC1101_Data_FREND1);
  writeReg(CC1101_FREND0,  CC1101_Data_FREND0);
  writeReg(CC1101_FSCAL3,  CC1101_Data_FSCAL3);
  writeReg(CC1101_FSCAL2,  CC1101_Data_FSCAL2);
  writeReg(CC1101_FSCAL1,  CC1101_Data_FSCAL1);
  writeReg(CC1101_FSCAL0,  CC1101_Data_FSCAL0);
  writeReg(CC1101_RCCTRL1,  CC1101_Data_RCCTRL1);
  writeReg(CC1101_RCCTRL0,  CC1101_Data_RCCTRL0);
  writeReg(CC1101_FSTEST,  CC1101_Data_FSTEST);
  writeReg(CC1101_PTEST,  CC1101_Data_PTEST);
  writeReg(CC1101_AGCTEST,  CC1101_Data_AGCTEST);
  writeReg(CC1101_TEST2,  CC1101_Data_TEST2);
  writeReg(CC1101_TEST1,  CC1101_Data_TEST1);
  writeReg(CC1101_TEST0,  CC1101_Data_TEST0);


} 
