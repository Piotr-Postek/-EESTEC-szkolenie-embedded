// LDR Characteristics
const float GAMMA = 0.7;
const float RL10 = 50;

void setup() {
  Serial.begin(9600);

}
void loop() {

  // Konwersja odczytanej wartości na lux
  int analogValue = analogRead(A0);
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

  Serial.print("Lux: ");
  Serial.println(lux);

  delay(100);
}

// TODO: Add LCD display to show actual value
