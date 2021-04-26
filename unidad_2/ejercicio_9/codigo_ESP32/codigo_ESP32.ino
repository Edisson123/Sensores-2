void setup() {
  Serial.begin(115200);

}

void loop() {
  static uint16_t x = 0;
  static uint16_t y = 0;
  static uint16_t z = 0;
  static bool countUp = true;


  if (Serial.available()) {
    if (Serial.read() == 0x73) {
      Serial.write((uint8_t)( x & 0x00FF));
      Serial.write( (uint8_t)( x >> 8 ));
      Serial.write((uint8_t)( y & 0x00FF ));
      Serial.write((uint8_t)(y >> 8 ));
      Serial.write((uint8_t)( z & 0x00FF ));
      Serial.write((uint8_t)(z >> 8 ));

      if (countUp == true) {
        if (x < 1000) {
          x = x + 1;
          y = y + 1;
          z = z + 1;
        }
        else countUp = false;
      }

      if (countUp == false)
      {
        if (x > 0) {
          x = x - 1;
          y = y - 1;
          z = z - 1;
        }
        else countUp = true;
      }
    }
  }
}
