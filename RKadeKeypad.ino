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
bool numPadMode = true;
#define SERIAL_BAUDRATE 9600
#define VERSION "1.0.1"
#define HOLD_TIME 1100

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  Serial.setTimeout(100);

  BootKeyboard.begin();
  keypad.setHoldTime(HOLD_TIME);
  numPadMode = true;

  //this help firmware update
  delay(500);
}

void processKeypad() {
  if (keypad.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) {  // Scan the whole key list.
      if (keypad.key[i].stateChanged) {   // Only find keys that have changed state.
        char key = keypad.key[i].kchar;
        char keycode = 0;
        switch (key) {
          case '*':
            if (numPadMode) {
              keycode = KEYPAD_MULTIPLY;
            } else {
              keycode = KEY_N;
            }
            break;
          case '#':
            if (numPadMode) {
              keycode = KEYPAD_DOT;
            } else {
              keycode = KEY_ENTER;
            }
            break;
          case '1' ... '9':
            if (numPadMode) {
              keycode = KEYPAD_1 + (key - 49);
            } else {
              keycode = KEY_1 + (key - 49);
            }
            break;
          case '0':
            if (numPadMode) {
              keycode = KEYPAD_0;
            } else {
              keycode = KEY_0;
            }
            break;
        }

        switch (keypad.key[i].kstate) {
          case PRESSED:
            if (!BootKeyboard.getLeds() & LED_NUM_LOCK) {
              BootKeyboard.write(KEY_NUM_LOCK);
            }
            break;
          case HOLD:
            switch (key) {
              case '#':
                send(KEYPAD_ENTER);
                break;
              case '*':
                send(KEYPAD_DIVIDE);
                break;
              case '6':
                send(KEY_NUM_LOCK);
                break;
              case '0':
                numPadMode = !numPadMode;
                break;
              case '1':
                numPadMode = true;
                break;
              case '2':
                numPadMode = false;
                break;
              case '3':
                if (key != lastKeyHeld) {
                  lastKeyHeld = key;
                  BootKeyboard.press(KeyboardKeycode(keycode));
                  delay(575);
                  BootKeyboard.releaseAll();
                }
                break;
            }
            lastKeyHeld = key;
            break;
          case RELEASED:
            if (key != lastKeyHeld) {
              send(keycode);
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

void send(char keycode) {
  BootKeyboard.press(KeyboardKeycode(keycode));
  delay(40);
  BootKeyboard.release(KeyboardKeycode(keycode));
}

void processSerial() {
  if (Serial.available()) {
    char cmd[16];
    uint8_t cmdLength;
    int32_t arg1 = -32768;

    cmdLength = Serial.readBytesUntil(' ', cmd, 15);
    cmd[cmdLength] = 0;

    if (Serial.available())
      arg1 = Serial.parseInt(SKIP_WHITESPACE);

    if (strcmp_P(cmd, PSTR("mode")) == 0) {
      if (arg1 == 1) {
        numPadMode = true;
      } else if (arg1 == 2) {
        numPadMode = false;
      }
      Serial.print("mode:");
      Serial.println(numPadMode ? 1 : 2);
    } else if (strcmp_P(cmd, PSTR("version")) == 0) {
      Serial.print("rkade keypad version:");
      Serial.println(VERSION);
    }
  }
}

void loop() {
  processKeypad();
  processSerial();
  delay(50);
}  // End loop
