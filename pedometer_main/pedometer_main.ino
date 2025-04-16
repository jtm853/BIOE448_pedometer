// BIOE 448 Final Project: Goonometer
// Created by Jonathan Makhoul and Alice Tian
// Description: a compact proof of concept device that tracks user step count and uses this to estimate 
// parameters like calories burned and distance traveled
// Last modified: 4/14/25

#include <Wire.h> // Necessary for I2C communication
#include <LiquidCrystal.h> // loaded LCD library
#include <ArduinoBLE.h> // loaded BLE protocol library

// initiating Bluetooth services and characteristics

BLEService newService("180A");
BLEByteCharacteristic read_StepCount("2A57", BLERead);
BLEByteCharacteristic read_CalBurned("2A58", BLERead);
BLEByteCharacteristic read_Distance("2A59", BLERead);
long previousMillis = 0;

// initiate LCD pins

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// initialize variables

int accel = 0x53; // I2C address for this sensor (from data sheet)
float x, y, z;
float XYZ;
bool any_peak_detected = false;
int threshold = 450;
int step_count = 0;
float caloriesBurned = 0;
float distanceTraveled = 0;
bool central_connected = false;

// initialize button variables for scrolling LCD

const int buttonPin = 1;
int buttonState = 0;

void setup() {
  
  Serial.begin(9600);
  Wire.begin(); // Initialize serial communications
  Wire.beginTransmission(accel); // Start communicating with the device
  Wire.write(0x2D); // Enable measurement
  Wire.write(8); // Get sample measurement
  Wire.endTransmission();

  // setting up LCD:

  lcd.begin(16,2); // Initiate LCD in 16x2 configuration
  lcd.print("Step count:");

  // setting up button for scrolling LCD

  pinMode(buttonPin, INPUT);

  // setting up Bluetooth:

  while(!Serial);
    if (!BLE.begin()) {
      Serial.println("waiting for ArduinoBLE");
      while(1);
    }

  BLE.setLocalName("Juicer Pedometer");
  BLE.setAdvertisedService(newService);
  newService.addCharacteristic(read_StepCount);
  newService.addCharacteristic(read_CalBurned);
  newService.addCharacteristic(read_Distance);
  BLE.addService(newService);

  read_StepCount.writeValue(0);
  read_CalBurned.writeValue(0);
  read_Distance.writeValue(0);

  BLE.advertise();
  Serial.println("Bluetooth device active");

}

void loop() {
  
  BLEDevice central = BLE.central(); // wait for a BLE central (your iPhone)

  if (central) {

    if (!central_connected) {
    Serial.print("Connected to central: ");
    Serial.println(central.address()); // print the central's BT address
    digitalWrite(LED_BUILTIN, HIGH); // turn on LED to indicate connection is good
    central_connected = true;

    }
  
  }

  Wire.beginTransmission(accel);
  Wire.write(0x32); // Prepare to get readings for sensor (address from data sheet)
  Wire.endTransmission(false);
  Wire.requestFrom(accel, 6, true); // Get readings (2 readings per direction x 3 directions = 6 values)
  x = (int16_t)(Wire.read() | Wire.read() << 8); // Parse x values
  y = (int16_t)(Wire.read() | Wire.read() << 8); // Parse y values
  z = (int16_t)(Wire.read() | Wire.read() << 8); // Parse z values

  XYZ = sqrt(pow(x,2) + pow(y,2) + pow(z,2));

  // Serial.println(XYZ);

  if (XYZ > threshold && any_peak_detected == false) {

    any_peak_detected = true;
    step_count++;

  }

  if (XYZ < threshold && any_peak_detected == true) {

    any_peak_detected = false;

  }

  // Calculate and report values:

  caloriesBurned = step_count * 0.04; // in kcal
  distanceTraveled = step_count * 0.71; // in meters

  delay(100);

  lcd.setCursor(12,0);
  lcd.print((step_count / 2));
  read_StepCount.writeValue((step_count / 2));
  read_CalBurned.writeValue(caloriesBurned);
  read_Distance.writeValue(distanceTraveled);
  lcd.setCursor(0,1);

  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {

    lcd.print("Cal burned:");
    lcd.setCursor(12,1);
    lcd.print(caloriesBurned);

  } else {

    lcd.print("Distance:  ");
    lcd.setCursor(12,1);
    lcd.print(distanceTraveled);

  }
}