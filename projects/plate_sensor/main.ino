/*
  Plate Sensor - ESP32-C3
  Detects when shooting plate falls and sends RF signal
  
  Components:
  - ESP32-C3 (XIAO_ESP32C3)
  - SC7A20H accelerometer (I2C) on pins 8/9
  - RF transmitter on pin 2
  - 3.3V battery
*/

#include <Wire.h>

// ============================================================================
// CONFIGURATION
// ============================================================================

// Pin definitions
#define ACCEL_SDA         8
#define ACCEL_SCL         9
#define RF_PIN            2

// I2C address for SC7A20 (commonly 0x18 or 0x19)
#define SC7A20_ADDR       0x18

// Tilt detection threshold (in g)
// When plate falls, Z-axis drops from ~1g (flat) to near 0g (vertical)
#define TILT_THRESHOLD    0.3

// Debounce to prevent multiple triggers
#define DEBOUNCE_MS       2000

// RF message to send when plate is down
#define RF_MESSAGE        12345

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

bool wasDown = false;
unsigned long lastTriggerTime = 0;

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Plate Sensor Starting ===");
  
  // Initialize I2C on pins 8/9
  Wire.begin(ACCEL_SDA, ACCEL_SCL);
  
  // Initialize RF transmitter
  pinMode(RF_PIN, OUTPUT);
  digitalWrite(RF_PIN, LOW);
  
  Serial.println("RF Transmitter: GPIO " + String(RF_PIN));
  Serial.println("I2C: SDA=" + String(ACCEL_SDA) + ", SCL=" + String(ACCEL_SCL));
  
  delay(100);
  
  // Verify accelerometer responds
  if (!testAccelerometer()) {
    Serial.println("ERROR: Accelerometer not found!");
    Serial.println("Try I2C address 0x19 instead of 0x18");
  } else {
    Serial.println("Accelerometer detected");
  }
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  float x, y, z;
  if (!readAccelerometer(x, y, z)) {
    Serial.println("Failed to read accelerometer");
    delay(100);
    return;
  }
  
  Serial.printf("X: %.2f, Y: %.2f, Z: %.2f\n", x, y, z);
  
  // Plate is "down" when Z is near 0g
  bool isDown = (z < TILT_THRESHOLD);
  
  // Detect falling edge with debounce
  if (isDown && !wasDown) {
    unsigned long now = millis();
    if (now - lastTriggerTime > DEBOUNCE_MS) {
      Serial.println("Plate DOWN!");
      sendRFSignal();
      lastTriggerTime = now;
    }
  }
  
  wasDown = isDown;
  delay(100);
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

bool testAccelerometer() {
  Wire.beginTransmission(SC7A20_ADDR);
  return (Wire.endTransmission() == 0);
}

bool readAccelerometer(float& x, float& y, float& z) {
  // SC7A20 register addresses
  const uint8_t REG_X_H = 0x29, REG_X_L = 0x28;
  const uint8_t REG_Y_H = 0x2B, REG_Y_L = 0x2A;
  const uint8_t REG_Z_H = 0x2D, REG_Z_L = 0x2C;
  
  int16_t rawX, rawY, rawZ;
  
  // Read X
  Wire.beginTransmission(SC7A20_ADDR);
  Wire.write(REG_X_H);
  if (Wire.endTransmission() != 0) return false;
  Wire.requestFrom(SC7A20_ADDR, 1);
  rawX = Wire.read() << 8;
  Wire.beginTransmission(SC7A20_ADDR);
  Wire.write(REG_X_L);
  Wire.endTransmission();
  Wire.requestFrom(SC7A20_ADDR, 1);
  rawX |= Wire.read();
  
  // Read Y
  Wire.beginTransmission(SC7A20_ADDR);
  Wire.write(REG_Y_H);
  Wire.endTransmission();
  Wire.requestFrom(SC7A20_ADDR, 1);
  rawY = Wire.read() << 8;
  Wire.beginTransmission(SC7A20_ADDR);
  Wire.write(REG_Y_L);
  Wire.endTransmission();
  Wire.requestFrom(SC7A20_ADDR, 1);
  rawY |= Wire.read();
  
  // Read Z
  Wire.beginTransmission(SC7A20_ADDR);
  Wire.write(REG_Z_H);
  Wire.endTransmission();
  Wire.requestFrom(SC7A20_ADDR, 1);
  rawZ = Wire.read() << 8;
  Wire.beginTransmission(SC7A20_ADDR);
  Wire.write(REG_Z_L);
  Wire.endTransmission();
  Wire.requestFrom(SC7A20_ADDR, 1);
  rawZ |= Wire.read();
  
  // Convert to g (±2g range)
  const float SENSITIVITY = 1024.0;
  x = rawX / SENSITIVITY;
  y = rawY / SENSITIVITY;
  z = rawZ / SENSITIVITY;
  
  return true;
}

void sendRFSignal() {
  Serial.println("Sending RF signal...");
  
  // Simple OOK pulse train
  for (int repeat = 0; repeat < 5; repeat++) {
    for (int i = 0; i < 10; i++) {
      digitalWrite(RF_PIN, HIGH);
      delayMicroseconds(500);
      digitalWrite(RF_PIN, LOW);
      delayMicroseconds(500);
    }
    delay(10);
  }
  
  Serial.println("RF signal sent!");
}
