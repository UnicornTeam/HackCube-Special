#include "APScan.h"

APScan::APScan() {

}

bool APScan::start() {
  if (debug) {
    Serial.println("starting AP scan...");
    Serial.println("MAC - Ch - RSSI - Encrypt. - SSID - Hidden");// - Vendor");
  }
  aps._clear();
  results = 0;
  for (int i = 0; i < maxAPScanResults; i++){
    selected[i] = false;
    String("").toCharArray(names[i], 33);
  }
  results = WiFi.scanNetworks(false, true); // lets scanNetworks return hidden APs. (async = false & show_hidden = true)
  if(results > maxAPScanResults) results = maxAPScanResults;

  if (debug) Serial.println("Scan results: "+(String)results);
  
  for (int i = 0; i < results && i < maxAPScanResults; i++) {
    Mac _ap;
    _ap.set(WiFi.BSSID(i)[0], WiFi.BSSID(i)[1], WiFi.BSSID(i)[2], WiFi.BSSID(i)[3], WiFi.BSSID(i)[4], WiFi.BSSID(i)[5]);
    aps.add(_ap);
    channels[i] = WiFi.channel(i);
    rssi[i] = WiFi.RSSI(i);
    encryption[i] = WiFi.encryptionType(i);
    hidden[i] = WiFi.isHidden(i);
    String _ssid = WiFi.SSID(i);
    _ssid.toCharArray(names[i], 33);
    //data_getVendor(WiFi.BSSID(i)[0],WiFi.BSSID(i)[1],WiFi.BSSID(i)[2]).toCharArray(vendors[i],9);
    if (debug) {
      Serial.print((String)i);
      Serial.print(" - ");
      _ap._print();
      Serial.print(" - ");
      Serial.print(channels[i]);
      Serial.print(" - ");
      Serial.print(rssi[i]);
      Serial.print(" - ");
      Serial.print(getEncryption(encryption[i]));
      Serial.print(" - ");
      Serial.print(names[i]);
      Serial.print(" - ");
      Serial.print(hidden[i]);
      //Serial.print(" - ");
      //Serial.print(vendors[i]);
      Serial.println();
    }
  }

  //for debugging the APScan crash bug
  /*
    if(debug){
    for(int i=results;i<maxAPScanResults;i++){
      Mac _ap;
      _ap.set(random(255),random(255),random(255),random(255),random(255),random(255));
      aps.add(_ap);
      channels[i] = random(1,12);
      rssi[i] = random(-30,-90);
      encryption[i] = ENC_TYPE_NONE;
      String _ssid = "test_dbeJwq3tPtJsuWtgULgShD9dxXV";
      _ssid.toCharArray(names[i],33);

      Serial.print((String)i);
      Serial.print(" - ");
      _ap._print();
      Serial.print(" - ");
      Serial.print(channels[i]);
      Serial.print(" - ");
      Serial.print(rssi[i]);
      Serial.print(" - ");
      Serial.print(getEncryption(encryption[i]));
      Serial.print(" - ");
      Serial.print(names[i]);
      Serial.println();

      results++;
    }
    }
  */

  if (debug) Serial.println("scan done");
  return true;
}

String APScan::getEncryption(int code) {
  switch (code) {
    case ENC_TYPE_NONE:
      return "Open";
      break;
    case ENC_TYPE_WEP:
      return "WEP";
      break;
    case ENC_TYPE_TKIP:
      return "WPA";
      break;
    case ENC_TYPE_CCMP:
      return "WPA2";
      break;
    case ENC_TYPE_AUTO:
      return "WPA*";
      break;
  }
  return "?";
}

String APScan::getAPName(int num) {
  //if (isHidden(num)) return "* Hidden SSID *";
  return names[num];
}
String APScan::getAPEncryption(int num) {
  return getEncryption(encryption[num]);
}
//String APScan::getAPVendor(int num){ return vendors[num]; }
String APScan::getAPMac(int num) {
  return aps._get(num).toString();
}
bool APScan::isHidden(int num) {
  return hidden[num];
}
int APScan::getAPRSSI(int num) {
  return rssi[num];
}
int APScan::getAPChannel(int num) {
  return channels[num];
}

int APScan::getFirstTarget() {
  for (int i = 0; i < maxAPScanResults; i++) {
    if (isSelected(i)) return i;
  }
  return -1;
}

String APScan::sanitizeJson(String input){
 input.replace("\\","\\\\");
 input.replace("\"","\\\"");
 input.replace("/","\\/");
 input.replace("\b","\\b");
 input.replace("\f","\\f");
 input.replace("\n","\\n");
 input.replace("\r","\\r");
 input.replace("\t","\\t");
 return input;
}

