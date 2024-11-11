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
Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  //Serial.begin(9600);
  Keyboard.begin();
  customKeypad.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  customKeypad.tick();

  while (customKeypad.available()) {
    keypadEvent e = customKeypad.read();
    if ((e.bit.KEY >= 48 && e.bit.KEY <= 57) || e.bit.KEY == 35 || e.bit.KEY == 42) {
      if (e.bit.EVENT == KEY_JUST_PRESSED) {
        Keyboard.write(e.bit.KEY);
        //} else if (e.bit.EVENT == KEY_JUST_RELEASED) {
        //  Keyboard.release(e.bit.KEY);
        //}
      }
    }
  }

  delay(20);
}