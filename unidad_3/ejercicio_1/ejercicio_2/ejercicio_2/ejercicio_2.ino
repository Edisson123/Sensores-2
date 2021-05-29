#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFiMulti.addAP("COSMOS_PENAGOS", "80986775");
  Serial.println();
  Serial.print("Esperando red WiFi...");

  while (WiFiMulti.run() != WL_CONNECTED) {
  
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Red WiFI conectada");
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:

}
