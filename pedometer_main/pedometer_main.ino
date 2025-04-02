#include <Wire.h> // Necessary for I2C communication
#include <LiquidCrystal.h> // loaded library

LiquidCrystal lcd(10, 8, 5, 4, 3, 2);

int accel = 0x53; // I2C address for this sensor (from data sheet)
float x, y, z;
float XYZ;

bool any_peak_detected = false;
bool first_peak_detected = false;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Initialize serial communications
  Wire.beginTransmission(accel); // Start communicating with the device
  Wire.write(0x2D); // Enable measurement
  Wire.write(8); // Get sample measurement
  Wire.endTransmission();

  lcd.begin(16,2); // Initiate LCD in 16x2 configuration
  lcd.print("test juice");
  lcd.setCursor(0,1);
  lcd.print("test 2");

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
  Serial.println(XYZ);

  // Serial.print("x = "); // Print values
  // Serial.print(x);
  // lcd.print(x);
  // Serial.print(", y = ");
  // Serial.print(y);
  // lcd.setCursor(0,1);
  // lcd.print(y);
  // Serial.print(", z = ");
  // Serial.println(z);
  delay(200);
}