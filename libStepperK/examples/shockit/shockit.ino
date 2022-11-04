/*

shockit - Arduino code for moving the translation stage
for the shock experiments

last changes log:
2022.11.04 Kananovich. (kananovicha@appstate.edu)
 */

#include "StepperK.h"

// define the trigger pin
const int trigPin = 33;

const int stepsPerRevolution = 100; // change this to fit the number of steps per revolution
// for your motor (which is set up in MotionStudio.exe)

// travel distance in steps. Positive number will move the translation stage forward,
// negative - backward
int steps_travel = - stepsPerRevolution *19;

// speed in RPM:
int speed_rpm = 120;

// number of steps motor accelerates to the speed:
int steps_to_accel = 100;

// initialize the StepperK library:
StepperK myStepper(steps_travel, 25, 29, 51, 47, 12, stepsPerRevolution);


void setup()
{
  // set up the trig pin:
  pinMode(trigPin, INPUT_PULLUP);

  pinMode(12, OUTPUT); // for testing
  digitalWrite(12, HIGH);

  // set the speed at speed_rpm rpm, with steps_to_accel steps to accelerate:
  myStepper.setStepsToAccelerate(speed_rpm, steps_to_accel);

}

void loop()
{
  // inject only if the trigger signal is received.
  //trigger signal is when the input on the BNC goes from 5 V to 0 V.

   if (digitalRead(trigPin) == LOW)
   {
     myStepper.accel_and_jog(-steps_travel);     
   }

}
