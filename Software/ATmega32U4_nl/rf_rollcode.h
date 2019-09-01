#include "HackCube.h"
/*
    CC1101
*/


//#include "cc1101.h"
//CC1101 cc1101;


void RF433_Setup_rollcode() {
  cc1101.SS_PIN = RF433_CS;
  cc1101.init();
  //byte syncWord[2] = {0x00, 0x00};
  //cc1101.setSyncWord(syncWord);
  //freq_cc1101=433920000;
  setfreqx(freq_cc1101);
  //cc1101.setCarrierFreq(CFREQ_433);

  //cc1101.disableAddressCheck();
  cc1101.cmdStrobe(CC1101_SRX);
  cc1101.writeReg(CC1101_IOCFG2, 0x0d);
  //cc1101.writeReg(CC1101_AGCCTRL2, 0xB0);
  //cc1101.writeReg(CC1101_MDMCFG4, 0xc8);
  //cc1101.writeReg(CC1101_MDMCFG4, 0xc8);
  //cc1101.writeReg(CC1101_MDMCFG3, 0x93);

  cc1101.writeReg(CC1101_MDMCFG4, 0x07);//RFB 200K
  cc1101.writeReg(CC1101_MDMCFG3, 0x93);
}

/*
    Keeloq keyfob attack
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef unsigned long u32;
typedef unsigned long long u64;
uint64_t key = 0x7d093b66b31c374a;
#define KeeLoq_NLF    0x3A5C742E
#define bit(x,n)    (((x)>>(n))&1)
#define g5(x,a,b,c,d,e) (bit(x,a)+bit(x,b)*2+bit(x,c)*4+bit(x,d)*8+bit(x,e)*16)
uint32_t  KeeLoq_Encrypt (const uint32_t data, const uint64_t key)
{
  uint32_t  x = data, r;
  for (r = 0; r < 528; r++) {
    x = (x >> 1) ^ ((bit(x, 0)^bit(x, 16) ^ (uint32_t)bit(key, r & 63)^bit(KeeLoq_NLF, g5(x, 1, 9, 20, 26, 31))) << 31);
  }
  return x;
}
uint32_t  KeeLoq_Decrypt (const uint32_t data, const uint64_t key)
{
  uint32_t   x = data, r;
  for (r = 0; r < 528; r++) {
    x = (x << 1)^bit(x, 31)^bit(x, 15) ^ (uint32_t)bit(key, (15 - r) & 63)^bit(KeeLoq_NLF, g5(x, 0, 8, 19, 25, 30));
  }
  return x;
}


/*
    receive Keeloq data
*/
#include "HackRadio.h"
HackRadio Hackradio = HackRadio();

void SnifferRollcode() {
  if (Hackradio.available()) {

    detachInterrupt(RF315_att);
    // if(mySwitch.getsn()=="39182da"){
    // Cube_PRINT.println("*  Receive");
    unsigned long rfdata = strtoul(Hackradio.getkey().c_str(), NULL, 16);
    //uint32_t datadc = KeeLoq_Decrypt(rfdata, key);
    Cube_PRINT.print("[RF][Sniffer]");
    Cube_PRINT.print("freq:");
    Cube_PRINT.print(String(freq_cc1101).substring(0, 3));
    Cube_PRINT.print(",protocol:keeloq,modulation:ask,");
    Cube_PRINT.print("data:");
    Cube_PRINT.print(Hackradio.getkey());
    Cube_PRINT.print(",SerialNumber:");
    Cube_PRINT.print(Hackradio.getsn());
    /*   Cube_PRINT.print("  Func:");
       Cube_PRINT.println("");
       Cube_PRINT.print("Decrypt:");
       Cube_PRINT.print(datadc, HEX);
       Cube_PRINT.print("-->Func:");
       unsigned int  Func = datadc >> 28;
       //Func=Func<<28;
       Cube_PRINT.print(Func, HEX);
       Cube_PRINT.print(" Distinguish Code:");
       uint32_t   Distinguish = datadc << 6;
       Distinguish = Distinguish >> 22;
       Cube_PRINT.print(Distinguish, HEX);
       Cube_PRINT.print(" Sync Counter Value:");
       uint32_t   SyncCounterValue = datadc << 16;
       SyncCounterValue = SyncCounterValue >> 16;
       Cube_PRINT.print(SyncCounterValue, HEX);
       //u8g.drawRFrame(10,12, 30,20, 5);*/
    Cube_PRINT.println("");
    Hackradio.resetAvailable();
    Hackradio.enableReceive(RF315_att);
  }
}
