#include "CytronMotorDriver.h"
#include <Wire.h> 

#define ENCODER_PIN_A_LEFT 21  // Interrupt pin for left encoder (a=green)
#define ENCODER_PIN_B_LEFT 20  // Secondary pin for left encoder
#define ENCODER_PIN_A_RIGHT 19 // Interrupt pin for right encoder
#define ENCODER_PIN_B_RIGHT 18 // Secondary pin for right encoder

volatile long encoderCountsLeft = 0;   // Encoder counts for left motor
volatile long encoderCountsRight = 0;  // Encoder counts for right motor

const float countsPerRevolution = 13.0; // Final calibrated PPR value
const float sprocketCircumference = 0.035; // Sprocket circumference in meters
const float distancePerCount = sprocketCircumference / countsPerRevolution;

// Configure the motor driver.
CytronMD motor1(PWM_DIR, 5, 4);  // PWM 1 = Pin 3, DIR 1 = Pin 4.
CytronMD motor2(PWM_DIR, 7, 6); // PWM 2 = Pin 9, DIR 2 = Pin 10.



void setup() {
  Serial.begin(9600); // Start serial communication
  
  // Attach interrupts for left encoder
  pinMode(ENCODER_PIN_A_LEFT, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B_LEFT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A_LEFT), leftEncoderISR, RISING);

  // Attach interrupts for right encoder
  pinMode(ENCODER_PIN_A_RIGHT, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B_RIGHT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A_RIGHT), rightEncoderISR, RISING);
}

void loop() {

  noInterrupts(); // Temporarily disable interrupts to read encoder counts safely
  long countsLeft = encoderCountsLeft;
  long countsRight = encoderCountsRight;
  interrupts(); // Re-enable interrupts

  //motor1.setSpeed(-255);  // Motor 1 runs backward at full speed.
  //motor2.setSpeed(255);   // Motor 2 runs forward at full speed.

  // Calculate distances
  float distanceLeft = countsLeft * distancePerCount;
  float distanceRight = countsRight * distancePerCount;

  // Send distances over Serial (or use for further processing)
  Serial.print("Distance Left (m): ");
  Serial.print(distanceLeft, 4);
  Serial.print(", Distance Right (m): ");
  Serial.println(distanceRight, 4);
  


  // Small delay to avoid spamming Serial Monitor
  delay(100);
}

// ISR for left encoder
void leftEncoderISR() {
  if (digitalRead(ENCODER_PIN_B_LEFT) == HIGH) {
    encoderCountsLeft++;
  } else {
    encoderCountsLeft--;
  }
}

// ISR for right encoder
void rightEncoderISR() {
  if (digitalRead(ENCODER_PIN_B_RIGHT) == HIGH) {
    encoderCountsRight++;
  } else {
    encoderCountsRight--;
  }
}
