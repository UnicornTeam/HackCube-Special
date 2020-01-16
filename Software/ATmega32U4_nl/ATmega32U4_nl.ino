/*
                 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
               @@@@@@@@@                    @@@@
             @@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@
            @@@@@   @@@@@@@@@@@@@@@@@@@@@@@   @@@@@
           @@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@
          @@@@@   @@@@@    @@@@@@@@@@@@@@@@@@   @@@@@
         @@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@
        @@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@
      @@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@
     @@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@
    @@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@
    @@@@@                       @@@@@@@@@@@@@@@@@@@@  @@@@@
     @@@@@   @@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@
      @@@@@   @@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@
       @@@@@   @@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@
         @@@@   @@@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@
          @@@@   @@@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@
           @@@@    @@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@
            @@@@@   @@@@@@@@@@@@@@@@@   @@@@@@@@@@@
             @@@@@   @@@@@@@@@@@@@@@@@   @@@@@@@@@
              @@@@@                       @@@@@@@
               @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
       _                   _                 _
      | |                 | |               | |
      | |__    __ _   ___ | | __ ___  _   _ | |__    ___
      | '_ \  / _` | / __|| |/ // __|| | | || '_ \  / _ \
      | | | || (_| || (__ |   <| (__ | |_| || |_) ||  __/
      |_| |_| \__,_| \___||_|\_\\___| \__,_||_.__/  \___|

    Author: mobier
    Email: vxxwej@gmail.com
    Git repository: https://github.com/UnicornTeam/HackCube-Special
*/

long freq_cc1101 = 315000000; //Stel de CC1101-standaard luisterfrequentie in
//long freq_cc1101 = 434000000; //Stel de CC1101-standaard luisterfrequentie in
int rf_class = 1;//Stel de RF-luistermodus in op vaste code
#define Cube_PRINT  Serial1
#define Print_Time 350
unsigned long SerialLastTime;
String Cube_PRINT_data;
#include "HackCube.h"
#include "rf.h"
#include "hid.h"
#include "nfc.h"
#include "SerialCommunication.h"
HackCube hc;
void setup() {
  Cube_PRINT.begin(9600);//Stel de communicatie pin in met ESP8266
  delay(2000);
  hc.pinSetup();//Initialisatie module pin
  delay(100);
  Cube_PRINT.println("Cubemini Runing...");
  cc1101_config();//Configureer de CC1101-module om standaard te luisteren naar de frequentie in de variabele freq_cc1101
  delay(4);
  cc1101.PrintConfig();//Uitgang CC 1101 sleutelregisters
  mySwitch.enableReceive(1);//Stel de mySwitch-onderbrekingspin in om het vaste codesignaal te ontvangen
  //rf_class=2;
  //freq_cc1101=433920000;
  //RF433_Setup_rollcode();//Rolling code CC1101 register configuratie
  //cc1101_config();
  //Hackradio.enableReceive(RF315_att);//Stel de onderbrekingspin in om het rolling codesignaal te ontvangen
  //rf24l01_SnifferSetup();//Initialiseer de nRF24L01-module en druk de registergegevens af
  //NFC_Switch("off");  

}
void loop() {

  //rf24Sniffer();//Polling nRF24L01 bufferregister om gegevens te ontvangen
  SnifferEM4100();//Decodeer kaartnummergegevens door de OUT-uitgangspen van de EM4095 te pollen
  AttackEM4100();//Simuleer/vernietig kaarten uit de EM4100-serie volgens de instructies
  SerialCommunication();//Via de seriële poort ESP8266 Communicatie interactiegegevens
  if (rf_class == 1) {
    SnifferFixedCode();//Vaste codegegevens sniffen
  } else if (rf_class == 2) {
    SnifferRollcode();//Rollende codegegevens sniffen
  }
}



