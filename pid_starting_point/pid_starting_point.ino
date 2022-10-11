// basic attempt at PID stuff
const uint8_t sensor = 9; // assuming we have one sensor reading and that it's coming from pin 9
  // in reality, this has to be figured out and changed later, especially given that we have two sensors.

double SP = 100; // SP is the desired setpoint -- this has to be set to whatever the sensor value is for the ground w/o tape
double PV = 100; // reads the sensor data
double e = SP-PV; // error value, which is SP-PV and should be calculated continuously
double u = 0; // control variable -- the tuning of P, I, and D are going to come together in a
// weighted sum to genearte a control variable that we can then use to adjust the turning

// for now, I'm setting it to just be a P controller (so I and D are 0). This is for simplicity 
// while we try to understand it. We can add them in later as we see fit.
double P = 1; // proportional to current value of error -- must be tuned
double I = 0; // integrates past values of error over time to eliminate cumulative error based on history
double D = 0; // estimates future trend of error based on current rate of change and attempts to damp large changes in error

void setup() {

  Serial.begin(9600);

  pinMode(sensor, INPUT);

  t = millis(); // keep track of time from start
  
}

void loop() {
  PV = analogRead(sensor); // get an updated sensor reading
  e = SV - PV; // calculate a new error value based on that reading

// this is a stand-in fr the overall control function. in reality, the I and D terms would be done a little differently.
  u = (P*e) + (I*e) + (D*e); 

// still need to figure out how we want to implement this, but effectively we change
// the behavior of the robot based on the u term, and in this case the behavior we're looking
// to change is the turning of the robot.
  turn(u); 
  
}
