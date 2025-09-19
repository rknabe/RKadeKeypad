#include <Keypad.h>
#include <HID-Project.h>

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
char lastKeyHeld = ' ';

void setup() {
  BootKeyboard.begin();
  keypad.setHoldTime(1000);
}

void processKeypad() {
  if (keypad.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) {  // Scan the whole key list.
      if (keypad.key[i].stateChanged) {   // Only find keys that have changed state.
        char key = keypad.key[i].kchar;
        char keycode = 0;
        switch (key) {
          case '*':
            keycode = KEYPAD_MULTIPLY;
            break;
          case '#':
            keycode = KEYPAD_DOT;
            break;
          case '1' ... '9':
            keycode = KEYPAD_1 + (key - 49);
            break;
          case '0':
            keycode = KEYPAD_0;
            break;
        }

        switch (keypad.key[i].kstate) {
          case PRESSED:
            if (!BootKeyboard.getLeds() & LED_NUM_LOCK) {
              BootKeyboard.write(KEY_NUM_LOCK);
            }
            break;
          case HOLD:
            if (key == '#') {
              lastKeyHeld = '#';
              BootKeyboard.write(KEYPAD_ENTER);
            } else if (key == '*') {
              lastKeyHeld = '*';
              BootKeyboard.write(KEYPAD_DIVIDE);
            } else if (key == '6') {
              lastKeyHeld = '6';
              BootKeyboard.write(KEY_NUM_LOCK);
            }
            break;
          case RELEASED:
            if (key != lastKeyHeld) {
              BootKeyboard.write(KeyboardKeycode(keycode));
            }
            lastKeyHeld = ' ';
            break;
          case IDLE:
            break;
        }
      }
    }
  }
}

void loop() {
  processKeypad();
  delay(50);
}  // End loop
