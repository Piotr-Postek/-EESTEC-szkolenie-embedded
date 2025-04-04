#define TEMP_PIN A0

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Ustawienie wartości BETA dla danego termistora
  const float BETA = 3950;

  // Odczyt wartosci z pinu alanogowego
  int analogValue = analogRead(TEMP_PIN);

  // Przeliczenie wartosci na temperature w C
  float celsius = 1 / (log(1 / (1023.0 / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;

  // Wypisanie wartosci
  Serial.print("Temp: ");
  Serial.print(celsius);
  Serial.println("C");

  delay(500);

  // TODO: Dodanie wyswietlacza LCD do wyswietlenia aktualnej wartosci 
}