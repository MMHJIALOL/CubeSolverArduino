#include <LiquidCrystal.h>

// === LCD Pins ===
LiquidCrystal lcd(23, 25, 27, 29, 31, 33);

// === Step/Dir/Enable Pins ===
// (same as your code, unchanged)
#define STEP_X 54
#define DIR_X 55
#define EN_X 38

#define STEP_Y 60
#define DIR_Y 61
#define EN_Y 56

#define STEP_Z 46
#define DIR_Z 48
#define EN_Z 62

#define STEP_A 26
#define DIR_A 28
#define EN_A 24

#define STEP_M 36
#define DIR_M 34
#define EN_M 30

#define STEP_N 40
#define DIR_N 42
#define EN_N 44

// === Motion Constants ===
#define STEP_COUNT 50
#define STEP_DELAY_MICROS 670
#define POST_DELAY 0

String moveSequence = "U B R' B' R D L' F' L2 F' L2 D2 L2 F' R2 F2 B L2 D L";
// String moveSequence = "L' D' L2 B' F2 R2 F L2 D2 L2 F L2 F L D' R' B R B' U'";


unsigned long startTime = 0;
unsigned long elapsedTime = 0;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.clear();

  setupMotorPins();

  lcd.setCursor(0, 0);
  lcd.print("The cube is going");
  lcd.setCursor(0, 1);
  lcd.print("to be solved...");

  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Solving...");

  startTime = millis();
  executeMoves(moveSequence);
  elapsedTime = millis() - startTime;

  disableAllMotors();

  // Show final time
  float seconds = elapsedTime / 1000.0;
  char buffer[17];

  for (int i = 0; i < 4; i++) {
    lcd.clear();
    dtostrf(seconds, 5, 2, buffer);
    lcd.setCursor(0, 0);
    lcd.print("Time: ");
    lcd.print(buffer);
    lcd.print("s");
    delay(400);

    lcd.clear();
    delay(200);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Your cube was");
  lcd.setCursor(0, 1);
  lcd.print("solved in ");
  lcd.print(buffer);
  lcd.print("s");
  delay(2000);        // Let user read the message    
  lcd.clear();        // Final screen clear

}

void loop() {
  // Nothing here
}

void setupMotorPins() {
  int stepPins[] = { STEP_X, STEP_Y, STEP_Z, STEP_A, STEP_M, STEP_N };
  int dirPins[]  = { DIR_X, DIR_Y, DIR_Z, DIR_A, DIR_M, DIR_N };
  int enPins[]   = { EN_X, EN_Y, EN_Z, EN_A, EN_M, EN_N };

  for (int i = 0; i < 6; i++) {
    pinMode(stepPins[i], OUTPUT);
    pinMode(dirPins[i], OUTPUT);
    pinMode(enPins[i], OUTPUT);
    digitalWrite(stepPins[i], LOW);
    digitalWrite(enPins[i], HIGH);
  }
}

void getMotorPins(char move, int &stepPin, int &dirPin, int &enPin) {
  switch (move) {
    case 'U': stepPin = STEP_X; dirPin = DIR_X; enPin = EN_X; break;
    case 'F': stepPin = STEP_Y; dirPin = DIR_Y; enPin = EN_Y; break;
    case 'R': stepPin = STEP_Z; dirPin = DIR_Z; enPin = EN_Z; break;
    case 'L': stepPin = STEP_A; dirPin = DIR_A; enPin = EN_A; break;
    case 'D': stepPin = STEP_M; dirPin = DIR_M; enPin = EN_M; break;
    case 'B': stepPin = STEP_N; dirPin = DIR_N; enPin = EN_N; break;
    default:  stepPin = -1; dirPin = -1; enPin = -1; break;
  }
}

void moveMotor(char move, int steps, bool clockwise) {
  int stepPin, dirPin, enPin;
  getMotorPins(move, stepPin, dirPin, enPin);

  if (stepPin == -1) return;

  digitalWrite(enPin, LOW);
  delay(5);
  digitalWrite(dirPin, clockwise ? HIGH : LOW);

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(STEP_DELAY_MICROS);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(STEP_DELAY_MICROS);

    // update timer display
    // updateTimerDisplay();
    if (i % 10000 == 0) updateTimerDisplay();
  }

  digitalWrite(enPin, HIGH);
  delay(POST_DELAY);
}

void executeMoves(String sequence) {
  int i = 0;

  while (i < sequence.length()) {
    if (sequence[i] == ' ') {
      i++;
      continue;
    }

    char move = sequence[i];
    bool clockwise = true;
    int turns = 1;

    if (i + 1 < sequence.length()) {
      if (sequence[i + 1] == '\'') {
        clockwise = false;
        i++;
      } else if (sequence[i + 1] == '2') {
        turns = 2;
        i++;
      }
    }

    Serial.print("Move: ");
    Serial.print(move);
    if (!clockwise) Serial.print("'");
    if (turns == 2) Serial.print("2");
    Serial.println();

    moveMotor(move, STEP_COUNT * turns, clockwise);
    i++;
  }
}

void updateTimerDisplay() {
  float currentTime = (millis() - startTime) / 1000.0;
  char buffer[6];
  dtostrf(currentTime, 5, 2, buffer);

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(buffer);
  lcd.print("s ");
}

void disableAllMotors() {
  digitalWrite(EN_X, HIGH);
  digitalWrite(EN_Y, HIGH);
  digitalWrite(EN_Z, HIGH);
  digitalWrite(EN_A, HIGH);
  digitalWrite(EN_M, HIGH);
  digitalWrite(EN_N, HIGH);
}
