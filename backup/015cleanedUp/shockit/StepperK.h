/*
 * StepperK.h - stepper library for modern steppers with drivers
 * Limit switches functionality added
 * loosely based on
 * Stepper.h - Stepper library for Wiring/Arduino - Version 1.1.0
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 */

// ensure this library description is only included once
#ifndef ARRMAXLENGTH
#define ARRMAXLENGTH 1200 // maximum size of the double precision array Arduino Mega
// can handle
#endif
#ifndef StepperK_h
#define StepperK_h

// library interface description
class StepperK
{
public:
  // constructors:
  StepperK(int number_of_steps, int pin_PU, int pin_DR,
           int pin_LS_CW, int pin_LS_CCW, int pin_MF, int steps_per_rev);

  // speed setter method:
  void setSpeed(long whatSpeed);

  // method to set the number of steps the motor accelerates:
  void setStepsToAccelerate(long finSpeed, long no_steps_acc);

  // mover method:
  void step(int number_of_steps);

  // mover method with acceleration:
  void accel_and_jog(int steps_to_move);

  int version(void);

private:
  void stepController(int dir); // in case an external controller takes
  // care of the step sequence

  int direction;            // Direction of rotation
  unsigned long step_delay; // delay between steps, in us, based on speed
                            //(or terminal speed)
  int number_of_steps;      // total number of steps this motor can take
  int steps_per_rev;        // steps per revolution (should coinside )
                            // with the external motor settings
  int steps_to_accelerate;  // number of steps the motor accelerates.
                            // should be < number_of_steps;
  int step_number;          // which step the motor is on
  int pin_count;
  unsigned long arr_delays[ARRMAXLENGTH];

  // motor pin numbers:
  int pin_PU;
  int pin_DR;
  int pin_LS_CW;
  int pin_LS_CCW;
  int pin_MF;

  unsigned long last_step_time; // timestamp in us of when the last step was taken
};

#endif
