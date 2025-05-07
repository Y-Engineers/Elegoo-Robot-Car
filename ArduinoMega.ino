#include <Servo.h>  // Servo library
Servo myservo;      // Erzeuge Servo-Object zur Servosteuerung

const byte  servoPin =   3,
            servoMin =  30, // minmal 0
            servoMax = 150; // maximal 180

void setup() {

  Serial.begin(9600);

  Serial1.begin(9600);

  myservo.attach(servoPin);
  myservo.write(60); delay(1000); // gerade aus
}

void loop() {
  if (Serial1.available()) {
    String angleString = Serial1.readStringUntil('\n');
    Serial.println(angleString);
    int angle = angleString.toInt();
    angle = constrain(angle, servoMin, servoMax);
    myservo.write(angle);
  }
}

