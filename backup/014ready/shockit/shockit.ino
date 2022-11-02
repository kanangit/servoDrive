/*

testing the new class StepperK
scukko
 */

#include "StepperK.h"

// define the trigger pin
const int trigPin = 33;

const int stepsPerRevolution = 100; // change this to fit the number of steps per revolution
// for your motor

// travel distance in steps:
int steps_travel = stepsPerRevolution *19;

// speed in RPM:
int speed_rpm = 60;

// number of steps motor accelerates to the speed:
int steps_to_accel = 100;

// initialize the StepperK library:
StepperK myStepper(stepsPerRevolution, 25, 29, 51, 47, 12, stepsPerRevolution);


void setup()
{
  // set up the trig pin:
  // pinMode(trigPin, INPUT_PULLUP);

  pinMode(12, OUTPUT); // for testing

  digitalWrite(12, HIGH);

  // set the speed at xx rpm, with xxx steps to accelerate:
  myStepper.setStepsToAccelerate(speed_rpm, steps_to_accel);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop()
{
  // inject only if the trigger signal is received:

   if (digitalRead(trigPin) == LOW)
   {
     Serial.println(digitalRead(trigPin));
     myStepper.accel_and_jog(-steps_travel);
     
   }

//  Serial.println("counterclockwise");
//  myStepper.accel_and_jog(steps_travel);
//  delay(60000);
}
