#include <Servo.h>

Servo posServo;  // Position Servo (Pin 11)
Servo rotServo;  // Continuous Rotation Servo (Pin 9)

// Best neutral PWM value to stop the continuous servo from spinning
const int neutralPWM = 93;

void setup() {
  Serial.begin(9600); // Initialize serial communication for command input
  posServo.attach(11);     // Position Servo on Pin 11
  rotServo.attach(9);      // Rotation Servo on Pin 9

  // Send STOP at startup
  for (int i = 0; i < 3; i++) {
    rotServo.write(neutralPWM);
    delay(20);
  }

  Serial.println("Ready!");
  Serial.println("Use:");
  Serial.println("P[angle]→ 0 to 180 for Position Servo)");
  Serial.println("R[speed]→ -100 to 100 for Rotation Servo");
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();        // Read 'P' or 'R'
    delay(5);                             // Allow full input to buffer
    int value = Serial.parseInt();       // Read the number after command

    if (command == 'P' || command == 'p') {
      //POSITION SERVO CONTROL
      if (value >= 0 && value <= 180) {
        posServo.write(value);
        Serial.print(" Moved position servo to ");
        Serial.print(value);
        Serial.println("°");
      } else {
        Serial.println("Invalid angle. Use 0–180.");
      }

    } else if (command == 'R' || command == 'r') {
      //ROTATION SERVO CONTROL
      value = constrain(value, -100, 100);

      if (abs(value) < 5) {
        // Dead zone: reinforce STOP signal multiple times
        for (int i = 0; i < 3; i++) {
          rotServo.write(neutralPWM);
          delay(20);
        }
        Serial.print("Rotation servo STOPPED (PWM ");
        Serial.print(neutralPWM);
        Serial.println(")");
      } else {
        int pwm = map(value, -100, 100, 0, 180);
        rotServo.write(pwm);
        Serial.print("Rotation servo set to ");
        Serial.print(value);
        Serial.print("% → PWM ");
        Serial.println(pwm);
      }

    } else {
      Serial.println("Invalid command. Use P[angle] or R[speed].");
    }

    // Flush any leftover characters
    while (Serial.available()) {
      Serial.read();
    }
  }
}

