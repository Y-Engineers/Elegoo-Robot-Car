#include <Servo.h>  // Servo library

// ====== CONSTANTS ======
constexpr byte servoPin = 3;
constexpr byte servoMin = 30;
constexpr byte servoMax = 150;

constexpr byte ENA = 5, IN1 = 7, IN2 = 8;   // Left motor
constexpr byte ENB = 6, IN3 = 9, IN4 = 11;  // Right motor

const int minSpeed = 120;
int speedValue = 150;
const int speedStep = 20;

const bool DEBUG = true;

// ====== SERVO ======
Servo myservo;

// ====== BUFFER ======
#define BUFFER_SIZE 32
char inputBuffer[BUFFER_SIZE];
byte inputPos = 0;

// ====== HELPER FUNCTIONS ======

void applyMotorBrake() {
  analogWrite(ENA, 0); digitalWrite(IN1, HIGH); digitalWrite(IN2, HIGH);
  analogWrite(ENB, 0); digitalWrite(IN3, HIGH); digitalWrite(IN4, HIGH);
}

void stopMotors() {
  applyMotorBrake();
  if (DEBUG) Serial.println("Motors OFF");
}

void drive() {
  if (speedValue > 0) {
    // Forward
    analogWrite(ENA, speedValue); digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    analogWrite(ENB, speedValue); digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    if (DEBUG) Serial.println("Motors FORWARD");
  } else if (speedValue < 0) {
    // Reverse
    analogWrite(ENA, -speedValue); digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    analogWrite(ENB, -speedValue); digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    if (DEBUG) Serial.println("Motors REVERSE");
  } else {
    applyMotorBrake();
    if (DEBUG) Serial.println("Motors OFF (speed = 0)");
  }
}

void processCommand(String input) {
  input.trim();
  if (input == "d") {
    speedValue = abs(speedValue);
    drive();
  } else if (input == "b") {
    speedValue = -abs(speedValue);
    drive();
  } else if (input == "s") {
    stopMotors();
  } else if (input.startsWith("a")) {
    String angleString = input.substring(1);
    int angle = angleString.toInt();
    if (angleString == "0" || angle != 0) {
      angle = constrain(angle, servoMin, servoMax);
      myservo.write(angle);
      if (DEBUG) {
        Serial.print("Servo moved to: ");
        Serial.println(angle);
      }
    } else {
      Serial.println("Invalid input for servo angle.");
    }
  } else if (input.startsWith("v")) {
    int newSpeed = input.substring(1).toInt();
    if (newSpeed >= minSpeed && newSpeed <= 255) {
      speedValue = newSpeed;
      if (DEBUG) {
        Serial.print("Speed set to: ");
        Serial.println(speedValue);
      }
    } else {
      Serial.println("Invalid speed value.");
    }
  } else {
    Serial.println("Invalid command.");
  }
}

// ====== SETUP ======
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  myservo.attach(servoPin);
  myservo.write(60); delay(1000);  // Initial straight position

  speedValue = 150;
  stopMotors();

  if (DEBUG) Serial.println("System ready.");
}

// ====== LOOP ======
void loop() {
  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\n') {
      inputBuffer[inputPos] = '\0';  // Null-terminate
      processCommand(String(inputBuffer));
      inputPos = 0;
    } else if (inputPos < BUFFER_SIZE - 1) {
      inputBuffer[inputPos++] = c;
    }
  }
}
