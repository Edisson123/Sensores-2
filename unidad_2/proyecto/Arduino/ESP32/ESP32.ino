#include <DS1307.h>
#include "Wire.h"
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

uint8_t info[23];
DS1307 DS1307(info);

#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme(BME_CS); // hardware SPI



void setup() {

  Serial.begin(115200);
  Wire.begin();

  unsigned status;
  status = bme.begin(); 
}

void loop() {

  if(Serial.available()){
    byte command = Serial.read();

    switch(command){

      case 0x03:
       
        Serial.readBytes(info, 7);
                         //segundos,minutos,horas,dia de la semana,dia,mes,año
        if(DS1307.SetClock(info[0],info[1],info[2],info[3],info[4],info[5],info[6])){
          Serial.write(0x4A);
        }else{
          Serial.write(0x05);
        }
        break;

      case 0x04:

        if(DS1307.ReadClock()){

          float information;
          information = bme.readTemperature();
          memcpy(&info[7],(uint8_t *)&information,4);
          
          information = (bme.readPressure() / 100.0F);
          memcpy(&info[11],(uint8_t *)&information,4);

          information = bme.readAltitude(SEALEVELPRESSURE_HPA);
          memcpy(&info[15],(uint8_t *)&information,4);
          
          information = bme.readHumidity();
          memcpy(&info[19],(uint8_t *)&information,4);
          
          Serial.write(0x4A);
          Serial.write(info, 23);
        }else{
          Serial.write(0x05);
        }
        break;
        
    }
    
  }

}
