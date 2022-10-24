/*

testing the new class StepperK
scukko
 */

#include "StepperK.h"

const int stepsPerRevolution = 800;  // change this to fit the number of steps per revolution
// for your motor

// initialize the StepperK library:
StepperK myStepper(stepsPerRevolution, 9, 8, 10, 11, 12);

//define the trigger pin
const int trigPin = 7;

int steps_travel = stepsPerRevolution * 28;

void setup() {
  //set up the trig pin:
  pinMode(trigPin, INPUT_PULLUP);
  // set the speed at xx rpm:
  myStepper.setSpeed(80);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
// inject only if the trigger signal is received:

    if (digitalRead(trigPin) == LOW)
    {
      Serial.println("counterclockwise");
      myStepper.step(steps_travel);
      delay(500);
    }



}
