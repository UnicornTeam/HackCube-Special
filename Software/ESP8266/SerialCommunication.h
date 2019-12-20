String Serial_data;

void SerialCmd() {
    while (Serial.available() > 0) {
        Serial_data += char(Serial.read());
        delay(4);
    }
    if (Serial_data.length() > 0) {
        if (Serial_data.substring(1, 3) == "RF") {
            if (int(Serial_data.indexOf("Jam")) > 0) {
                rf_jam = Serial_data.substring(9, Serial_data.indexOf('\n') - 1);
            } else if (int(Serial_data.indexOf("Sniffer")) > 0) {
                if (int(Serial_data.indexOf("rf24l01")) > 0) {
                }
                if (int(Serial_data.indexOf("keeloq")) > 0) {

                } else {
                    AddRfList(Serial_data);
                }
                ajaxData_rf = Serial_data.substring(Serial_data.indexOf("data") + 5, Serial_data.indexOf('\n') - 1);
                LED_STATE(LED_DATA, true);
            } else if (int(Serial_data.indexOf("Progress")) > 0) {
                rf_prog = Serial_data.substring(14, Serial_data.length()).toInt();
                if (rf_prog == "100")
                    LED_STATE(LED_RUN);
            }
        } else if (Serial_data.substring(1, 4) == "NFC") { //NFC
            if (int(Serial_data.indexOf("progress")) > 0) {
                nfc_prog = Serial_data.substring(15, Serial_data.length()).toInt();
                if (nfc_prog.toInt() > 100)
                    nfc_prog = "100";
                if (nfc_prog == "100")
                    LED_STATE(LED_RUN);
            } else if (int(Serial_data.indexOf("read")) > 0) {
                Add_NFC(Serial_data);
                ajaxData_nfc = Serial_data.substring(Serial_data.indexOf("data") + 5, Serial_data.length());
                LED_STATE(LED_DATA, true);
            }
        } else if (Serial_data[0] == 0x63) {
            ajaxData_rf_24l01 = Serial_data.substring(Serial_data.indexOf("data") + 5, Serial_data.length());
            LED_STATE(LED_DATA, true);
        } else {
            //Serial.print("error:");
            //Serial.println(Serial_data[0], HEX);
        }
        Serial_data = "";
    }
}

