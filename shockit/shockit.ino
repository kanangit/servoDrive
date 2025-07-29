/*

shockit - Arduino code for moving the translation stage
for the shock experiments

last changes log:
2505.07.29 editing for acceleration
2022.12.18 Kananovich. (kananovicha@appstate.edu) Now the motor is starting when
                        trigPin is HIGW
2022.11.04 Kananovich. (kananovicha@appstate.edu)
 */

#include "StepperK.h"

// define the trigger pin
const int trigPin = 33;

const int stepsPerRevolution = 100; // change this to fit the number of steps per revolution
// for your motor (which is set up in MotionStudio.exe)

// travel distance in steps. Positive number will move the translation stage forward,
// negative - backward
int steps_travel = -stepsPerRevolution *10;

// speed in RPM: maximum limit is 3000
int speed_rpm = 60;

// number of steps motor accelerates to the speed: don't go below 5
int steps_to_accel = 100; // max acceleration 100 step corresponds to one revolution. 

// initialize the StepperK library:
StepperK myStepper(steps_travel, 25, 29, 51, 47, 12, stepsPerRevolution);


void setup()
{
  // set up the trig pin:
  pinMode(trigPin, INPUT);

  pinMode(12, OUTPUT); // for testing
  digitalWrite(12, HIGH);

  // set the speed at speed_rpm rpm, with steps_to_accel steps to accelerate:
  myStepper.setStepsToAccelerate(speed_rpm, steps_to_accel);

}

void loop()
{
  // inject only if the trigger signal is received.
  //trigger signal is when the input on the BNC goes from 5 V to 0 V.

   if (digitalRead(trigPin) == HIGH)
   {
     myStepper.accel_and_jog(-steps_travel);
     // myStepper.setStepsToAccelerate(60, steps_to_accel);  // optional comment for enabling the forward and backward motion
    // myStepper.accel_and_jog(steps_travel);         
   }

}
