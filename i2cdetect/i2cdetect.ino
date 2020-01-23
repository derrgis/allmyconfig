#include <Wire.h>
#include <i2cdetect.h>

void setup() {
  Wire.begin(26,27);
  Serial.begin(115200);
  Serial.println("i2cdetect example\n");
  Serial.print("Scanning address range 0x03-0x77\n\n");
}

void loop() {
  i2cdetect();  // default range from 0x03 to 0x77
  delay(2000);
}
