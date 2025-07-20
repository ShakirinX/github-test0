// Include iBusBM Library
#include <IBusBM.h>
#include "CytronMotorDriver.h"

// Configure the motor driver.    PWM, DIR
CytronMD motorL(PWM_DIR, 6, 5);  // Left motor: PWM = Pin 6, DIR = Pin 5
CytronMD motorR(PWM_DIR, 10, 9); // Right motor: PWM = Pin 10, DIR = Pin 9

// Create iBus Object
IBusBM ibus;

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
  uint16_t ch = ibus.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Read the channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void setup() {
  // Start serial monitor
  Serial.begin(115200);

  // Attach iBus object to serial port
  ibus.begin(Serial1);
}

void loop() {
  // Read motor speeds from Channel 1 and 2
  int LM = readChannel(0, -255, 255, 0); // Channel 1 -> LM
  int RM = readChannel(1, -255, 255, 0); // Channel 2 -> RM
  int rot = readChannel(3, -255, 255, 0);
  int status = readSwitch(4, false);

  // Print all 4 analog channels (1 to 4)
  for (byte i = 0; i < 4; i++) {
    int value = readChannel(i, -255, 255, 0);
    Serial.print("Ch");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(value);
    Serial.print(" | ");
  }

  // Print switch states
  Serial.print("Ch5: ");
  Serial.print(readSwitch(4, false));
  Serial.print(" | ");
  Serial.print("Ch6: ");
  Serial.print(readSwitch(5, false));
  Serial.println();

  if (status == true){
    if (rot < -150){
    motorL.setSpeed(-255);
    motorR.setSpeed(255);
    delay(500);
    }

    else if (rot > 150){
      motorL.setSpeed(255);
      motorR.setSpeed(-255);
      delay(500);
    }

    else{
      motorL.setSpeed(LM);
      motorR.setSpeed(RM);
    }
    delay(10);
  }
  else {
    Serial.print(" \nNot Active\n ");
  }
  
  
}
