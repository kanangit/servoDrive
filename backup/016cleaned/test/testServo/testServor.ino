/*

testing the new class StepperK
scukko
 */

#include "StepperK.h"

const int stepsPerRevolution = 800;  // change this to fit the number of steps per revolution
// for your motor

// initialize the Stepper library on pins 9 and 8:
StepperK myStepper(stepsPerRevolution, 9, 8, 10, 11);

void setup() {
  // set the speed at xx rpm:
  myStepper.setSpeed(10);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  // step one revolution in one direction (forward):
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution * 2);
  delay(500);

  // step one revolution in the other direction:
  //Serial.println("counterclockwise");
  //myStepper.step(-stepsPerRevolution * 2);
 // delay(500);
}
