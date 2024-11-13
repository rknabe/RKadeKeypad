#include <Keypad.h>
#include "Keyboard.h"

const byte ROWS = 4;  //four rows
const byte COLS = 3;  //three columns
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 5, 4, 3, 2 };  //connect to the row pinouts of the kpd
byte colPins[COLS] = { 8, 7, 6 };     //connect to the column pinouts of the kpd
//byte rowPins[ROWS] = { 2, 3, 4, 5 };  //connect to the row pinouts of the kpd
//byte colPins[COLS] = { 8, 9, 10 };    //connect to the column pinouts of the kpd

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
}

bool isHeld(char keyChar) {
  for (byte i = 0; i < LIST_MAX; i++) {
    if (keypad.key[i].kchar == keyChar) {
      if ((keypad.key[i].kstate == HOLD))
        return true;
    }
  }
  return false;  // Not pressed.
}

bool isPressed(char keyChar) {
  for (byte i = 0; i < LIST_MAX; i++) {
    if (keypad.key[i].kchar == keyChar) {
      if ((keypad.key[i].kstate == PRESSED))
        return true;
    }
  }
  return false;  // Not pressed.
}

void loop() {

  // Fills keypad.key[ ] array with up-to 10 active keys.
  // Returns true if there are ANY active keys.
  if (keypad.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) {  // Scan the whole key list.
      if (keypad.key[i].stateChanged) {   // Only find keys that have changed state.
        char key = keypad.key[i].kchar;
        char keycode = 0;
        switch (key) {
          case '*':
            keycode = 221;
            break;
          case '#':
            keycode = 235;
            break;
          case '1':
            keycode = 225;
            break;
          case '2':
            keycode = 226;
            break;
          case '3':
            keycode = 227;
            break;
          case '4':
            keycode = 228;
            break;
          case '5':
            keycode = 229;
            break;
          case '6':
            keycode = 230;
            break;
          case '7':
            keycode = 231;
            break;
          case '8':
            keycode = 232;
            break;
          case '9':
            keycode = 233;
            break;
          case '0':
            keycode = 234;
            break;
        }

        switch (keypad.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            if (key == '#' && (isHeld('*') || isPressed('*'))) {
              keycode = 224;
              Keyboard.press(keycode);
              delay(100);
              Keyboard.releaseAll();
            } else if (key == '6' && (isHeld('*') || isPressed('*'))) {
              //*6 will toggle numlock mode
              keycode = 219;
              Keyboard.press(keycode);
              delay(100);
              Keyboard.releaseAll();
            } else {
              Keyboard.press(keycode);
            }
            break;
          case HOLD:
            break;
          case RELEASED:
            Keyboard.release(keycode);
            break;
          case IDLE:
            break;
        }
      }
    }
  }
}  // End loop
