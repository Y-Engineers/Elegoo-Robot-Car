#include <Servo.h>  // Servo library
Servo myservo;      // Erzeuge Servo-Object zur Servosteuerung

const byte  servoPin =   3,
            servoMin =  30, // minimal 0
            servoMax = 150; // maximal 180

// Motor pins for left and right motors
const byte ENA = 5, IN1 = 7, IN2 = 8;   // Left motor
const byte ENB = 6, IN3 = 9, IN4 = 11;  // Right motor

int speedValue = 150;       // Initial speed
int speedStep = 20;         // Step size for speed adjustments
const int minSpeed = 120;   // Minimum speed to overcome motor dead zone

void stopMotors() {
  analogWrite(ENA, 0); digitalWrite(IN1, HIGH); digitalWrite(IN2, HIGH);
  analogWrite(ENB, 0); digitalWrite(IN3, HIGH); digitalWrite(IN4, HIGH);
  Serial.println("Motors OFF");
}

void drive() {
  if (speedValue > 0) {
    // Forward
    analogWrite(ENA, speedValue); digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    analogWrite(ENB, speedValue); digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    Serial.println("Motors FORWARD");
  } else if (speedValue < 0) {
    // Reverse
    analogWrite(ENA, -speedValue); digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    analogWrite(ENB, -speedValue); digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    Serial.println("Motors REVERSE");
  } else {
    // Stop
    analogWrite(ENA, 0); digitalWrite(IN1, HIGH); digitalWrite(IN2, HIGH);
    analogWrite(ENB, 0); digitalWrite(IN3, HIGH); digitalWrite(IN4, HIGH);
    Serial.println("Motors OFF");
  }
}

void setup() {
  // Start the serial communication (USB to computer) for debugging
  Serial.begin(9600);

  Serial1.begin(9600);  // Set to match the baud rate of the sending device

  myservo.attach(servoPin);
  myservo.write(60); delay(1000); // Set initial servo position (straight)

  speedValue = 150;
}

void loop() {
  if (Serial1.available()) {
    String inputString = Serial1.readStringUntil('\n');  // Read until newline
    inputString.trim();  // Remove any leading/trailing whitespace

    if (inputString == "d") {
      // If 'd' is received, drive the motors
      drive();
    } else if (inputString == "s") {
      // If 's' is received, stop the motors
      stopMotors();
    } else if (inputString.startsWith("a")) {
      // If 'a' is received, try to turn the servo with the number following 'a'
      String angleString = inputString.substring(1);  // Get the number after 'a'
      int angle = angleString.toInt();  // Convert to integer
      if (angle != 0 || angleString == "0") {  // Check if it's a valid number
        angle = constrain(angle, servoMin, servoMax);  // Clamp angle to safe range
        myservo.write(angle);  // Move servo to the desired position
        Serial.print("Servo moved to: ");
        Serial.println(angle);
      } else {
        Serial.println("Invalid input for servo angle.");
      }
    } else {
      Serial.println("Invalid command.");
    }
  }
}
