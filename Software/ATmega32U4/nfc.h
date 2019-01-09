/*
 * 
 */
#include "em4095.h"
EM4095 EM4095;

int NFC_VD;
int NFC_action;

unsigned long long NFC_data;
unsigned long NFC_ID;

unsigned long NFC_START;
unsigned long NFC_END;

/*
 *  通过轮询EM4095的OUT输出引脚解码卡号数据
*/

void SnifferEM4100(){
int EM4100_VD;
unsigned long EM4100_DATA;

  if (EM4095.decodeTag(EM4100_DATA, EM4100_VD)) {
    if (millis() - SerialLastTime < Print_Time) {
      //mySwitch.resetAvailable();
      return;
    } else if (EM4100_DATA < 1) {
      return;
    }
    Cube_PRINT.print("[NFC][read]vd:");
    Cube_PRINT.print(EM4100_VD);
    Cube_PRINT.print(",data:");
    Cube_PRINT.println(EM4100_DATA);
    EM4100_DATA = 0;
    EM4100_VD = 0;
    SerialLastTime = millis();
  }
}

/*
 *  通过控制EM4095 MOD 引脚模拟卡片耦合进行模拟卡操作
 */

void simulation() {
  unsigned long int udata = 6834998;
  EM4095.simulation(0x0c, udata);
}

/*
 *  控制EM4095 SHD 和 MOD 引脚，使其进入休眠状态进行低功耗
 */
void NFC_Switch(String action) {
  //pinMode(11,OUTPUT);
  if (action == "off") {
    
    digitalWrite(MOD, HIGH);
    digitalWrite(SHD, HIGH);
    //digitalWrite(11,HIGH);
  } else if (action == "on") {
    digitalWrite(MOD, LOW);
    digitalWrite(SHD, LOW);
    //digitalWrite(12,LOW);
  } else {
  }
}
bool NFC_Action() {
  return !digitalRead(MOD);
}

/*
 * 爆破卡号程序
 */
void NFC_attack(int vd, unsigned long start, unsigned long end ) {
  //NFC_ID=start;
  // 35 Ms / count;15==500MS
  for (int i = 0; i < 15; i++) {
    EM4095.simulation(NFC_VD, NFC_ID);
  }
  int NFC_porg = map(NFC_ID, start, end, 0, 100);
  if (NFC_porg > 100)
    NFC_porg = 100;
  Cube_PRINT.print("[NFC][progress]");
  Cube_PRINT.println(NFC_porg);
  //Serial.println(NFC_ID);
  if (NFC_ID == end )
    NFC_action = 0;

  NFC_ID++;
}

/*
 * 根据指令去模拟/爆破EM4100系列卡片
 */
void AttackEM4100(){
    while (NFC_action) {
    switch (NFC_action) {
      case 1:
        EM4095.simulation(NFC_VD, NFC_ID);
        break;
      case 2:
        NFC_attack(NFC_VD, NFC_START, NFC_END);
        break;
    }
    if (Cube_PRINT.available() > 0) {
      NFC_action = 0;
      Cube_PRINT.println("exit");
    }
  }
}

//String nfcdata="1111111110000011000000000000001100100010100110111001100110010000";
//
//unsigned long long nfcdata = 0xFF830003229b9990;
//0006834998



void nfcbit1() {
  digitalWrite(MOD, LOW);
  delayMicroseconds(250);
  digitalWrite(MOD, HIGH);
  delayMicroseconds(250);
}
void nfcbit0() {
  digitalWrite(MOD, HIGH);
  delayMicroseconds(250);
  digitalWrite(MOD, LOW);
  delayMicroseconds(250);
}
