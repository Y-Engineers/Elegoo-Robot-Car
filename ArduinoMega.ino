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
void logBoth(const String& msg) {
  Serial.println(msg);
  Serial1.println(msg);
}

void logBothf(const char* prefix, int value) {
  Serial.print(prefix); Serial.println(value);
  Serial1.print(prefix); Serial1.println(value);
}

void stopMotors() {
  analogWrite(ENA, 0); digitalWrite(IN1, HIGH); digitalWrite(IN2, HIGH);
  analogWrite(ENB, 0); digitalWrite(IN3, HIGH); digitalWrite(IN4, HIGH);
  logBoth("[Feedback] Motors stopped.");
}

void drive() {
  if (speedValue > 0) {
    analogWrite(ENA, speedValue); digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    analogWrite(ENB, speedValue); digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    logBothf("[Feedback] Driving forward at speed ", speedValue);
  } else if (speedValue < 0) {
    analogWrite(ENA, -speedValue); digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    analogWrite(ENB, -speedValue); digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    logBothf("[Feedback] Driving backward at speed ", -speedValue);
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
  myservo.write(60); delay(1000);

  stopMotors();
  logBoth("[System] Ready.");
}

// ====== LOOP ======
void loop() {
  if (Serial1.available()) {
    String input = Serial1.readStringUntil('\n');
    input.trim();
    logBoth("[Input] Received: " + input);

    if (input == "d") {
      speedValue = abs(speedValue);
      drive();
    } else if (input == "b") {
      speedValue = -abs(speedValue);
      drive();
    } else if (input == "s") {
      stopMotors();
    } else if (input.startsWith("a")) {
      String angleStr = input.substring(1);
      int angle = angleStr.toInt();
      if (angleStr == "0" || angle != 0) {
        angle = constrain(angle, servoMin, servoMax);
        myservo.write(angle);
        logBothf("[Feedback] Servo moved to angle: ", angle);
      } else {
        logBoth("[Error] Invalid angle value.");
      }
    } else if (input.startsWith("v")) {
      int newSpeed = input.substring(1).toInt();
      if ((input.substring(1) == "0") || newSpeed != 0) {
        if (newSpeed >= minSpeed && newSpeed <= 255) {
          speedValue = newSpeed;
          logBothf("[Feedback] Speed set to: ", speedValue);
        } else {
          logBoth("[Error] Speed out of range (120–255).");
        }
      } else {
        logBoth("[Error] Invalid speed value.");
      }
    } else {
      logBoth("[Error] Unknown command.");
    }
  }
}
