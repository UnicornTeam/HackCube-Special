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

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include "APScan.h" //https://github.com/samdenty99/Wi-PWN
#include <Hash.h>
#include <FS.h>
#include "ClientScan.h"
#include "NameList.h"

MDNSResponder mdns;
APScan apScan;
ClientScan clientScan;
NameList nameList;

#define DEBUG;
ESP8266WebServer server(80);

#include "LED.h"
#include "JSON.h"
#include "WEB.h"
#include "SerialCommunication.h"
#include "WIFI.h"

#include "webfiles.h" //https://github.com/spacehuhn/esp8266_deauther/tree/master/utils/web_converter
#include "WebInterface.h"


void sniffer(uint8_t *buf, uint16_t len) {
  clientScan.packetSniffer(buf, len);
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::White);
  LED_STATE(LED_CONNECT);
  ConfigWifi(); //建立"HackCUBESpecial_XXXXXX"热点

 
  delay(100);
  EEPROM.begin(4096);
  EEPROM.write(1094, 15);
  delay(100);
  EEPROM.commit();
  
  wifi_set_promiscuous_rx_cb(sniffer);
  //ConnectWif();
  delay(100);
  LED_STATE(LED_RUN);
  SPIFFS.begin();
  Serial.swap(); //将串口切换到和ATmega32u4通信串口中
  delay(100);
  WebFileSetup(); //量产使用压缩后的网页,如需使用data目录中网页,请设置WebFFS变量,并使用SPIFFS上传数据
  WebInterface(); //初始化网页端接口
  server.begin(); 
  delay(100);
  led_action = 1;
  

}


void loop() {
  SerialCmd();
  LowBatteryCheck();
  LED_SHOW();
  
  if (clientScan.sniffing) {
      if (clientScan.stop()){
        ConfigWifi();
      }
  }
  server.handleClient();
}
