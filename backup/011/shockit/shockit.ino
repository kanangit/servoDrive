/*

testing the new class StepperK
scukko
 */

#include "StepperK.h"

const int stepsPerRevolution = 100; // change this to fit the number of steps per revolution
// for your motor

// initialize the StepperK library:
StepperK myStepper(stepsPerRevolution, 25, 29, 51, 47, 12, stepsPerRevolution);

// define the trigger pin
const int trigPin = 33;

int steps_travel = stepsPerRevolution * 4;

void setup()
{
  // set up the trig pin:
  // pinMode(trigPin, INPUT_PULLUP);

  pinMode(12, OUTPUT); // for testing

  digitalWrite(12, HIGH);

  // set the speed at xx rpm, with xxx steps to accelerate:
  myStepper.setStepsToAccelerate(60, 100);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop()
{
  // inject only if the trigger signal is received:

  // if (digitalRead(trigPin) == LOW)
  // {
  //   Serial.println("counterclockwise");
  //   myStepper.step(steps_travel);
  //   delay(500);
  // }

  Serial.println("counterclockwise");
  myStepper.accel_and_jog(steps_travel);
  delay(5000);
}
