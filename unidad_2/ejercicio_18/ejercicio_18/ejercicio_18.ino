#include <Wire.h>

bool pruebaConexion(uint8_t direccion);


void setup() {
  Wire.begin();
  Serial.begin(9600);

}

void loop() {

  if(pruebaConexion(104)){

    Serial.println("Dispositivo Disponible");
  }else{
    Serial.println("Error de Conexion");
  }

  delay(1000);
}


bool pruebaConexion(uint8_t direccion){

  Wire.beginTransmission(direccion);
  Wire.write(0x00);

  /*Terminamos la escritura y verificamos si el DS1307 respondio
    Si la escritura se llevo a cabo el metodo endTransmission retorna 0
  */
  if (Wire.endTransmission() != 0)
    return false;

  return true;
}
