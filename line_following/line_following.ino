// keeps track of sensor pins
const uint8_t rightSensor = 0;
const uint8_t leftSensor = 1;

// time variables
unsigned long t; 
const int interval = 3000; // interval between checks to make sure it's still on/near the line
unsigned int lastTime; // keeps track of the last time it checked to make sure it was on the line

// PID variables
// our error is the difference between the left and right wheels, so we'll have a small number if they're both off the tape
// and a much larger error if one is on and one is off (we'd expect something around 15 or 20 given inital calibration data).
double e = rightSensor - leftSensor;
// cumulative error for use with the I term
double cumError;
// rate of change of the error for use with the D term
double rateError;

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

int baseSpeed = 25;

// boolean to keep track of whether we want the robot to run or not so we can start and stop at will
bool start = false;

// other necessary variables
String cmd = "";
String command = "";
String value = "";

void setup() {

  Serial.begin(9600);
  Serial.print("connected--what would you like to do? Options: start, stop, P, speed, print, clear");

  pinMode(rightSensor, INPUT);
  pinMode(leftSensor, INPUT);

  AFMS.begin();

  // speeds range from 0 (stopped) to 255 (full speed)
  right->setSpeed(baseSpeed);
  left->setSpeed(baseSpeed);

  t = millis(); // keep track of time from start
  
}

void loop() {

  // NOTE: need to  figure out how this is intaking serial commands
  while (Serial.available() > 0) {

    char incoming = Serial.read(); // reads one character from the buffer

    if (command.compareTo("P")==0 || command.compareTo("speed")==0) { // if the command requires an additional value, put future serial inputs into value instead of command
      value.concat(incoming);
    } else { // otherwise keep adding serial inputs to command
      command.concat(incoming);
    }
    
    
    if (command.compareTo("start")==0) {
      startBot(); // call the start function
      command = ""; // reset command for next time
    } else if (command.compareTo("stop")==0) {
      stopBot(); // call the stop function
      command = ""; // reset command for next time
    } else if (command.compareTo("print")==0) {
      printVals(); // call the function to print the values of all the variables
      command = "";
    } else if (command.compareTo("P")==0) {
      Serial.println("enter value, terminate with semicolon");
      if (value.indexOf(";")!=-1) {
        value[value.length()-1] = "\0";
        setP(value.toDouble());
        command = "";
        value = "";
      }
    } else if (command.compareTo("speed")==0) {
      Serial.println("enter value, terminate with semicolon");
      if (value.indexOf(";")!=-1) {
        value[value.length()-1] = "\0";
        setBaseSpeed(value.toDouble());
        command = "";
        value = "";
      }
    } else if (command.indexOf("clear")!=-1) { // if one wants to clear whatever is in the buffer currently
      command = "";
      value = "";
      Serial.println("cleared");
    }
  }

  if (start) { // if we're in the start state
    
    e = readSensors(); // get updated sensor readings
    Serial.print(t); Serial.print(", "); Serial.print(e); Serial.print(", "); // print time and sensor readings to serial port

    // this is the overall control function. currently, it only uses a P term (no I or D).
    u = (P*e)+(I*cumError)+(D*rateError);
  
    // we run the robot and change its behavior based on the control term
    runBot(u); 
  
    // NOTE: currently we're not building in any delay. let's see what happens with that.
    // will most likely need to build some in later. also, we potentially want to make it stop
    // in between loops just so it's vaguely controllable/we get a good reading. 

    if (t >= lastTime + interval) {
      if (!checkLine()){
        stopBot(); // the robot is no longer near the line
        Serial.println("offline");
      }

      lastTime = t; // resets lastTime
    }
  } else {
    stopBot();
  }
  
}

// reads both sensor values and returns the difference of them as is appropriate for the error
double readSensors() {
  double r = analogRead(rightSensor);
  double l = analogRead(leftSensor);
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

  if (u==0) { // run straight
    right->setSpeed(baseSpeed);
    left->setSpeed(baseSpeed);
    right->run(FORWARD);
    left->run(FORWARD);
    Serial.print(baseSpeed); Serial.print(", "); Serial.print(baseSpeed); Serial.println(", "); // prints left and right motor speeds
  } else if (u>0) { // we want to turn right
    right->run(RELEASE);
    left->setSpeed(baseSpeed);
    left->run(FORWARD);
    Serial.print(baseSpeed); Serial.print(", "); Serial.println(0); Serial.println(", ");
  } else { // turn left
    right->setSpeed(baseSpeed);
    left->run(RELEASE);
    right->run(FORWARD);
    Serial.print(0); Serial.println(", "); Serial.println(baseSpeed); Serial.println(", ");
  }
  
}

// method to change P for serial control
void setP(double newP) {
  P = newP;
  Serial.print("P: "); Serial.println(P);
}

// method to change base speed for serial control
void setBaseSpeed(int newSpeed) {
  baseSpeed = newSpeed;
}

// method to print out values for serial control (and sanity)
void printVals() {
  Serial.println("");
  Serial.print("P: "); Serial.println(P);
  Serial.print("I: "); Serial.println(I);
  Serial.print("D: "); Serial.println(D);
  Serial.print("Base Speed: "); Serial.println(baseSpeed);
}

// the robot wiggles slightly to make sure that it's still centered over or at least near the line
// will return true if it's fine, false if not
bool checkLine() {
  double e = readSensors();

  // turn one direction for a bit and if it sees the tape at any point during that, return true
  // the end condition of i is how we control how long it turns for
  for (double u = -1; u <= 2; u+=3) { // u will equal -1, then 2, and then this should end
    for (int i=0; i < 500; i++) {
      // if it's seeing the line, return true
      if (e > 5) {
        return true;
      }
    
      runBot(u); // turns to the right, then to the left
      e = readSensors();
    }
  }

  return false;
  
}
