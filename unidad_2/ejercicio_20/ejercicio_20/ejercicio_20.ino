#include <DS1307.h>
#include "Wire.h"

uint8_t info[8];
bool bandera = false;
DS1307 DS1307(info);

void setup() {

  Serial.begin(9600);
  Wire.begin();

}

void loop() {

  if (DS1307.SetClock(00, 05, 12, 5, 21, 4, 21)) {

  } else {
    Serial.println("Error de comunicacion");
  }


  while (bandera == false) {

    delay(1000);

    if (DS1307.ReadClock()) {

      Serial.print("Fecha: ");
      Serial.print(info[4]);
      Serial.print('/');
      Serial.print(info[5]);
      Serial.print('/');
      Serial.print(info[6]);

      Serial.print("  Hora: ");
      Serial.print(info[2]);
      Serial.print(':');
      Serial.print(info[1]);
      Serial.print(':');
      Serial.println(info[0]);

    } else {
      Serial.println("Error leyendo");
    }

  }

}
