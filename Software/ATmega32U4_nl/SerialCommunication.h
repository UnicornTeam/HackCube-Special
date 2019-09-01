/*
Communiceer met de ESP8266 via de seriële poort om de gegevens naar de webachtergrond weer te geven.
De achtergrondtransmissie en aanvalshandelingen worden via de seriële poort naar de 32U4 verzonden voor uitvoering.
   32U4 -> esp8266 voert gesnuffelde gegevens terug naar de webachtergrond
   Vaste code / rolling code gegevens gesnoven door CC1101 / nRF24L01
  [RF][Sniffer]freq:315,protocol:Fixed-1,modulation:ask,func:1,data:B710
  [RF][Sniffer]freq:433,protocol:keeloq,modulation:ask,data:a9df5d4c,SerialNumber:39182da
  [RF][Sniffer]freq:2.4G,protocol:rf24l01,modulation:gfsk,data:401FFFFFF02AE9
  [RF][Sniffer]freq:433,protocol:keeloq,modulation: ask,data:2d6f28d4,SerialNumber:39182da
  CC1101 Signaaldetectiegegevens in intelligente interferentiemodus
  [RF][Jam]-9b 433Mhz/Jamming.../[RF][Jam]Not Jamming
  EM4095 Lees kaartnummerinformatie
  [NFC][read]vd:123,data:1432455555
  Voortgang in de aanvalsmodus
  [NFC][progress]100
  [RF][Progress]100
  +
  32U4 <- esp8266 Verzend instructies via de webinterface
  Verzend een vast codesignaal via CC1101
  [RF][Transmit]315,1,B710
  Lancering van een lichtbalksignaal via CC1101
  [RF][Transmit]LigthBar,freq:315,data:2600
  Vaste code stralen
  [RF][Attack]Class2,freq:315,start:B701,end:B710,func:1
  FSK modulatie bandenspanningssignaal verzonden door CC1101
  [RF][TPMS]2093ef85A0203300
  Interferentie door CC1101
  [RF][Jam]class:Smart,freq1:433920000,freq2:315000000
  [RF][Jam]class:Rude,freq:433920000
  [RF][Jam]Stop
  Schakel CC1101, nRF24l01 voeding uit / in
  [RF][Switch]action:off/on
  //Stel de RF-chipinformatie in
  [RF][Setup]chip:CC1101,class:fixed,freq:315000000
  [RF][Setup]chip:CC1101,class:keeloq,freq:433920000
  [RF][Setup]chip:nRF24L01,class:keeloq,freq:315000000
  Analoge laagfrequente kaartfunctie
  [NFC][simulation]vd:123,data:4294967295
  [NFC][attack]vd:123,start:4294967205,end:4294967295
  Schakel de EM4095-voeding in, werkende laagfrequentiechip
  [NFC][switch]action:off/on
  Laagfrequente kaartschrijffunctie
  [NFC][write]vd:22,data:33333
  Badusb functie
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
      } else if (int(Cube_PRINT_data.indexOf("Switch")) > 0) {
        RF_Switch(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("action") + 7, Cube_PRINT_data.indexOf('\r')));
      } else if (int(Cube_PRINT_data.indexOf("Setup")) > 0) {
        if (int(Cube_PRINT_data.indexOf("CC1101")) > 0) {
          cc1101_Setup();
        } else if (int(Cube_PRINT_data.indexOf("nRF24L01")) > 0) {

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
      }  else if (int(Cube_PRINT_data.indexOf("write")) > 0) {
        //NFC_action = 3;
        NFC_VD = atoi(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("vd") + 3, Cube_PRINT_data.indexOf(",data")).c_str());
        NFC_ID = strtoul(Cube_PRINT_data.substring(Cube_PRINT_data.indexOf("data") + 5, Cube_PRINT_data.length()).c_str(), NULL, 10);
        Cube_PRINT.print("w,VD:");
        Cube_PRINT.print(NFC_VD);
        Cube_PRINT.print("ID:");
        Cube_PRINT.println(NFC_ID);
        EM4095.Write_ID(NFC_VD,NFC_ID);
      }
    } else {
      Cube_PRINT.print("error:");
      Cube_PRINT.println(Cube_PRINT_data[0], HEX);
    }
    Cube_PRINT_data = "";
  }
}
