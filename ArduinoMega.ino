#include <Servo.h>

// ====== CONSTANTS ======
const byte servoPin = 3;
const byte servoMin = 30;
const byte servoMax = 150;

const byte ENA = 5, IN1 = 7, IN2 = 8;   // Left motor
const byte ENB = 6, IN3 = 9, IN4 = 11;  // Right motor

const int minSpeed = 120;
int speedValue = 150;

Servo myservo;

// ====== HELPER FUNCTIONS ======
void stopMotors() {
  analogWrite(ENA, 0); digitalWrite(IN1, HIGH); digitalWrite(IN2, HIGH);
  analogWrite(ENB, 0); digitalWrite(IN3, HIGH); digitalWrite(IN4, HIGH);
  Serial.println("[Feedback] Motors stopped.");
  Serial1.println("[Feedback] Motors stopped.");
}

void drive() {
  if (speedValue > 0) {
    analogWrite(ENA, speedValue); digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    analogWrite(ENB, speedValue); digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    Serial.print("[Feedback] Driving forward at speed "); Serial.println(speedValue);
    Serial1.print("[Feedback] Driving forward at speed "); Serial1.println(speedValue);
  } else if (speedValue < 0) {
    analogWrite(ENA, -speedValue); digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    analogWrite(ENB, -speedValue); digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    Serial.print("[Feedback] Driving backward at speed "); Serial.println(-speedValue);
    Serial1.print("[Feedback] Driving backward at speed "); Serial1.println(-speedValue);
  } else {
    stopMotors();
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

  stopMotors();
  Serial.println("[System] Ready.");
  Serial1.println("[System] Ready.");
}

// ====== LOOP ======
void loop() {
  if (Serial1.available()) {
    String inputString = Serial1.readStringUntil('\n');
    inputString.trim();

    Serial.print("[Input] Received: ");
    Serial.println(inputString);

    Serial1.print("[Input] Received: ");
    Serial1.println(inputString);

    if (inputString == "d") {
      speedValue = abs(speedValue);
      drive();
    } else if (inputString == "b") {
      speedValue = -abs(speedValue);
      drive();
    } else if (inputString == "s") {
      stopMotors();
    } else if (inputString.startsWith("a")) {
      String angleString = inputString.substring(1);
      int angle = angleString.toInt();
      if (angleString == "0" || angle != 0) {
        angle = constrain(angle, servoMin, servoMax);
        myservo.write(angle);
        Serial.print("[Feedback] Servo moved to angle: ");
        Serial.println(angle);
        Serial1.print("[Feedback] Servo moved to angle: ");
        Serial1.println(angle);
      } else {
        Serial.println("[Error] Invalid angle value.");
        Serial1.println("[Error] Invalid angle value.");
      }
    } else if (inputString.startsWith("v")) {
      int newSpeed = inputString.substring(1).toInt();
      if ((inputString.substring(1) == "0") || newSpeed != 0) {
        if (newSpeed >= minSpeed && newSpeed <= 255) {
          speedValue = newSpeed;
          Serial.print("[Feedback] Speed set to: ");
          Serial.println(speedValue);
          Serial1.print("[Feedback] Speed set to: ");
          Serial1.println(speedValue);
        } else {
          Serial.println("[Error] Speed out of range (120–255).");
          Serial1.println("[Error] Speed out of range (120–255).");
        }
      } else {
        Serial.println("[Error] Invalid speed value.");
        Serial1.println("[Error] Invalid speed value.");
      }
    } else {
      Serial.println("[Error] Unknown command.");
      Serial1.println("[Error] Unknown command.");
    }
  }
}
