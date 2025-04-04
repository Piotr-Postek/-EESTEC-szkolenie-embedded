#define DIRECTION_PIN       8
#define STEP_PIN            9
#define NUMBER_OF_STEPS     200
#define PULSE_WIDTH_MICROS  20
#define MILLIS_BETW_STEPS   10
#define BUTTON_1            2
#define BUTTON_2            3

int statostart;
void setup() { 
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
}

void loop() { 

  if(digitalRead(BUTTON_1) == HIGH){
    delay(50);
    if(digitalRead(BUTTON_1) == HIGH){
    digitalWrite(DIRECTION_PIN, HIGH);
    for(int n = 0; n < NUMBER_OF_STEPS; n++) {
     digitalWrite(STEP_PIN, HIGH);
     delayMicroseconds(PULSE_WIDTH_MICROS); 
     digitalWrite(STEP_PIN, LOW);
     delay(MILLIS_BETW_STEPS);
     digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
  }

  if(digitalRead(BUTTON_2) == HIGH){
    delay(50);
    if(digitalRead(BUTTON_2) == HIGH){
    digitalWrite(DIRECTION_PIN, LOW);
    for(int n = 0; n < NUMBER_OF_STEPS; n++) {
     digitalWrite(STEP_PIN, HIGH);
     delayMicroseconds(PULSE_WIDTH_MICROS); 
     digitalWrite(STEP_PIN, LOW);
     delay(MILLIS_BETW_STEPS);
     digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
  }
}
