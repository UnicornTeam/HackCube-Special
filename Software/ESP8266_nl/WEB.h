


String ajaxData_nfc;
String getajax_nfc() {
  return ajaxData_nfc;
}

String ajaxData_rf;
String getajax_rf() {
  return ajaxData_rf;
}


String ajaxData_rf_24l01;
String getajax_rf_24l01() {
  return ajaxData_rf_24l01;
}

String rf_prog;
String getrf_prog() {
  return rf_prog;
}

String rf_jam;
String getrf_jam() {
  return rf_jam;
}

String nfc_prog;
String getnfc_prog() {
  return nfc_prog;
}

void rf_Replay(int num) {
  if (RFlist[num].protocol == "LigthBar") {
    Serial.print("[RF][Transmit]LigthBar,freq:");
    Serial.print(RFlist[num].freq);
    Serial.print(",data:");
    Serial.println(RFlist[num].data);
  } else if (RFlist[num].protocol == "Fixed-1") {
    Serial.print("[RF][Transmit]");
    Serial.print(RFlist[num].freq);
    Serial.print(",");
    Serial.print(RFlist[num].func);
    Serial.print(",");
    Serial.println(RFlist[num].data);
  }
}




