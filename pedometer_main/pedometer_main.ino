#include <Wire.h> // Necessary for I2C communication
#include <LiquidCrystal.h> // loaded LCD library
#include <ArduinoBLE.h> // loaded BLE protocol library

// initiating Bluetooth services and characteristics

BLEService newService("180A");
BLEByteCharacteristic readChar("2A57", BLERead);
BLEByteCharacteristic writeChar("2A58", BLEWrite);
long previousMillis = 0;

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int accel = 0x53; // I2C address for this sensor (from data sheet)
float x, y, z;
float XYZ;

bool any_peak_detected = false;
bool first_peak_detected = false;

int upper_threshold = 90000;
int lower_threshold = 67000;
int step_count = 0;

void setup() {
  
  Serial.begin(9600);
  Wire.begin(); // Initialize serial communications
  Wire.beginTransmission(accel); // Start communicating with the device
  Wire.write(0x2D); // Enable measurement
  Wire.write(8); // Get sample measurement
  Wire.endTransmission();

  // setting up LCD:

  lcd.begin(16,2); // Initiate LCD in 16x2 configuration
  lcd.print("Step count: ");

  // setting up Bluetooth:

  while(!Serial);
    if (!BLE.begin()) {
      Serial.println("waiting for ArduinoBLE");
      while(1);
    }

  BLE.setLocalName("Juicer Pedometer");
  BLE.setAdvertisedService(newService);
  newService.addCharacteristic(readChar);
  newService.addCharacteristic(writeChar);
  BLE.addService(newService);

  readChar.writeValue(0);
  writeChar.writeValue(0);

  BLE.advertise();
  Serial.println("Bluetooth device active");


  if (central) {

  Serial.print("Connected to central : ");
  Serial.println(central.address()); // print the central's BT address
  digitalWrite(LED_BUILTIN, HIGH); // turn on LED to indicate connection is good

  BLEDevice central = BLE.central(); // wait for a BLE central (your iPhone)


}

void loop() {

  Wire.beginTransmission(accel);
  Wire.write(0x32); // Prepare to get readings for sensor (address from data sheet)
  Wire.endTransmission(false);
  Wire.requestFrom(accel, 6, true); // Get readings (2 readings per direction x 3 directions = 6 values)
  x = (Wire.read() | Wire.read() << 8); // Parse x values
  y = (Wire.read() | Wire.read() << 8); // Parse y values
  y = (Wire.read() | Wire.read() << 8); // Parse z values

  XYZ = sqrt(pow(x,2) + pow(y,2) + pow(z,2));
  // Serial.println(XYZ);

  if (XYZ > upper_threshold && any_peak_detected == false) {

    any_peak_detected = true;

    if (first_peak_detected == false) {

      first_peak_detected = true;

    } else {

      first_peak_detected = false;
      step_count++;
      Serial.println(step_count);
      lcd.setCursor(0,1);
      lcd.print(step_count);
    }
  }

  if (XYZ < lower_threshold) {

    any_peak_detected = false;

  }

  if (central.connected()) { // if central is connected
      readChar.writeValue(step_count);
  }

  delay(500);
}

}