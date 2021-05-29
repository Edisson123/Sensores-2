#include <WiFi.h>
#include <WiFiUdp.h>
#include <DS1307.h>
#include "Wire.h"
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

const char* ssid = "Wifi483";
const char* password = "lucero483";
WiFiUDP udpDevice;
uint16_t localUdpPort = 4023;
uint16_t UDPPort = 666;
#define MAX_LEDSERVERS 3
const char* hosts[MAX_LEDSERVERS] = {"192.168.1.56", "192.168.1.103", "?.?.?.?"};
#define SERIALMESSAGESIZE 3
uint32_t previousMillis = 0;
uint32_t previousMillisLoop = 0;
#define ALIVE 1000
#define D0 5

//Variables para el reloj
uint8_t info[7];
DS1307 DS1307(info);

void setup() {
  pinMode(D0, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(D0, HIGH);
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Wire.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());
  udpDevice.begin(localUdpPort);
}

void networkTask() {

  // UDP event:
  uint8_t packetSize = udpDevice.parsePacket();
  if (packetSize) {
    Serial.print("Data from: ");
    Serial.print(udpDevice.remoteIP());
    Serial.print(":");
    Serial.print(udpDevice.remotePort());
    Serial.print(' ');
    uint8_t command = udpDevice.read();

    Serial.write(command);
    switch (command) {

      case 0x03:

        //Serial.readBytes(info, 7);
        for (int i = 0; i <= packetSize; i++) {

          info[i] = udpDevice.read();
        }
        //segundos,minutos,horas,dia de la semana,dia,mes,año
        if (DS1307.SetClock(info[0], info[1], info[2], info[3], info[4], info[5], info[6])) {
          //Serial.write(0x4A);
        } else {
          udpDevice.beginPacket(hosts[0] , UDPPort);
          udpDevice.write(0x03);
          udpDevice.write(' ');
          udpDevice.endPacket();
          //Serial.write(0x05);
        }
        break;

      default:
        break;
    }

  }

  uint32_t currentMillis;
  currentMillis  = millis();
  if ((currentMillis - previousMillisLoop) >= ALIVE) {
    previousMillisLoop = currentMillis;

    if (DS1307.ReadClock()) {
      udpDevice.beginPacket(hosts[0] , UDPPort);
      udpDevice.write(0x4A);
      udpDevice.write(info, 7);

      udpDevice.endPacket();
      Serial.println("envio");

    } else {

      udpDevice.beginPacket(hosts[0] , UDPPort);
      udpDevice.write(0x05);
      udpDevice.write(' ');
      udpDevice.endPacket();
    }

  }

}

void aliveTask() {
  uint32_t currentMillis;
  static uint8_t ledState = 0;
  currentMillis  = millis();
  if ((currentMillis - previousMillis) >= ALIVE) {
    previousMillis = currentMillis;
    if (ledState == 0) {
      digitalWrite(D0, HIGH);
      ledState = 1;
    }
    else {
      digitalWrite(D0, LOW);
      ledState = 0;
    }
  }
}



void loop() {
  networkTask();
  aliveTask();
}
