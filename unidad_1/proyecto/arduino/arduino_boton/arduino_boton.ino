//uint8_t comando = 0xA2;
byte comando = 0xA2;
int boton = 2;
unsigned long tiempoViejo;

void setup() {
  Serial.begin(115200);
  pinMode(boton, INPUT_PULLUP); //Boton
  
}

void loop() {

  unsigned long tiempoActual = millis();
  if (tiempoActual >= tiempoViejo + 300) {

    tiempoViejo = tiempoActual;
    if (digitalRead(boton) == 0) {
      Serial.write(comando);
    }
    
  }
}
