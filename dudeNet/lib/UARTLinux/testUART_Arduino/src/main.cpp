#include <Arduino.h>

void setup() {
  Serial.begin(115200, SERIAL_8N2);
}

void loop() {
  /*
  Serial.write(0x23);
  delay(1000);
  */

  if(Serial.available()){
    Serial.write(Serial.read());
  }
  //Serial.print("\n");
}
