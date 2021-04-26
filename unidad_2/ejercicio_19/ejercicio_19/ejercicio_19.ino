#include <Wire.h>

bool bandera=false;
uint8_t info[8];
void setup() {


  Serial.begin(9600);
  Wire.begin();

}


void loop() {

  if (SetClock(00,05,12,5,21, 4, 21)){ 
    
  }else{
    Serial.println("Error de comunicacion");
  }

  
  while(bandera==false){

    delay(1000);
    
    if(ReadClock(info)){
      //Serial.println(bin2bcd(5));
    }else{
      Serial.println("Error leyendo");
    }
      
    
  }
}


bool SetClock(uint8_t second,uint8_t minute, uint8_t hour, uint8_t wday, uint8_t day, uint8_t month, uint8_t year)
{
  // Iniciar el intercambio de información con el DS1307 (0x68)
  Wire.beginTransmission(0x68);

  // Escribir la dirección del registro segundero
  Wire.write(0x00);

  // Escribir valores en los registros, nos aseguramos que el bit clock halt
  // en el registro del segundero este desactivado (esto hace que el reloj funcione)
  Wire.write(bin2bcd(second)); // <--- Esto hace que el reloj comience a trabajar
  Wire.write(bin2bcd(minute));
  Wire.write(bin2bcd(hour));
  Wire.write(bin2bcd(wday));
  Wire.write(bin2bcd(day));
  Wire.write(bin2bcd(month));
  Wire.write(bin2bcd(year));

  // Terminamos la escritura y verificamos si el DS1307 respondio
  // Si la escritura se llevo a cabo el metodo endTransmission retorna 0
  if (Wire.endTransmission() != 0)
    return false;

  // Retornar verdadero si se escribio con exito
  return true;
}


bool SetClock(uint8_t second,uint8_t minute, uint8_t hour, bool format, bool am_pm)
{
  // Iniciar el intercambio de información con el DS1307 (0x68)
  Wire.beginTransmission(0x68);

  // Escribir la dirección del registro segundero
  Wire.write(0x00);

  // Escribir valores en los registros, nos aseguramos que el bit clock halt
  // en el registro del segundero este desactivado (esto hace que el reloj funcione)
  Wire.write(bin2bcd(second)); 
  Wire.write(bin2bcd(minute));
  uint8_t horasBCD = bin2bcd(hour);
  Wire.write(horasBCD | 0x20);
  info[7] = (horasBCD | 0x20);
  

  // Terminamos la escritura y verificamos si el DS1307 respondio
  // Si la escritura se llevo a cabo el metodo endTransmission retorna 0
  if (Wire.endTransmission() != 0)
    return false;

  // Retornar verdadero si se escribio con exito
  return true;
}


bool ReadClock(uint8_t *info)
{
  //uint8_t second, minute, hour, wday, day, month, year, ctrl;
  
  Wire.beginTransmission(0x68);

  
  Wire.write(0x00);

  // Terminamos la escritura y verificamos si el DS1307 respondio
  // Si la escritura se llevo a cabo el metodo endTransmission retorna 0
  if (Wire.endTransmission() != 0)
    return false;

  // Si el DS1307 esta presente, comenzar la lectura de 8 bytes
  Wire.requestFrom(0x68, 8);

  // Recibimos el byte del registro 0x00 y lo convertimos a binario
  info[0] = bcd2bin(Wire.read()); //segundos
  info[1] = bcd2bin(Wire.read()); // minutos
  info[2] = bcd2bin(Wire.read()); //horas
  info[3] = bcd2bin(Wire.read()); //Dia de la semana
  info[4] = bcd2bin(Wire.read()); //dia del mes
  info[5] = bcd2bin(Wire.read()); //mes
  info[6] = bcd2bin(Wire.read()); //año

  // Recibir los datos del registro de control en la dirección 0x07
  //ctrl = Wire.read();

  // Operacion satisfactoria, retornamos verdadero
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
  /*
  Serial.print("    HorasBCD  ");
  Serial.print(info[7]);
  Serial.println();
  */
  return true;
}

uint8_t bin2bcd(uint8_t bin)
{
  uint8_t decenas, unidades;

  decenas = bin/10;
  decenas = decenas << 4;
  unidades = bin%10;
  
  return (decenas | unidades);
}


uint8_t bcd2bin(uint8_t bcd)
{
  uint8_t decenas, unidades;

  decenas = (bcd & 0xF0);
  decenas = decenas>>4;
  decenas = decenas *10;
  unidades = bcd & 0x0F;
  return (decenas+unidades);
}
