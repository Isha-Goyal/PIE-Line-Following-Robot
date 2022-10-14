#include <Adafruit_MotorShield.h>

#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_Circuit_Playground.h>

// include required libraries
#include <Wire.h> // this is for some sort of controller/connection/thing
#include <Adafruit_MotorShield.h> // motorshield library
#include "utility/Adafruit_MS_PWMServoDriver.h" // maybe don't need? try commenting out later

// create adafruit_motorshield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// create DC motor object
// note: it must have the asterisk to start for some reason. something about point and non
// pointer types and scalar stuff. not exactly sure, but keep it.
Adafruit_DCMotor *right = AFMS.getMotor(3); // currently connected to M1 (change if using different port)
Adafruit_DCMotor *left = AFMS.getMotor(4); // currently connected to M4 (change if using different port)

void setup() {
  AFMS.begin();

  // speeds range from 0 (stopped) to 255 (full speed)
  right->setSpeed(50);
  left->setSpeed(50);

//  myTime = millis();

}

void loop() {
  // put your main code here, to run repeatedly:
  right->run(FORWARD);
  left->run(FORWARD);

}

// drives both wheels at the same speed for the amount of milliseconds specified
//void driveForward(int period) {
//  currentTime = myTime; //  starting time
//  while (myTime <= currentTime + period) { // goes until the period of time has elapsed
//    right->run(FORWARD);
//    left->run(FORWARD);
//  }
//
//  // stops once the time has passed
//  right->run(RELEASE);
//  left->run(RELEASE);
//  
//}
//
//// has the robot stop and wait for the amount of milliseconds specified
//void wait(int period) {
//  currentTime = myTime;
//  while (myTime <= currentTime + period) {
//    right->run(RELEASE);
//    left->run(RELEASE);
//  }
//}
