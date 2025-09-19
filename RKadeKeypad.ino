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

void setup() {
  BootKeyboard.begin();
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

        switch (keypad.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            if (key == '#' && (isHeld('*') || isPressed('*'))) {
              BootKeyboard.write(KEYPAD_ENTER);
            } else if (key == '6' && (isHeld('*') || isPressed('*'))) {
              //*6 will toggle numlock mode
              BootKeyboard.write(KEY_NUM_LOCK);
            } else {
              if (!BootKeyboard.getLeds() & LED_NUM_LOCK) {
                BootKeyboard.write(KEY_NUM_LOCK);
              }
              BootKeyboard.press(KeyboardKeycode(keycode));
            }
            break;
          case HOLD:
            break;
          case RELEASED:
            BootKeyboard.release(KeyboardKeycode(keycode));
            break;
          case IDLE:
            break;
        }
      }
    }
  }
}

bool isHeld(char keyChar) {
  for (byte i = 0; i < LIST_MAX; i++) {
    if (keypad.key[i].kchar == keyChar) {
      if ((keypad.key[i].kstate == HOLD))
        return true;
    }
  }
  return false;  // Not held.
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
  processKeypad();
  delay(50);
}  // End loop
