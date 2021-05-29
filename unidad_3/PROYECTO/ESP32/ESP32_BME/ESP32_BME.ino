#include <WiFi.h>
#include <WiFiUdp.h>
#include "Wire.h"
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

const char* ssid = "Wifi483";
const char* password = "lucero483";
WiFiUDP udpDevice;
uint16_t localUdpPort = 4022;
uint16_t UDPPort = 911;
#define MAX_LEDSERVERS 3
const char* hosts[MAX_LEDSERVERS] = {"192.168.1.56", "192.168.1.103", "?.?.?.?"};
#define SERIALMESSAGESIZE 3
uint32_t previousMillis = 0;
uint32_t previousMillisLoop = 0;
#define ALIVE 1000
#define D0 5

#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme(BME_CS); // hardware SPI
//Variables para el reloj
uint8_t info[17];

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

  unsigned status;
  status = bme.begin(); 

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


  uint32_t currentMillis;
  currentMillis  = millis();
  if ((currentMillis - previousMillisLoop) >= ALIVE) {
    previousMillisLoop = currentMillis;

    Serial.println("envioBME");

    float information;
    information = bme.readTemperature();
    Serial.println(information);
    memcpy(&info[1],(uint8_t *)&information,4);
    
    information = (bme.readPressure() / 100.0F);
    Serial.println(information);
    memcpy(&info[5],(uint8_t *)&information,4);

    information = bme.readAltitude(SEALEVELPRESSURE_HPA);
    Serial.println(information);
    memcpy(&info[9],(uint8_t *)&information,4);
    
    information = bme.readHumidity();
    Serial.println(information);
    memcpy(&info[13],(uint8_t *)&information,4);
    
    udpDevice.beginPacket(hosts[0] , UDPPort);
    info[0] = 0x47;
    udpDevice.write(info, 17);

    udpDevice.endPacket();
    

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
