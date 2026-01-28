# Plate Sensor - Dependencies

## Required Libraries

### 1. RCSwitch (Recommended for RF)
For easy RF transmission.

**Installation via Arduino Library Manager:**
1. Open Arduino IDE
2. Tools → Manage Libraries
3. Search for "RCSwitch"
4. Install "RCSwitch" by sui77

**Installation via CLI:**
```bash
arduino-cli lib install "RCSwitch"
```

---

## Optional: Using Without RCSwitch

The `main.ino` code includes a simple RF implementation using digitalWrite. 
This works without any external libraries but may be less reliable.

To use the simple implementation, the current code already uses it.

---

## Testing the Accelerometer

Before uploading, test the accelerometer standalone:

```cpp
#include <Wire.h>

#define SC7A20_ADDR 0x18

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);  // SDA=8, SCL=9
  
  delay(1000);
  
  // Test I2C connection
  Wire.beginTransmission(SC7A20_ADDR);
  if (Wire.endTransmission() == 0) {
    Serial.println("Accelerometer found at 0x" + String(SC7A20_ADDR, HEX));
  } else {
    Serial.println("Accelerometer NOT found!");
    Serial.println("Try address 0x19 instead of 0x18");
  }
}

void loop() {}
```

---

## I2C Address

The SC7A20 commonly uses:
- **0x18** (SDO pulled low)
- **0x19** (SDO pulled high)

If your accelerometer doesn't respond, try changing `SC7A20_ADDR` to `0x19`.
