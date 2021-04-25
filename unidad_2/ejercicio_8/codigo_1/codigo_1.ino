void setup() {
  Serial.begin(115200);

}

void loop() {
  static uint16_t x = 0;

  if (Serial.available()) {
    if (Serial.read() == 0x73) {
      Serial.write((uint8_t)( x & 0x00FF ));
      Serial.write( (uint8_t)(x >> 8 ));
    }
  }
}
