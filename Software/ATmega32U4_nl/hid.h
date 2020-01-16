/*
 *  Analoog toetsenbordapparaat
 */
#include "Keyboard.h"

void hid_attack() {
  //if (Cube_PRINT_data[1] == 0x61) {
  if (!Cube_PRINT_data.indexOf("[HID]lock")) {
    Cube_PRINT.println("lock");
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('l');
    delay(1000);
    Keyboard.releaseAll();
    Keyboard.write(KEY_RETURN);
  } else if (!Cube_PRINT_data.indexOf("[HID]cmatrix")) {
    Keyboard.press(KEY_RIGHT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('t');
    delay(200);
    Keyboard.releaseAll();
    delay(1500);
    Keyboard.press(KEY_F11);
    delay(300);
    Keyboard.releaseAll();
    delay(300);
    Keyboard.print("cmatrix");
    delay(100);
    Keyboard.write(KEY_RETURN);
  } else {
    Cube_PRINT_data[0] = "";
    Keyboard.print(Cube_PRINT_data.substring(5, Cube_PRINT_data.length()));
  }
  Cube_PRINT.println("hid");
}
