#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#include "SafeState.h"
#include "icons.h"

/* Wyswietlacz */
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

/* LED */
#define PIN_R 6

/* Buzzer */
#define SPEAKER_PIN 13

// Nuty
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294

/* Ustawienie keepada */
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2};
byte colPins[KEYPAD_COLS] = {A3, A2, A1, A0};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

/* Ustawienie zmiennej safestate do pamieci EEPROM */
SafeState safeState;


void lock() {
  safeState.lock();
}

void unlock() {
    digitalWrite(PIN_R, LOW);
    noTone(SPEAKER_PIN);
}


void showStartupMessage() {
  lcd.setCursor(4, 0);
  lcd.print("Witaj!");
  delay(1000);

  lcd.setCursor(0, 2);
  String message = "UNO Alarm v1.0";
  for (byte i = 0; i < message.length(); i++) {
    lcd.print(message[i]);
    delay(100);
  }
  delay(500);
}

String inputSecretCode() {
  lcd.setCursor(5, 1);
  lcd.print("[____]");
  lcd.setCursor(6, 1);
  String result = "";
  while (result.length() < 4) {
    char key = keypad.getKey();
    if (key >= '0' && key <= '9') {
      lcd.print('*');
      result += key;
    }
  }
  return result;
}

void showWaitScreen(int delayMillis) {
  lcd.setCursor(2, 1);
  lcd.print("[..........]");
  lcd.setCursor(3, 1);
  for (byte i = 0; i < 10; i++) {
    delay(delayMillis);
    lcd.print("=");
  }
}

bool setNewCode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Podaj kod:");
  String newCode = inputSecretCode();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Zatwierdz kod");
  String confirmCode = inputSecretCode();

  if (newCode.equals(confirmCode)) {
    safeState.setCode(newCode);
    return true;
  } else {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Odmowa!");
    lcd.setCursor(0, 1);
    lcd.print("Kod niezgodny!");
    delay(2000);
    return false;
  }
}

void showUnlockMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(ICON_UNLOCKED_CHAR);
  lcd.setCursor(4, 0);
  lcd.print("Otwarte");
  lcd.setCursor(15, 0);
  lcd.write(ICON_UNLOCKED_CHAR);
  delay(1000);
}

void safeUnlockedLogic() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.write(ICON_UNLOCKED_CHAR);
  lcd.setCursor(2, 0);
  lcd.print(" # zablokuj");
  lcd.setCursor(15, 0);
  lcd.write(ICON_UNLOCKED_CHAR);

  bool newCodeNeeded = true;

  if (safeState.hasCode()) {
    lcd.setCursor(0, 1);
    lcd.print("  A = nowy kod");
    newCodeNeeded = false;
  }

  auto key = keypad.getKey();
  while (key != 'A' && key != '#') {
    key = keypad.getKey();
  }

  bool readyToLock = true;
  if (key == 'A' || newCodeNeeded) {
    readyToLock = setNewCode();
  }

  if (readyToLock) {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.write(ICON_UNLOCKED_CHAR);
    lcd.print(" ");
    lcd.write(ICON_RIGHT_ARROW);
    lcd.print(" ");
    lcd.write(ICON_LOCKED_CHAR);

    safeState.lock();
    lock();
    showWaitScreen(100);
  }
}

void safeLockedLogic() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(ICON_LOCKED_CHAR);
  lcd.print(" Uzbrojono! ");
  lcd.write(ICON_LOCKED_CHAR);

  String userCode = inputSecretCode();
  bool unlockedSuccessfully = safeState.unlock(userCode);
  showWaitScreen(200);

  if (unlockedSuccessfully) {
    showUnlockMessage();
    unlock();
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Odmowa!");
    digitalWrite(PIN_R, HIGH);

    tone(SPEAKER_PIN, NOTE_C4);
    delay(30);
    tone(SPEAKER_PIN, NOTE_CS4);
    delay(30);
    tone(SPEAKER_PIN, NOTE_D4);

    showWaitScreen(1000);
  }
}

void setup() {
  lcd.begin(16, 2);
  init_icons(lcd);

  /* Sprawdzenie czy zmienna jest w pamieci EEPROM i ustawienie odpowiedniego stanu */
  Serial.begin(115200);
  if (safeState.locked()) {
    lock();
  } else {
    unlock();
  }

  showStartupMessage();

  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(PIN_R, OUTPUT);

  randomSeed(analogRead(A0));

  tone(SPEAKER_PIN, NOTE_C4);
  delay(30);
  tone(SPEAKER_PIN, NOTE_CS4);
  delay(30);
  tone(SPEAKER_PIN, NOTE_D4);
  delay(30);
  noTone(SPEAKER_PIN);
}

void loop() {
  if (safeState.locked()) {
    safeLockedLogic();
  } else {
    safeUnlockedLogic();
  }
}