void APScan::sendResults() {
//  if (debug) Serial.print(millis());
  if (debug) Serial.print("sending AP scan result JSON ");
//  delay(1000);
/*
  size_t _size = 25; // {"aps":[ ... ],"multiAPs":"1"}
  for (int i = 0; i < results && i < maxAPScanResults; i++) {
    _size += 67;
    _size += String(i).length();
    _size += String(getAPChannel(i)).length();
    _size += sanitizeJson(getAPName(i)).length();
    _size += String(getAPRSSI(i)).length();
    if ((i != results - 1) && (i != maxAPScanResults - 1)) _size++; // ,
  }

  sendHeader(200, "text/json", _size);*/

  String json;
  int bufc = 0; //bufferCounter
  json = "{\"aps\":[";

  sendToBuffer(json);

  for (int i = 0; i < results && i < maxAPScanResults; i++) {
    if (debug) Serial.print(".");
    json = "{";
    json += "\"i\":" + (String)i + ",";
    json += "\"c\":" + (String)getAPChannel(i) + ",";
    json += "\"m\":\"" + getAPMac(i) + "\",";
    json += "\"ss\":\"" + sanitizeJson(getAPName(i)) + "\",";
    json += "\"r\":" + (String)getAPRSSI(i) + ",";
    json += "\"e\":" + (String)encryption[i] + ",";
    //json += "\"v\":\""+getAPVendor(i)+"\",";
    json += "\"h\":" + (String)hidden[i] + ",";
    json += "\"se\":" + (String)isSelected(i);
    json += "}";
    if ((i != results - 1) && (i != maxAPScanResults - 1)) json += ",";

    sendToBuffer(json);
  }

  json = "],\"multiAPs\":\"";
  //if(settings.multiAPs) json += "1";
  //else json += "0";
  json += "0";
  json += "\"}";


  sendToBuffer(json);
  //delay(100);
  sendBuffer();

  if (debug) Serial.println("done");
}


String APScan::getResultsJSON() {
  if (debug) Serial.print("getting AP scan result JSON ");
  String json = "{ \"aps\":[ ";
  for (int i = 0; i < results && i < maxAPScanResults; i++) {
    if (debug) Serial.print(".");
    json += "{";
    json += "\"i\":" + (String)i + ",";
    json += "\"c\":" + (String)getAPChannel(i) + ",";
    json += "\"m\":\"" + getAPMac(i) + "\",";
    json += "\"ss\":\"" + sanitizeJson(getAPName(i)) + "\",";
    json += "\"r\":" + (String)getAPRSSI(i) + ",";
    json += "\"e\":" + (String)encryption[i] + ",";
    //json += "\"v\":\""+getAPVendor(i)+"\",";
    json += "\"h\":" + (String)hidden[i] + ",";
    json += "\"se\":" + (String)isSelected(i);
    json += "}";
    if ((i != results - 1) && (i != maxAPScanResults - 1)) json += ",";
  }
  json += "] }";
  if (debug) {
    Serial.println(json);
    Serial.println("done");
  }
  return json;
}

void APScan::sort() {
  if (debug) Serial.println("sorting APs ");

  /* I know, it's bubble sort... but it works and that's the main thing! ;) (feel free to improve it tho) */

  for (int i = 0; i < results - 1; i++) {
    Serial.println("--------------");
    for (int h = 0; h < results - i - 1; h++) {

      if (rssi[h] < rssi[h + 1]) {
        Serial.println("switched: " + (String)rssi[h] + " > " + (String)rssi[h + 1]);
        int tmpA = channels[h];
        channels[h] = channels[h + 1];
        channels[h + 1] = tmpA;

        tmpA = rssi[h];
        rssi[h] = rssi[h + 1];
        rssi[h + 1] = tmpA;

        tmpA = encryption[h];
        encryption[h] = encryption[h + 1];
        encryption[h + 1] = tmpA;

        String tmpB = names[h];
        strncpy(names[h], names[h + 1], 32);
        tmpB.toCharArray(names[h + 1], 32);

        bool tmpC = hidden[h];
        hidden[h] = hidden[h + 1];
        hidden[h + 1] = tmpC;

        tmpC = selected[h];
        selected[h] = selected[h + 1];
        selected[h + 1] = tmpC;

        Mac tmpMac = aps._get(h);
        aps.set(h,aps._get(h+1));
        aps.set(h+1,tmpMac);

      } else Serial.println((String)rssi[h] + " < " + (String)rssi[h + 1]);
    }
  }
}

void APScan::select(int num) {
  if (debug) Serial.println("select " + (String)num + " - " + !selected[num]);
  if(num < 0) {
    if(num == -1){
      /*if(settings.multiAPs) {
        selectedSum = results;
        for (int i = 0; i < results; i++) selected[i] = true;
      }*/
    } else {
      selectedSum = 0;
      for (int i = 0; i < maxAPScanResults; i++) selected[i] = false;
    }
  } /*else if(!settings.multiAPs) {
    for (int i = 0; i < maxAPScanResults; i++){
      if(i != num) selected[i] = false;
      else selected[num] = !selected[num];
    }
    selectedSum = 1;
  }*/ else {
     if(selected[num]) selectedSum--;
     else selectedSum++;
     selected[num] = !selected[num];
  }
}

bool APScan::isSelected(int num) {
  return selected[num];
}

