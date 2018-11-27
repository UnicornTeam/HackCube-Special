
/*
  RF json
*/
int RFnum = 0;
#define RFnummax 30

struct myRF {
  //int num;
  String func;
  long freq;
  String data;
  String protocol;
  String modulation;
};
myRF   RFlist[RFnummax];
void AddRfList(String data) {
  //ajaxData_rf = data.substring(data.indexOf("data") + 5, data.length());
  for (int i = 0; i < RFnum; i++) {
    if (RFlist[i].data == data.substring(data.indexOf("data") + 5, data.indexOf('\n') - 1) && RFlist[i].func == data.substring(data.indexOf("func:") + 5, data.indexOf(",data")))
      return;
  }
  
  RFlist[RFnum].freq = atoi(data.substring(data.indexOf("freq:") + 5, data.indexOf(",protocol")).c_str());
  //RFlist[RFnum].func = strtoul(data.substring(data.indexOf("func:") + 5, data.indexOf(";data")).c_str(), NULL, 16);
  RFlist[RFnum].func = data.substring(data.indexOf("func:") + 5, data.indexOf(",data"));
  RFlist[RFnum].protocol = data.substring(data.indexOf("protocol:") + 9, data.indexOf(",modulation"));
  RFlist[RFnum].modulation = data.substring(data.indexOf("modulation:") + 11, data.indexOf(",func"));
  RFlist[RFnum].data = data.substring(data.indexOf("data") + 5, data.indexOf('\n') - 1);
  
  //Serial.print("------");
  #ifdef DEBUG
  Serial.print( RFlist[RFnum].freq);
  Serial.print(",");
  Serial.print(RFlist[RFnum].func);
  Serial.print(",");
  Serial.print( RFlist[RFnum].protocol);
  Serial.print(",");
  Serial.print( RFlist[RFnum].modulation);
  Serial.print(",");
  Serial.print( RFlist[RFnum].data);
  Serial.print(",");
  Serial.println(RFnum);
  #endif
  RFnum++;
}

String getrflist() {
  String json;
  //  int bufc = 0; //bufferCounter
  json = "{\"rf\":[";
  //sendToBuffer(json);
  for (int i = 0; i < RFnum && i < RFnummax; i++) {
    json += "{";
    json += "\"i\":" + (String)i + ",";//freq
    json += "\"f\":" + (String)RFlist[i].freq + ",";//freq
    json += "\"p\":\"" + RFlist[i].protocol + "\",";//protocol
    json += "\"m\":\"" + RFlist[i].modulation + "\",";//modulation
    json += "\"func\":\"" + RFlist[i].func + "\",";//func
    json += "\"d\":\"" + RFlist[i].data ;//data
    json += "\"}";
    if ((i != RFnum - 1) ) json += ",";
  }
  json += "]}";
  //Serial.println(json);
  return json;
  json = "";
}


void rf_json() {
  //Access-Control-Allow-Origin
  //server.sendHeader("Connection", "close");
  //server.sendHeader("Access-Control-Allow-Origin", "*");
  // server._prepareHeader("Access-Control-Allow-Origin", 200, "text/json", 10);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  //server.sendHeader("Access-Control-Allow-Methods:OPTIONS", "*");
  server.send(200, "application/json;charset=UTF-8", getrflist());
  //server.sendHeader("Content-Length", (String)4000);
  //sendHeader(200, "text/json", 2500);
  //server.send(200, "text/html", "");
  //delay(100);
  //server.send(200, "application/json",data_websiteBuffer);
  //server.send(200, "application/json", "{\"temperat666666666666ure\": " + String(2, 1) + ", \"humidity\": " + String(2, 0) + "}");
  //apScan.sendResults();
}

/*
  NFC.JSon
*/

int NFCnum = 0;
#define NFCmax 30

struct myNFC {
  int vd;
  unsigned long data;
};
myNFC   NFClist[NFCmax];

String get_nfclist() {
  String json;
  //  int bufc = 0; //bufferCounter
  json = "{\"nfc\":[";
  //sendToBuffer(json);
  for (int i = 0; i < NFCnum && i < NFCmax; i++) {
    json += "{";
    json += "\"i\":" + (String)i + ",";//i
    json += "\"v\":" + (String)NFClist[i].vd + ",";//vd
    json += "\"d\":" + (String)NFClist[i].data ;//data
    json += "}";
    if ((i != NFCnum - 1) ) json += ",";
  }
  json += "]}";
  return json;
  json = "";
}

void Add_NFC(String data) {
  //ajaxData_rf = data.substring(data.indexOf("data") + 5, data.length());
  for (int i = 0; i < NFCnum; i++) {
    if (NFClist[i].data ==  strtoul(data.substring(data.indexOf("data") + 5, data.length()).c_str(),NULL,10) && NFClist[i].vd == atoi(data.substring(data.indexOf("vd") + 3, data.indexOf(",data")).c_str()))
      return;
  }
  NFClist[NFCnum].vd = atoi(data.substring(data.indexOf("vd") + 3, data.indexOf(",data")).c_str());
  NFClist[NFCnum].data = strtoul(data.substring(data.indexOf("data") + 5, data.length()).c_str(),NULL,10);
  //Serial.print("------");
  #ifdef DEBUG
  Serial.print( NFClist[NFCnum].vd);
  Serial.print(",");
  Serial.print(NFClist[NFCnum].data);
  Serial.print(",");
  Serial.print(data.substring(data.indexOf("data") + 5, data.length()).c_str());
  Serial.print(",");
  Serial.println(NFCnum);
  #endif
  NFCnum++;
}

void nfc_json() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json;charset=UTF-8", get_nfclist());
}


