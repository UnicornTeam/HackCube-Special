/*
  通过串口与ESP8266进行通信将数据显示到Web后台，后台的发射、攻击的操作通过串口传输到32U4中进行执行
  32U4 -> esp8266 将嗅探到的数据反馈到Web后台中
  通过CC1101/nRF24L01 嗅探的到的固定码/滚动码数据
  [RF][Sniffer]freq:315,protocol:Fixed-1,modulation:ask,func:1,data:B710
  [RF][Sniffer]freq:2.4G,protocol:rf24l01,modulation:gfsk,data:401FFFFFF02AE9
  [RF][Sniffer]freq:433,protocol:keeloq,modulation:ask,data:2d6f28d4,SerialNumber:39182da
  CC1101 智能干扰模式下的信号探测数据
  [RF][Jam]-9b 433Mhz/Jamming.../[RF][Jam]Not Jamming
  EM4095 读取到的卡号信息
  [NFC][read]vd:123,data:1432455555
  攻击模式下的爆破进度
  [NFC][progress]100
  [RF][Progress]100
  +
  32U4 <- esp8266 通过Web界面下发指令
  通过CC1101 发射固定码信号
  [RF][Transmit]315,1,B710
  通过CC1101 发射灯光棒信号
  [RF][Transmit]LigthBar,freq:315,data:2600
  爆破固定码
  [RF][Attack]Class2,freq:315,start:B701,end:B710,func:1
  通过CC1101 发射FSK调制胎压信号
  [RF][TPMS]2093ef85A0203300
  通过CC1101 进行干扰
  [RF][Jam]class:Smart,freq1:433920000,freq2:315000000
  [RF][Jam]class:Rude,freq:433920000
  [RF][Jam]Stop
  关闭/开启CC1101,nRF24l01供电
  [RF][Switch]action:off/on
  //设置射频芯片信息
  [RF][Setup]chip:CC1101,class:fixed,freq:315000000
  [RF][Setup]chip:CC1101,class:keeloq,freq:433920000
  [RF][Setup]chip:nRF24L01,class:keeloq,freq:315000000
  模拟低频卡功能
  [NFC][simulation]vd:123,data:4294967295
  [NFC][attack]vd:123,start:4294967205,end:4294967295
  关闭/开启EM4095供电,低频工作芯片
  [NFC][switch]action:off/on
  Badusb 功能
  [HID]test
*/

void SerialCommunication() {
  while (Cube_PRINT.available() > 0) {
    Cube_PRINT_data += char(Cube_PRINT.read());
    delay(4);
  }
  if (Cube_PRINT_data.length() > 0) {
    if (Cube_PRINT_data.substring(1, 4) == "HID") { //h
      hid_attack();
    } else if (Cube_PRINT_data.substring(1, 3) == "RF") {

      if (int(Cube_PRINT_data.indexOf("Jam")) > 0) {
        String Jam_Class = Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("class") + 6, Cube_PRINT_data.indexOf(",freq"));
        if (Jam_Class == "Smart") {
          freq1 = strtoul(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("freq1") + 6, Cube_PRINT_data.indexOf(",freq2")).c_str(), NULL, 10);
          freq2 = strtoul(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("freq2") + 6, Cube_PRINT_data.length()).c_str(), NULL, 10);
          RF_action = 1;

          RF_Jam();
        } else if (Jam_Class == "Rude") {
          freq = strtoul(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("freq") + 5, Cube_PRINT_data.length()).c_str(), NULL, 10);
          //Serial.println(freq);
          RF_action = 2;
          RF_Jam();
        }

      } else if (int(Cube_PRINT_data.indexOf("TPMS")) > 0) {
        CC1101_TPMS_TX();
      } else if (int(Cube_PRINT_data.indexOf("Transmit")) > 0) {
        if (int(Cube_PRINT_data.indexOf("LigthBar")) > 0) {
          RF_LigthBar();
        } else {
          RF_Transmit();
        }
      } else if (int(Cube_PRINT_data.indexOf("Attack")) > 0) {
        RF_Attack();
      }else if (int(Cube_PRINT_data.indexOf("Switch")) > 0) {
        RF_Switch(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("action") + 7, Cube_PRINT_data.indexOf('\r')));
      }else if (int(Cube_PRINT_data.indexOf("Setup")) > 0){
        if (int(Cube_PRINT_data.indexOf("CC1101")) > 0){
          cc1101_Setup();
        }else if (int(Cube_PRINT_data.indexOf("nRF24L01")) > 0){
          
        }
      }
    } else if (Cube_PRINT_data.substring(1, 4) == "NFC") {
      if (Cube_PRINT_data.substring(6, 16) == "simulation") {
        NFC_action = 1;
        NFC_VD = atoi(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("vd") + 3, Cube_PRINT_data.indexOf(",data")).c_str());
        NFC_ID = strtoul(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("data") + 5, Cube_PRINT_data.length()).c_str(), NULL, 10);
        Cube_PRINT.print("VD:");
        Cube_PRINT.print(NFC_VD);
        Cube_PRINT.print("ID:");
        Cube_PRINT.println(NFC_ID);
      } else if (Cube_PRINT_data.substring(6, 12) == "attack") {
        NFC_action = 2;
        NFC_VD = atoi(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("vd") + 3, Cube_PRINT_data.indexOf(",start")).c_str());
        NFC_START = strtoul(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("start") + 6, Cube_PRINT_data.indexOf(",end")).c_str(), NULL, 10);
        NFC_END = strtoul(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("end") + 4, Cube_PRINT_data.length()).c_str(), NULL, 10);
        NFC_ID = NFC_START;
        Cube_PRINT.print("VD:");
        Cube_PRINT.print(NFC_VD);
        Cube_PRINT.print(";Start ID:");
        Cube_PRINT.print(NFC_START);
        Cube_PRINT.print(";END ID:");
        Cube_PRINT.println(NFC_END);
      } else if (Cube_PRINT_data.substring(6, 12) == "switch") {
        NFC_Switch(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("action") + 7, Cube_PRINT_data.indexOf('\r')));
      }
    } else {
      Cube_PRINT.print("error:");
      Cube_PRINT.println(Cube_PRINT_data[0], HEX);
    }
    Cube_PRINT_data = "";
  }
}
