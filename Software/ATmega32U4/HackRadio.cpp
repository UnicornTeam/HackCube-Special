#include "HackRadio.h"
#include <string.h>

//#if defined(__AVR_ATmega1280__)

//#if defined (MCU_STM32F103R)
#if defined(__STM32F1__) 
#include "itoa.h"
#endif
//



unsigned long HackRadio::nReceivedValue = NULL;
String HackRadio::nsn;
String HackRadio::nkey;
String HackRadio::action;
unsigned int HackRadio::nReceivedBitlength = 0;
unsigned int HackRadio::nReceivedDelay = 0;
unsigned int HackRadio::nReceivedProtocol = 0;
int HackRadio::nReceiveTolerance = 60;
unsigned int HackRadio::timings[HackRadio_MAX_CHANGES];


HackRadio::HackRadio() {
  this->nTransmitterPin = -1;
  this->setPulseLength(100);//长度
  this->setRepeatTransmit(4);//次数
  this->setProtocol(1);//方法

  this->nReceiverInterrupt = -1;
  this->setReceiveTolerance(60);
  HackRadio::nReceivedValue = NULL;

}


void HackRadio::Begin(void){

};





String HackRadio::getsn() {
  return HackRadio::nsn;
}
String HackRadio::getkey() {
  return HackRadio::nkey;
}
String HackRadio::getaction() {
  return HackRadio::action;
}


void HackRadio::enableTransmit(int nTransmitterPin) {
  this->nTransmitterPin = nTransmitterPin;
  pinMode(this->nTransmitterPin, OUTPUT);
}



void HackRadio::sendPreamble() {
    this->transmit(360,389);//435
}

void HackRadio::transmit(int nHighPulses, int nLowPulses) {
    digitalWrite(this->nTransmitterPin, HIGH);
    delayMicroseconds( nHighPulses);
    digitalWrite(this->nTransmitterPin, LOW);
    delayMicroseconds( nLowPulses);
}


void HackRadio::Send0() {
      
      this->transmit(721,390);//690 420
      //Serial.print("0");
}
void HackRadio::Send1() {
      this->transmit(371,756);//815
      //Serial.print("1");
}


void HackRadio::Send(unsigned long data,unsigned long sn,int action,int Repeat){
  for (int nRepeat=0; nRepeat<Repeat; nRepeat++) {
    int i = 0;
    for (int a=0;a<11;a++){
      this->sendPreamble();
    }
    this->transmit(360,3769);//202

    for (int i = 0; i < 32; i++) {
      if ((data >> i)&1)
        this->Send1();
      else
        this->Send0();
    }

    for (int i = 0; i < 28; i++) {
      if ((sn >> i)&1)
        this->Send1();
      else
        this->Send0();
    }

    for (int i = 0; i < 5; i++) {
      if ((action >> i)&1)
        this->Send1();
      else
        this->Send0();
    }

    this->transmit(300,15400);//202

  }
}



bool HackRadio::available() {
  return HackRadio::nReceivedValue != NULL;
}

void HackRadio::resetAvailable() {
  HackRadio::nReceivedValue = NULL;
}


void HackRadio::setProtocol(int nProtocol) {
  this->nProtocol = nProtocol;
  if (nProtocol == 1){
    this->setPulseLength(245);
  }
  else if (nProtocol == 2) {
    this->setPulseLength(650);
  }
  else if (nProtocol == 3) {
    this->setPulseLength(100);
  }
}

void HackRadio::setProtocol(int nProtocol, int nPulseLength) {
  this->nProtocol = nProtocol;
  this->setPulseLength(nPulseLength);
}

void HackRadio::setPulseLength(int nPulseLength) {
  this->nPulseLength = nPulseLength;
}

void HackRadio::setRepeatTransmit(int nRepeatTransmit) {
  this->nRepeatTransmit = nRepeatTransmit;
}

void HackRadio::setReceiveTolerance(int nPercent) {
  HackRadio::nReceiveTolerance = nPercent;
}


void HackRadio::enableReceive(int interrupt) {
  this->nReceiverInterrupt = interrupt;
  this->enableReceive();
}


void HackRadio::enableReceive() {
  if (this->nReceiverInterrupt != -1) {
    HackRadio::nReceivedValue = NULL;
    HackRadio::nReceivedBitlength = NULL;
    HackRadio::nsn;
    HackRadio::nkey;
    HackRadio::action;
    attachInterrupt(this->nReceiverInterrupt, handleInterrupt, CHANGE);
  }
}


