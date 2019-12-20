char *ssid = "hackcube";
char *password = "hackcube";


//String AP_Name;
#include "stdlib_noniso.h"
#include <EEPROM.h>

const bool debug = true;

void sendHeader(int code, String type, size_t _size) {
    server.setContentLength(_size);
    server.send(code, type, "");
    Serial.print("setsize");
    Serial.println(_size);
}

#define bufSize 2500
int bufc = 0; //buffer counter;
char data_websiteBuffer[bufSize];

void startAPScan() {
    //scanMode = "scanning...";
    if (apScan.start()) {
        Serial.println("scanning");
        // apScan.sort();
        server.send(200, "text/json", "true");
        //    attack.stopAll();
        //    scanMode = "SCAN";
    }
    Serial.println("scanend");
}

void startClientScan() {
    if (server.hasArg("time") && apScan.getFirstTarget() > -1 && !clientScan.sniffing) {
        server.send(200, "text/json", "true");
        String timex = "15";
        clientScan.start(timex.toInt());
    } else server.send(200, "text/json", "ERROR: No selected Wi-Fi networks!");
}

void sendBuffer() {
    if (bufc > 0) {
        server.sendContent_P(data_websiteBuffer, bufc);
        bufc = 0;
    }
}

void sendToBuffer(String str) {
    size_t len = str.length();
    if (bufc + len > bufSize) {
        server.sendContent_P(data_websiteBuffer, bufc);
        bufc = 0;
    }
    memcpy(data_websiteBuffer + bufc, str.c_str(), len);
    bufc += len;
}

void sendAPResults() {
    String json = apScan.sendResults();
    server.send(200, "text/json", json);
}

void sendClientResults() {
    clientScan.send();
}

void selectAP() {
    if (server.hasArg("num")) {
        apScan.select(server.arg("num").toInt());
        server.send(200, "text/json", "true");
    }
}

void ResultsJSON() {
    apScan.getResultsJSON();
}


String getmac() {
    byte mac[8];
    String MACS = "HackCUBESpecial_";
    WiFi.macAddress(mac);
    for (int i = 3; i < 6; i++) {
        if (mac[i] < 0x10) {
            MACS += "0";
        }
        //itoa(0xaa, MACB, 16);
        MACS += String(mac[i], HEX);
    }
    return MACS;
}

void ConnectWif() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
    }
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void ConfigWifi() {
    IPAddress local_IP(192, 168, 5, 1);
    IPAddress gateway(192, 168, 5, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.mode(WIFI_AP);
    String AP_Name = getmac();
    Serial.print("SSID:");
    Serial.println(AP_Name);
    Serial.print("Setting soft-AP configuration ... ");
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
    Serial.print("Setting soft-AP ... ");
    Serial.println(WiFi.softAP(AP_Name.c_str(), "hackcube") ? "Ready" : "Failed!");
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
}

void connectWifi() {
    unsigned long outtime = millis();
    bool outflag = false;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED && !outflag) {
        Serial.print(".");
        if (millis() - outtime > 10000)
            outflag = true;
    }
    if (outflag) {
        ConfigWifi();
        Serial.println("out");
    } else {
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
}
