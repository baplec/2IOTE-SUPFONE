#include <LiquidCrystal.h>
#include <Wire.h>
#include <Keypad.h>

// Keypad setup
const byte ROWS = 4; // Number of rows in the keypad
const byte COLS = 4; // Number of columns in the keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A5, A4, A3, A2}; // Row pins of the keypad
byte colPins[COLS] = {A1, A0, 6, 7}; // Column pins of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// LCD setup
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// I2C address of the router
const byte routerAddress = 0x08;
const byte DHCPAddress = 0x09;

byte slaveAddress; // Address of the slave device

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial); // Wait for Serial Monitor to open

  lcd.begin(16, 2); // Set up the number of columns and rows on the LCD
  lcd.print("I2C Messenger"); // Print a message to the LCD

  // Get the address of the slave device
  Wire.requestFrom(DHCPAddress, 1);
  if (Wire.available()) {
    slaveAddress = Wire.read();
    lcd.setCursor(0, 1);
    lcd.print("device: 0x");
    if (slaveAddress < 16) {
      lcd.print("0");
    }
    lcd.print(slaveAddress, HEX);
  }
}

void loop() {
  char key = keypad.getKey(); // Get the pressed key from the keypad

  if (key != NO_KEY) {
    // Input the message to be sent
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Message:");
    lcd.setCursor(0, 1);
    String message = "";
    while (key != '#') {
      key = keypad.getKey();
      if (key != NO_KEY && key != '#') {
        message += key;
        lcd.print(key);
      }
      delay(100);
    }

    // Send the message to the router
    Wire.beginTransmission(routerAddress);
    Wire.write(slaveAddress); // Send the address of the slave device
    Wire.write(message.c_str()); // Send the message
    Wire.endTransmission();

    // Print the sent message on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sent to:");
    lcd.setCursor(0, 1);
    lcd.print("Addr: 0x");
    if (slaveAddress < 16) {
      lcd.print("0");
    }
    lcd.print(slaveAddress, HEX);
    lcd.setCursor(0, 2);
    lcd.print("Msg: " + message);

    delay(2000);
  }
}