void HackRadio::handleInterrupt() {

  static unsigned int duration;
  static unsigned int changeCount;
  static unsigned long lastTime;
  static unsigned int repeatCount;
 
  long time = micros();
  duration = time - lastTime;

  if (
     changeCount > 155
    && duration > HackRadio::timings[0] - 4000 
    && duration < HackRadio::timings[0] + 4000
     &&  HackRadio::timings[24]<4500
     &&  HackRadio::timings[24]>3500
    ) {
    //Serial.println(changeCount);
    receiveProtocol2(changeCount);
    HackRadio::nReceivedValue = 1;
    changeCount = 0;
  } else if (duration > 13000) {
    changeCount = 0;
  }


  
  if (changeCount >= HackRadio_MAX_CHANGES) {
    changeCount = 0;
    repeatCount = 0;
  }
  HackRadio::timings[changeCount++] = duration;
  lastTime = time;  
}

bool HackRadio::receiveProtocol2(unsigned int changeCount){
  String sn;
  String tiaoma;
  String action;
  unsigned long delay = HackRadio::timings[0] / 31;
  unsigned long delayTolerance = delay * HackRadio::nReceiveTolerance * 0.01; 
      for (int i = 25 ; i< 89 ; i=i+2){
        if (HackRadio::timings[i] < HackRadio::timings[i+1]){      
          tiaoma +="0";
        } else if (HackRadio::timings[i] > HackRadio::timings[i+1]) {
          tiaoma +="1";
        }
    }
    for (int i = 89 ; i< 145 ; i=i+2) { 
        if (HackRadio::timings[i] < HackRadio::timings[i+1]) {
          sn +="0";
        } else if (HackRadio::timings[i] > HackRadio::timings[i+1]) {
          sn +="1";
        }
    }
    for (int i=145;i<153;i=i+2){
        if (HackRadio::timings[i] < HackRadio::timings[i+1]) {
          action +="0";
        } else if (HackRadio::timings[i] > HackRadio::timings[i+1]) {
          action +="1";
        }
    }
  HackRadio::action =dec4bin(action);
  HackRadio::nkey =dec4bin(tiaoma);
  HackRadio::nsn =dec4bin(sn);
  HackRadio::nReceivedValue = 1;
  HackRadio::nReceivedProtocol = 4;
  if (tiaoma == 0){
      return false;
  }else if (tiaoma != 0){
      return true;
  }
}

//因为　编译　链路　　不通
/*
String HackRadio::dec4bin(String data){
String ss;
int len = data.length();
String sss;
//data.copy(datas,len,0);
//cout<<data;
if (len==28){
    for (int i=27;i>=0;i--){
      sss = data[i];
     if(sss=="0"){
      ss += "1";
     }else if(sss=="1"){
      ss += "0";
     }
    }
    unsigned long dataa=strtoul(ss.c_str(),NULL,2);
    char  string[64];
    ultoa(dataa, string, 16);
    return string;
  }else if(len==32){
    for (int i=31;i>=0;i--){
      sss = data[i];
    if(sss=="0"){
      ss += "1";
     }else if(sss=="1"){
      ss += "0";
     }
    }
    unsigned long dataa=strtoul(ss.c_str(),NULL,2);
    char  string[64];
    ultoa(dataa, string, 16);
    return string;
  }else if(len==4){
   for (int i=3;i>=0;i--){
      sss = data[i];
    if(sss=="0"){
      ss += "1";
     }else if(sss=="1"){
      ss += "0";
     }
    }
    unsigned long dataa=strtoul(ss.c_str(),NULL,2);
    char  string[64];
    ultoa(dataa, string, 16);
    return string;
  }
}*/
String HackRadio::dec4bin(String data){
String ss;
int len = data.length();
String sss;
//data.copy(datas,len,0);
//cout<<data;
if (len==28){
    for (int i=27;i>=0;i--){
      sss = String(data[i]);
     if(sss=="0"){
      ss += "1";
     }else if(sss=="1"){
      ss += "0";
     }
    }
    unsigned long dataa=strtoul(ss.c_str(),NULL,2);
    char  string[64];
    ultoa(dataa, string, 16);
    return string;
  }else if(len==32){
    for (int i=31;i>=0;i--){
      sss =String(data[i]);
    if(sss=="0"){
      ss += "1";
     }else if(sss=="1"){
      ss += "0";
     }
    }
    unsigned long dataa=strtoul(ss.c_str(),NULL,2);
    char  string[64];
    ultoa(dataa, string, 16);
    return string;
  }else if(len==4){
   for (int i=3;i>=0;i--){
      sss = String(data[i]);
    if(sss=="0"){
      ss += "1";
     }else if(sss=="1"){
      ss += "0";
     }
    }
    unsigned long dataa=strtoul(ss.c_str(),NULL,2);
    char  string[64];
    ultoa(dataa, string, 16);
    return string;
  }
}
