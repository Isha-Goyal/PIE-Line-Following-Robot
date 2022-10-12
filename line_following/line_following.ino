// keeps track of sensor pins
const uint8_t rightSensor = 9;
const uint8_t leftSensor = 8;

// PID variables
// our error is the difference between the left and right wheels, so we'll have a small number if they're both off the tape
// and a much larger error if one is on and one is off (we'd expect something around 15 or 20 given inital calibration data).
double e = rightSensor - leftSensor;

// our desired setpoint (SV) is 0, because we want both sensors to be over ground (as opposed to tape) and have similar sensor
// readings, so when you take the difference you want it to be small (ideally 0 but realistically just small).
// usually the error is the setpoint minus the process variable (PV), but in this case we'll just use our error since that's
// essentially the PV.

// control variable -- the tuning of P, I, and D are going to come together in a
// weighted sum to genearte a control variable that we can then use to adjust the turning
double u = 0;

// for now, I'm setting it to just be a P controller (so I and D are 0). This is for simplicity 
// while we try to understand it. We can add them in later as we see fit.
double P = 1; // proportional to current value of error -- must be tuned
double I = 0; // integrates past values of error over time to eliminate cumulative error based on history
double D = 0; // estimates future trend of error based on current rate of change and attempts to damp large changes in error

// motor stuff
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

int baseSpeed = 50;

// boolean to keep track of whether we want the robot to run or not so we can start and stop at will
bool start = true;

void setup() {

  Serial.begin(9600);
  Serial.print("connected");

  pinMode(rightSensor, INPUT);
  pinMode(leftSensor, INPUT);

  AFMS.begin();

  // speeds range from 0 (stopped) to 255 (full speed)
  right->setSpeed(baseSpeed);
  left->setSpeed(baseSpeed);

  // t = millis(); // keep track of time from start
  
}

void loop() {

  // NOTE: need to  figure out how this is intaking serial commands
//  if (Serial.available()) {
//    char cmd = Serial.read();
//    switch(cmd) {
//      case 1:
//      
//    }
//  }

  if (start) { // if we're in the start state
    
    e = readSensors(); // get updated sensor readings

    // this is the overall control function. currently, it only uses a P term (no I or D).
    u = (P*e);
  
    // we run the robot and change its behavior based on the control term
    runBot(u); 
  
    // NOTE: currently we're not building in any delay. let's see what happens with that.
    // will most likely need to build some in later. also, we potentially want to make it stop
    // in between loops just so it's vaguely controllable/we get a good reading. 
  } else {
    stopBot();
  }
  
}

// reads both sensor values and returns the difference of them as is appropriate for the error
double readSensors() {
  double r = digitalRead(rightSensor);
  double l = digitalRead(leftSensor);
  return r-l;
}

// sets robot state to start so it'll start running the code
void startBot() {
  start = true;
  Serial.println("started");
}

// stops robot and changes run state to false
void stopBot() {
  right->run(RELEASE);
  left->run(RELEASE);
  start = false;
}

// runs robot for the period of one loop iteration based on the control term (u)
// desired behavior: if u is close to 0 it drives straight. if it's positive it
// should start turning right, if it's negative turn left.
void runBot(double u) {
  // general idea is to increase the speed of the wheel that we want on the outside
  // of the turn and decrease the speed of the other based on u. the following should
  // take direction into account since u retains its sign.
  right->setSpeed(baseSpeed+u); // add u to right wheel
  left->setSpeed(baseSpeed-u); // subtract u from left wheel

  right->run(FORWARD);
  left->run(FORWARD);
}

// method to change P for serial control
void setP(double newP) {
  P = newP;
}

// method to change base speed for serial control
void setBaseSpeed(int newSpeed) {
  baseSpeed = newSpeed;
}

// method to print out values for serial control (and sanity)
void printVals() {
  Serial.println("P: "); Serial.print(P);
  Serial.println("I: "); Serial.print(I);
  Serial.println("D: "); Serial.print(D);
  Serial.println("Base Speed: "); Serial.print(baseSpeed);
}
