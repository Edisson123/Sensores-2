uint32_t last_time = 0;
float potValor, valorMap = 0;
const int portPin = 15;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  // Print a heartbeat
  if(Serial.available() > 0){
    
    if(Serial.read() == 'A'){
        if (millis() > last_time + 20){
    
        //Serial.println("Arduino is alive!!");
        last_time = millis();
        potValor = analogRead(portPin);
        valorMap = map(potValor, 0, 4095, -750, 750);
        Serial.println(valorMap/100);  //Envío del valor al puerto serie
        }
    }
    
  }


}
