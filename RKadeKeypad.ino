#include <Keypad.h>
#include "Keyboard.h"

const byte KEYPAD_ROWS = 4;  //four rows
const byte KEYPAD_COLS = 3;  //three columns
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[KEYPAD_ROWS] = { 1, 6, 5, 3 };  //connect to the row pinouts of the kpd
byte colPins[KEYPAD_COLS] = { 2, 0, 4 };     //connect to the column pinouts of the kpd
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

void setup() {
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
            keycode = KEY_KP_ASTERISK;
            break;
          case '#':
            keycode = KEY_KP_DOT;
            break;
          case '1' ... '9':
            keycode = KEY_KP_1 + (key - 49);
            break;
          case '0':
            keycode = KEY_KP_0;
            break;
        }

        switch (keypad.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            if (key == '#' && (isHeld('*') || isPressed('*'))) {
              Keyboard.write(KEY_KP_ENTER);
            } else if (key == '6' && (isHeld('*') || isPressed('*'))) {
              //*6 will toggle numlock mode
              Keyboard.write(KEY_NUM_LOCK);
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

  delay(50);
}  // End loop
