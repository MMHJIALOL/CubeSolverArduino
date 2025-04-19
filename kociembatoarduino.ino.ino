// === Motor Enable Control ===
#define ENABLE_PIN 8  // Connect this to EN pin on all A4988s

// === CNC Shield Motors (X, Y, Z) ===
#define STEP_X 2
#define DIR_X 5

#define STEP_Y 3
#define DIR_Y 6

#define STEP_Z 4
#define DIR_Z 7

// === Breadboard Motors (A, M, N) ===
#define STEP_A 30
#define DIR_A 31

#define STEP_M 50
#define DIR_M 51

#define STEP_N 36
#define DIR_N 37

// === Settings ===
#define STEP_DELAY 1500         // Microseconds between step highs/lows
#define STEP_COUNT (200 / 4)    // Steps for 90° turn (adjust if needed)
#define MOTOR_DELAY 200         // Delay between moves in milliseconds

// === Move Sequence (Sample Scramble) ===
String moveSequence = "U' F L' B2 R D L' B2 U F2 D' B2 R2 D' L2 U2 B2 R2 D2 B' R'";

void setup() {
  Serial.begin(115200); 

  // Set enable pin and motor pins as output
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, HIGH); // Disable all motors initially

  int stepPins[] = {STEP_X, STEP_Y, STEP_Z, STEP_A, STEP_M, STEP_N};
  int dirPins[]  = {DIR_X, DIR_Y, DIR_Z, DIR_A, DIR_M, DIR_N};

  for (int i = 0; i < 6; i++) {
    pinMode(stepPins[i], OUTPUT);
    pinMode(dirPins[i], OUTPUT);
    digitalWrite(stepPins[i], LOW);
    digitalWrite(dirPins[i], LOW); // Default direction LOW
  }

  delay(100);  // Let pins settle
  digitalWrite(ENABLE_PIN, LOW);  // Enable all motors
  delay(50);   // Let driver enable stabilize

  Serial.println("Executing moves...");
  executeMoves(moveSequence);
}

void loop() {
  // Nothing in loop for now — only run once in setup
}

// === Rotate a motor ===
void rotateMotor(int stepPin, int dirPin, int steps, bool clockwise = true) {
  digitalWrite(dirPin, clockwise ? HIGH : LOW);
  delayMicroseconds(100); // Allow direction pin to stabilize

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(STEP_DELAY);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(STEP_DELAY);
  }
}

// === Get step/dir pins based on move letter ===
void getMotorPins(char move, int &stepPin, int &dirPin) {
  switch (move) {
    case 'U': stepPin = STEP_X; dirPin = DIR_X; break;
    case 'F': stepPin = STEP_Y; dirPin = DIR_Y; break;
    case 'R': stepPin = STEP_Z; dirPin = DIR_Z; break;
    case 'L': stepPin = STEP_A; dirPin = DIR_A; break;
    case 'D': stepPin = STEP_M; dirPin = DIR_M; break;
    case 'B': stepPin = STEP_N; dirPin = DIR_N; break;
    default: stepPin = -1; dirPin = -1; break;
  }
}

// === Parse and execute move string ===
void executeMoves(String sequence) {
  int i = 0;
  while (i < sequence.length()) {
    if (sequence[i] == ' ') {
      i++;
      continue;
    }

    char move = sequence[i];
    int stepPin, dirPin;
    getMotorPins(move, stepPin, dirPin);

    if (stepPin == -1) {
      Serial.print("Unknown move: ");
      Serial.println(move);
      i++;
      continue;
    }

    // Defaults
    int turns = 1;
    bool clockwise = true;

    // Check next character for modifier
    if (i + 1 < sequence.length()) {
      char next = sequence[i + 1];
      if (next == '\'') {
        clockwise = false;
        i++;
      } else if (next == '2') {
        turns = 2;
        i++;
      }
    }

    Serial.print("Executing move: ");
    Serial.print(move);
    if (!clockwise) Serial.print("'");
    if (turns == 2) Serial.print("2");
    Serial.println();

    for (int j = 0; j < turns; j++) {
      rotateMotor(stepPin, dirPin, STEP_COUNT, clockwise);
      delay(MOTOR_DELAY);
    }

    i++;
  }

  Serial.println("All moves executed.");

  digitalWrite(ENABLE_PIN, HIGH); // Disable all motors initially

}