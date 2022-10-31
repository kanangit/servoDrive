#include "Arduino.h"
#include "StepperK.h"

/*
 * two-wire constructor.
 * Sets which wires should control the motor.
 */
StepperK::StepperK(int number_of_steps, int pin_PU, int pin_DR,
                   int pin_LS_CW, int pin_LS_CCW)
{
  this->step_number = 0;                   // which step the motor is on
  this->direction = 0;                     // motor direction
  this->last_step_time = 0;                // timestamp in us of the last step taken
  this->number_of_steps = number_of_steps; // total number of steps for this motor

  // Arduino pins for the motor control connection:
  this->pin_PU = pin_PU;
  this->pin_DR = pin_DR;

  // Arduino pins for limit swithches
  /* clocwise (when looking at the tip of the shaft of the motor)
   is considered postitive directinion */
  this->pin_LS_CW = pin_LS_CW;
  this->pin_LS_CCW = pin_LS_CCW;

  // setup the pins on the microcontroller:
  pinMode(this->pin_PU, OUTPUT);
  pinMode(this->pin_DR, OUTPUT);
  pinMode(pin_LS_CW, INPUT_PULLUP);
  pinMode(pin_LS_CCW, INPUT_PULLUP);
  pin_count = 2;
}

/*
 * Sets the speed in revs per minute
 */
void StepperK::setSpeed(long whatSpeed)
{
  this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / whatSpeed;
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void StepperK::step(int steps_to_move)
{
  int steps_left = abs(steps_to_move); // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0)
  {
    this->direction = 1;
  }
  if (steps_to_move < 0)
  {
    this->direction = 0;
  }

  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
    unsigned long now = micros();
    // move only if the appropriate delay has passed:
    if (now - this->last_step_time >= this->step_delay)
    {
      // get the timeStamp of when you stepped:
      this->last_step_time = now;
      // increment or decrement the step number,
      // depending on direction:
      if (this->direction == 1)
      {
        this->step_number++;
        if (this->step_number == this->number_of_steps)
        {
          this->step_number = 0;
        }
      }
      else
      {
        if (this->step_number == 0)
        {
          this->step_number = this->number_of_steps;
        }
        this->step_number--;
      }
      // decrement the steps left:
      steps_left--;
      // tell the driver to make one step:
      stepController(direction);
    }
  }
}

/*
 * Moves the motor forward or backwards.
 */

void StepperK::stepController(int dir)
{
  int state_LS_CW = HIGH;
  int state_LS_CCW = HIGH;
  digitalWrite(pin_PU, HIGH);
  if (dir == 0)
  {
    if (digitalRead(pin_LS_CCW) == LOW)
    {
      digitalWrite(pin_DR, LOW);
      digitalWrite(pin_PU, LOW);
    }
  }
  else
  {
    if (digitalRead(pin_LS_CW) == LOW)
    {
      digitalWrite(pin_DR, HIGH);
      digitalWrite(pin_PU, LOW);
    }
  }
}

/*
  version() returns the version of the library:
*/
int StepperK::version(void)
{
  return 20;
}
