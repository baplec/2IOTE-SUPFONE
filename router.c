#include <Wire.h>
#include <LiquidCrystal.h>

// I2C address of the DHCP Arduino
const byte dhcpAddress = 0x09;

// Maximum number of slaves that can connect to the router
const int maxSlaves = 10;

// Data structure to store information about connected slaves
struct SlaveInfo {
  byte address;
  bool assigned;
} slaves[maxSlaves];

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // LCD object

void setup() {
  Wire.begin(dhcpAddress); // Initialize I2C communication with the DHCP Arduino address
  Wire.onReceive(receiveEvent); // Register receiveEvent() function as I2C event handler
  Serial.begin(9600);
  lcd.begin(16, 2); // Set up the number of columns and rows on the LCD.
  lcd.print("DHCP Server"); // Print a message to the LCD.
  lcd.setCursor(0, 1); // Set the cursor to column 0, line 2
  lcd.print("is ready...");

  // Initialize the slave info data structure
  for (int i = 0; i < maxSlaves; i++) {
    slaves[i].address = 0;
    slaves[i].assigned = false;
  }
}

void loop() {
  // Do any other tasks if needed
}

void receiveEvent(int bytes) {
  while (Wire.available()) {
    byte address = Wire.read(); // Read the address sent by the slave
    String message = "";
    while (Wire.available()) {
      char c = Wire.read(); // Read each character of the message
      message += c;
    }

    // Check if the received address is a new slave requesting an address
    if (address == 0x00 && message.equals("REQUEST")) {
      byte assignedAddress = assignAddress(); // Assign an address to the new slave
      if (assignedAddress != 0) {
        // If an address is assigned, send it back to the slave as an acknowledgment
        Serial.println("Assigned address " + String(assignedAddress));
        Wire.beginTransmission(assignedAddress); // Start I2C communication with the new slave
        Wire.write("ACK"); // Send the acknowledgment to the new slave
        Wire.endTransmission(); // End I2C communication with the new slave

        // Display the assigned address on the LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Assigned:");
        lcd.setCursor(0, 1);
        lcd.print("0x");
        lcd.print(assignedAddress, HEX);
      } else {
        // If no address is available, send an error message to the new slave
        Serial.println("No available address");
        Wire.beginTransmission(0x00); // Start I2C communication with the requesting slave
        Wire.write("ERROR"); // Send an error message to the requesting slave
        Wire.endTransmission(); // End I2C communication with the requesting slave

        // Display an error message on the LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("No address");
        lcd.setCursor(0, 1);
        lcd.print("available");
      }
    }
  }
}

byte assignAddress() {
  // Function to assign an available address to a new slave
  for (int i = 1; i <= maxSlaves; i++) {
    bool addressTaken = false;
    for (int j = 0; j < maxSlaves; j++) {
      if (slaves[j].address == i) {
        addressTaken = true;
        break;
      }
    }
    if (!addressTaken) {
      // If the address is not taken, mark it as assigned and return it
      slaves[i - 1].address = i;
      slaves[i - 1].assigned = true;
      return i;
    }
  }
  // If no available address is found, return 0
  return 0;
}
