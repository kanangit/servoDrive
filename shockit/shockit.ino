/*

shockit - Arduino code for moving the translation stage
for the shock experiments

last changes log:
2505.07.29 editing for acceleration pac pac
2022.12.18 Kananovich. (kananovicha@appstate.edu) Now the motor is starting when
                        trigPin is HIGW
2022.11.04 Kananovich. (kananovicha@appstate.edu)
 */

#include "StepperK.h"
#include <math.h>

//---------------------------------------------------------------------------------------------
const int stepsPerRevolution = 50; // change this to fit the number of steps per revolution
// for your motor (which is set up in MotionStudio.exe). Usually don't have to change
//----------------------------------------------------------------------------------------------


// travel distance in steps. Positive number will move the translation stage forward,
// negative - backward
// each step is 0.1 mm travel
int steps_travel = stepsPerRevolution *2;

// speed in RPM: maximum limit is 3000
int speed_rpm = 100;

// number of steps motor accelerates to the speed: don't go below 5
int steps_to_accel = 50; // max acceleration 100 step corresponds to one revolution.

int steps_rectilin = 250; // number of steps the piston moves with constant speed speed_rpm after the first aceeleration stage
                         // (and before the second acceleration stage

double acceleration2 = 6.0; // m/s^2 the acceleration of the moving piston as it hits the particles cloud

int steps_to_accel_2 = 250; // number of steps the piston accelerates as it hits the particles 



//-------------------------------------------------------------------------------------------------
//more permanent parameters (usually don't need to be changed):
//-------------------------------------------------------------------------------------------------
int trigPinValue;
// define the trigger pin
const int trigPin = 33;

double lead = 0.010;         //the linear distance (in meters) the translation stage travels in one revolution of the screw
                            //(corresponding to one revolution of the motor shaft.

// initialize the StepperK library:
StepperK myStepper(steps_travel, 25, 29, 51, 47, 12, stepsPerRevolution);


void setup()
{
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // set up the trig pin:
  pinMode(trigPin, INPUT);

  pinMode(12, OUTPUT); // for testing
  digitalWrite(12, HIGH);

  // set the speed at speed_rpm rpm, with steps_to_accel steps to accelerate:
  //myStepper.setStepsToAccelerate(speed_rpm, steps_to_accel);
  myStepper.setStepsToAccelerateAgain(speed_rpm, steps_to_accel, steps_rectilin, acceleration2 / lead / 2.0 / M_PI, steps_to_accel_2);

}

void loop()
{
  // inject only if the trigger signal is received.
  //trigger signal is when the input on the BNC goes from 5 V to 0 V.
trigPinValue = digitalRead(trigPin);
   if (trigPinValue == HIGH)
   {
    Serial.println(trigPinValue);
     myStepper.accel_and_jog(-steps_travel);
     // myStepper.setStepsToAccelerate(60, steps_to_accel);  // optional comment for enabling the forward and backward motion
    // myStepper.accel_and_jog(steps_travel);      
   }

}
