// Use this example with the Adafruit Keypad products.
// You'll need to know the Product ID for your keypad.
// Here's a summary:
//   * PID3844 4x4 Matrix Keypad
//   * PID3845 3x4 Matrix Keypad
//   * PID1824 3x4 Phone-style Matrix Keypad
//   * PID1332 Membrane 1x4 Keypad
//   * PID419  Membrane 3x4 Matrix Keypad

#include "Adafruit_Keypad.h"
#include "Keyboard.h"

// define your specific keypad here via PID
#define KEYPAD_PID1824
// define your pins here
// can ignore ones that don't apply
#define R1 2
#define R2 3
#define R3 4
#define R4 5
#define C1 8
#define C2 9
#define C3 10
//e#define C4    11  // not needed for 3x4
// leave this import after the above configuration
#include "keypad_config.h"

//initialize an instance of class NewKeypad
Adafruit_Keypad keypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
uint8_t lastKeyPressed = 0;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  keypad.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  keypad.tick();

  while (keypad.available()) {
    keypadEvent e = keypad.read();
    uint8_t key = 0;
    switch (e.bit.KEY) {
      case '*':
        key = 221;
        break;
      case '#':
        if (keypad.isPressed('*')) {
          Serial.println("combo");
          key =	224;
        } else {
          key = 235;
          Serial.println("not combo");
        }
        break;
      case '1':
        key = 225;
        break;
      case '2':
        key = 226;
        break;
      case '3':
        key = 227;
        break;
      case '4':
        key = 228;
        break;
      case '5':
        key = 229;
        break;
      case '6':
        key = 230;
        break;
      case '7':
        key = 231;
        break;
      case '8':
        key = 232;
        break;
      case '9':
        key = 233;
        break;
      case '0':
        key = 234;
        break;
    }

    //if ((e.bit.KEY >= 48 && e.bit.KEY <= 57) || e.bit.KEY == 35 || e.bit.KEY == 42) {
    if (key > 0) {
      if (e.bit.EVENT == KEY_JUST_PRESSED) {
        //Keyboard.write(e.bit.KEY);
        //if (key != lastKeyPressed) {
          Keyboard.press(key);          
       // }
        lastKeyPressed = key;
      } else if (e.bit.EVENT == KEY_JUST_RELEASED) {
        Keyboard.release(key);
        //lastKeyPressed = 0;
      }
    }    
  }

  delay(10);
}