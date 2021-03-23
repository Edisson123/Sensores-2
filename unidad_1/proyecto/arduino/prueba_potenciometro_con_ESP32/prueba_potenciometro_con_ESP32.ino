// Potenciometro conectado a GPIO 35
const int portPin = 15;
// Almacenamiento del valor de puerto (Rango de 0-4095)
float potValor, valorMap = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);
}
void loop() {
  // Lectura del valor en cada vuelta del bucle
  potValor = analogRead(portPin);
  valorMap = map(potValor, 0, 4095, -750, 750);
  Serial.println(valorMap/100);  //Envío del valor al puerto serie
  delay(50);
}
