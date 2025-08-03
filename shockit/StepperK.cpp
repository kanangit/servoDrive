#include "Arduino.h"
#include "StepperK.h"
#include <math.h>

/*
 * two-wire constructor.
 * Sets which wires should control the motor.
 */
StepperK::StepperK(int number_of_steps, int pin_PU, int pin_DR,
                   int pin_LS_CW, int pin_LS_CCW, int pin_MF, int steps_per_rev)
{

  this->lead = 0.010;                      //lead of the translation stage lead screw
  this->step_number = 0;                   // which step the motor is on
  this->direction = 0;                     // motor direction
  this->last_step_time = 0;                // timestamp in us of the last step taken
  this->number_of_steps = number_of_steps; // total number of steps for this motor
  this->steps_per_rev = steps_per_rev;     // steps per revolution (should coinside )
                                           // with the external motor settings
  this->steps_to_accelerate = 50;

  // Arduino pins for the motor control connection:
  this->pin_PU = pin_PU;
  this->pin_DR = pin_DR;
  this->pin_MF = pin_MF;

  // Arduino pins for limit swithches
  /* clocwise (when looking at the tip of the shaft of the motor)
   is considered postitive directinion */
  this->pin_LS_CW = pin_LS_CW;
  this->pin_LS_CCW = pin_LS_CCW;

  // setup the pins on the microcontroller:
  pinMode(this->pin_PU, OUTPUT);
  pinMode(this->pin_DR, OUTPUT);
  pinMode(this->pin_MF, OUTPUT);
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
 * Sets the number of steps motor takes to accelerate to the speed
 */
void StepperK::setStepsToAccelerate(long finSpeed, long no_steps_acc)
{
  double currentDeltaT, acceleration, deltaPhi, deltaTfinal, totalTime;

  // the angle the motor turns per one step:
  deltaPhi = 2.0 * M_PI / double(this->steps_per_rev);
  // the time delay between steps after the motor finishes
  // the acceleration stage:
  deltaTfinal = 60.0 / double(this->steps_per_rev) / double(finSpeed);
  // acceleration
  acceleration = deltaPhi / 2.0 / double(no_steps_acc) / deltaTfinal / deltaTfinal;
  for (int i = 0; i < no_steps_acc; i++)
  {
    this->arr_delays[i] = round(1000000.0 * sqrt(deltaPhi / 2.0 / acceleration / double(i + 1)));
   
//    Serial.print(i);
//    Serial.print("  ");
//    Serial.print(arr_delays[i]);
//    Serial.println();
  }
  for (int i = no_steps_acc; i < ARRMAXLENGTH; i++)
  {
    this->arr_delays[i] = round(1000000.0 * deltaTfinal);
//    Serial.print(i);
//    Serial.print("  ");
//    Serial.print(arr_delays[i]);
//    Serial.println();
  }
}


/*
 * Sets the number of steps motor takes to accelerate to the speed
 */
void StepperK::setStepsToAccelerateAgain(long finSpeed, long no_steps_acc, long no_steps_rectilin, long alpha2, long no_steps_acc2)
{
  double currentDeltaT, acceleration, deltaPhi, deltaTfinal;
  double prevTheta, prevOmega, nextDeltaT, nextTheta, nextOmega;
  double alpha1, omegaFinal1, thetaRectil;
  double nextThetaTheor;
  double alpha_break;
  double totalTime;

  totalTime = 0;

  alpha_break = -20.0 / lead / M_PI;

  // the angle the motor turns per one step:
  deltaPhi = double(2.0 * M_PI / double(this->steps_per_rev));
  // total angle the motor will turn at the end of the first acceleration stage:
  thetaRectil = double(no_steps_acc) * deltaPhi;
  // the time delay between steps after the motor finishes
  // the first acceleration stage:
  deltaTfinal = double(60.0) / double(this->steps_per_rev) / double(finSpeed);


  
  // angular velocity at the end of the first acceleration stage:
  omegaFinal1 = double(2.0 * M_PI) / deltaTfinal / double(this->steps_per_rev);

  // angular acceleration to accelerate from standstill to constant velocity
  alpha1 = omegaFinal1 * omegaFinal1 / double(2.0) / thetaRectil;
  //deleta variable acceleration later:
  //acceleration = deltaPhi / 2.0 / double(no_steps_acc) / deltaTfinal / deltaTfinal;


//    Serial.print("  ");
//    Serial.print("deltaTfinal = ");
//    Serial.print(deltaTfinal);
//    Serial.print(" ");    
//    Serial.println("--------------------------------------- ");

//    Serial.print("  ");
//    Serial.print("omegaFinal1 = ");
//    Serial.print(omegaFinal1);
//    Serial.print(" ");    
//    Serial.println("--------------------------------------- ");
  
  prevTheta = 0;
  prevOmega = 0;
  for (int i = 0; i < no_steps_acc; i++)
  {
    nextOmega = sqrt(prevOmega * prevOmega  + double(2.0) * alpha1 * deltaPhi);
    nextDeltaT = (nextOmega - prevOmega) / alpha1;
    nextTheta = prevTheta + deltaPhi;

    nextThetaTheor = prevTheta + prevOmega * nextDeltaT + alpha1 * nextDeltaT * nextDeltaT / 2.0;
    
    this->arr_delays[i] = round(1000000.0 * nextDeltaT);
    totalTime = totalTime + double(arr_delays[i]) / 1000000.0;
    Serial.print(i);

 //   Serial.print("  ");
 //   Serial.print("nextOmega = ");
 //   Serial.print(nextOmega);
 //   Serial.print(" ");

 //   Serial.print("  ");
 //   Serial.print("nextTheta = ");
 //   Serial.print(nextTheta);
 //   Serial.print(" ");

    Serial.print("  ");
    Serial.print("step delay = ");
    Serial.print(arr_delays[i]);
    Serial.print("   ");


      Serial.print("  ");
      Serial.print("totalTime = ");
      Serial.print(totalTime);
      Serial.print(" ");
      Serial.println();
    prevTheta = nextTheta;
    prevOmega = nextOmega;
  }
 // Serial.println("rectilinear motion start");
 // Serial.println("--------------------------------------");
  for (int i = no_steps_acc; i < (no_steps_acc + no_steps_rectilin); i++)
  {
    nextOmega = prevOmega;
    nextDeltaT = nextDeltaT;
    nextTheta = prevTheta + deltaPhi;
    nextThetaTheor = prevTheta + prevOmega * nextDeltaT;
    //this->arr_delays[i] = round(1000000.0 * deltaTfinal);
    this->arr_delays[i] = round(1000000.0 * nextDeltaT);//this has an error ???
    totalTime = totalTime + double(arr_delays[i]) / 1000000.0;
    
   Serial.print(i);

//    Serial.print("  ");
//    Serial.print("nextOmega = ");
//    Serial.print(nextOmega);
//    Serial.print(" ");

//    Serial.print("  ");
//    Serial.print("nextTheta = ");
//    Serial.print(nextTheta);
//    Serial.print(" ");

    Serial.print("  ");
    Serial.print("step delay = ");
    Serial.print(arr_delays[i]);
    Serial.print("   ");


      Serial.print("  ");
      Serial.print("totalTime = ");
      Serial.print(totalTime);
      Serial.print(" ");
      Serial.println();
    
    prevTheta = nextTheta;
    prevOmega = nextOmega;
  }

//   Serial.println("Second acceleration motion start");
//  Serial.println("--------------------------------------");
  for (int i = (no_steps_rectilin + no_steps_acc); i < (no_steps_rectilin + no_steps_acc + no_steps_acc2); i++)
  {
    nextOmega = sqrt(prevOmega * prevOmega  + double(2.0) * alpha2 * deltaPhi);
    nextDeltaT = (nextOmega - prevOmega) / alpha2;
    nextTheta = prevTheta + deltaPhi;

nextThetaTheor = prevTheta + prevOmega * nextDeltaT + alpha2 * nextDeltaT * nextDeltaT / 2.0;
    
    this->arr_delays[i] = round(1000000.0 * nextDeltaT);
    totalTime = totalTime + double(arr_delays[i]) / 1000000.0;
    Serial.print(i);

//    Serial.print("  ");
//    Serial.print("nextOmega = ");
//    Serial.print(nextOmega);
//    Serial.print(" ");

//    Serial.print("  ");
//    Serial.print("nextTheta = ");
//    Serial.print(nextTheta);
//    Serial.print(" ");

    Serial.print("  ");
    Serial.print("step delay = ");
    Serial.print(arr_delays[i]);
    Serial.print("   ");


      Serial.print("  ");
      Serial.print("totalTime = ");
      Serial.print(totalTime);
      Serial.print(" ");
      Serial.println();
    prevTheta = nextTheta;
    prevOmega = nextOmega;
  }

//    Serial.println("breaking start");
//  Serial.println("--------------------------------------");
  for (int i = (no_steps_rectilin + no_steps_acc + no_steps_acc2); i < (no_steps_rectilin + no_steps_acc + no_steps_acc2 +100); i++)
  {
    nextOmega = sqrt(prevOmega * prevOmega  + double(2.0) * alpha_break * deltaPhi);
        if (nextOmega > 0) {
      nextDeltaT = (nextOmega - prevOmega) / alpha_break;
      //Serial.println(nextOmega);
    }
    if (prevOmega * prevOmega <= - double(2.0) * alpha_break * deltaPhi) {
      nextOmega = prevOmega/2 ;
      nextDeltaT = (this->arr_delays[i - 1]) * 2.0 / 1000000.0;
      //Serial.println(nextOmega);
    }
      if (nextOmega <= 0) {
      nextDeltaT = 100000.0;
      nextOmega = 2.0 * M_PI / nextDeltaT;
        //Serial.println(nextOmega);
    }


    nextTheta = prevTheta + deltaPhi;

nextThetaTheor = prevTheta + prevOmega * nextDeltaT + alpha2 * nextDeltaT * nextDeltaT / 2.0;
    
    this->arr_delays[i] = round(1000000.0 * nextDeltaT);
//    Serial.print(i);

//    Serial.print("  ");
//    Serial.print("nextOmega = ");
//    Serial.print(nextOmega);
//    Serial.print(" ");

//    Serial.print("  ");
//    Serial.print("nextTheta = ");
//    Serial.print(nextTheta);
//    Serial.print(" ");

//    Serial.print("  ");
//    Serial.print("nextThetaTheor = ");
//    Serial.print(nextThetaTheor);
//    Serial.print(" ");

//    Serial.print(arr_delays[i]);
//    Serial.println();
    prevTheta = nextTheta;
    prevOmega = nextOmega;
  }
}


void StepperK::accel_and_jog(int steps_to_move)
{
  int steps_left = abs(steps_to_move); // how many steps to take
  digitalWrite(pin_MF, HIGH);          // activate the motor

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0)
  {
    this->direction = 1;
  }
  if (steps_to_move < 0)
  {
    this->direction = 0;
  }
  int i_delay = 0;
  unsigned long step_v_delay = 500000L;
  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
    unsigned long now = micros();
    step_v_delay = arr_delays[i_delay];
    // move only if the appropriate delay has passed:
    if (now - this->last_step_time >= step_v_delay)
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
      // next delay time from the array:
      if (i_delay < (ARRMAXLENGTH - 1))
      {
        i_delay++;
      }

      // tell the driver to make one step:
      stepController(direction);
    }
  }
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void StepperK::step(int steps_to_move)
{
  int steps_left = abs(steps_to_move); // how many steps to take
  digitalWrite(pin_MF, HIGH);          // activate the motor

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
  return 21;
}
