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
       _                   _          _                  
      | |                 | |        | |                 
      | |__    __ _   ___ | | __ ___ | |__   _   _   ___ 
      | '_ \  / _` | / __|| |/ // __|| '_ \ | | | | / _ \
      | | | || (_| || (__ |   <| (__ | |_) || |_| ||  __/
      |_| |_| \__,_| \___||_|\_\\___||_.__/  \__,_| \___|
                                                                                      
    Author: mobier
    Email: vxxwej@gmail.com
    Git repository: https://github.com/UnicornTeam/HackCube-Special
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "APScan.h" //https://github.com/samdenty99/Wi-PWN
#include <Hash.h>
#include <FS.h>

MDNSResponder mdns;
APScan apScan;

#define DEBUG;
ESP8266WebServer server(80);

#include "LED.h"
#include "JSON.h"
#include "WEB.h"
#include "SerialCommunication.h"
#include "WIFI.h"

#include "webfiles.h" //https://github.com/spacehuhn/esp8266_deauther/tree/master/utils/web_converter
#include "WebInterface.h"

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::White);
  LED_STATE(LED_CONNECT);
  ConfigWifi(); //建立"HackCUBESpecial_XXXXXX"热点
  //ConnectWif();
  delay(100);
  LED_STATE(LED_RUN);
  SPIFFS.begin();
  Serial.swap(); //将串口切换到和ATmega32u4通信串口中
  delay(100);
  WebFileSetup(); //量产使用压缩后的网页,如需使用data目录中网页,请设置WebFFS变量,并使用SPIFFS上传数据
  WebInterface(); //初始化网页端接口
  server.begin(); 

  /*
  AddRfList("[RF][Sniffer]freq:433,protocol:Fixed-1,modulation:ask,func:0,data:41013");
  AddRfList("[RF][Sniffer]freq:433,protocol:Fixed-1,modulation:ask,func:c,data:41010");
  AddRfList("[RF][Sniffer]freq:315,protocol:Fixed-1,modulation:ask,func:1,data:B710");
  */
  /*
  AddRfList("[RF][Sniffer]freq:315,protocol:LigthBar,modulation:ask,func:red,data:2600");
  AddRfList("[RF][Sniffer]freq:315,protocol:LigthBar,modulation:ask,func:green,data:2680");
  AddRfList("[RF][Sniffer]freq:315,protocol:LigthBar,modulation:ask,func:blue,data:2640");
  AddRfList("[RF][Sniffer]freq:315,protocol:LigthBar,modulation:ask,func:effect_1,data:26f0");
  AddRfList("[RF][Sniffer]freq:315,protocol:LigthBar,modulation:ask,func:effect_2,data:2688");
  AddRfList("[RF][Sniffer]freq:315,protocol:LigthBar,modulation:ask,func:clear,data:26d8");
  */
  //[RF][Sniffer]freq:433,protocol:keeloq,modulation:ask,data:8e56d5b5,SerialNumber:39182da
  delay(100);
  led_action = 1;
  delay(100);
}


void loop() {
  SerialCmd();
  LowBatteryCheck();
  LED_SHOW();
  server.handleClient();
}
