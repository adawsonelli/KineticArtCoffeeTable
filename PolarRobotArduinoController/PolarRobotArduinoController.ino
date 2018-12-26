#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>


//-------------------------- initialization ------------------------------------
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

//setup adafruit stepper motors
int ppr = 400; //pulses per revolution
Adafruit_StepperMotor *thetaMotor = AFMS.getStepper(PPR, 1); //(M1 and M2)
Adafruit_StepperMotor *rMotor = AFMS.getStepper(PPR, 2);     //(M3 and M4)

//setup wrapper functions for forward and backwards use
void forwardstep1() {
  thetaMotor->onestep(FORWARD, DOUBLE);  // you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
}
void backwardstep1() {
  thetaMotor->onestep(BACKWARD, DOUBLE);
}
void forwardstep2() {
  rMotor->onestep(FORWARD, DOUBLE);  // you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
}
void backwardstep2() {
  rMotor->onestep(BACKWARD, DOUBLE);
}

//AccelStepper objects with Alternative Constructor
AccelStepper ArStepper(forwardstep1, backwardstep1); // Alternate constructor
AccelStepper AthetaStepper(forwardstep2, backwardstep2); // Alternate constructor


//state variables
int freq = 2;                          //2hz = .5sec period
int stepperVmx = 1000;                 //mx velocity of the stepper
bool atDestination = false;            //is r state at rTarget?
bool waitForPacket = true;             //are we waiting for packet?
ArStepper.setCurrentPosition(0);       //current location of r axis  [steps]
AthetaStepper.setCurrentPosition(0);   //current location theta axis [steps]
unsigned int rTarget = 0;              //desired location of r axis in global pulses [steps]
unsigned int thetaTarget = 0;          //desired location of the theta axis [steps]

//--------------------------- functions ----------------------------------------
int minAng(int target, int cpos)
/* return the min angle between 2 positions*/
{
  //assume input for target is always on [0,4000]
  int ppr = 4000; int upper = 2000 ; int lower = -2000
  int dpos = target - cpos;

  //perform dpos ROC
  if (dpos > upper){dpos  = dpos - ppr;}
  else if(dpos <= lower){dpos  = dpos + ppr;}

  //return dpos
  return dpos
}

void thetaROC()
/*perform ROC on the theta axis, so that its always a positive angle*/
{
  int ppr = 4000 ; cpos = AthetaStepper.CurrentPosition()
  if (cpos >=  ppr){AthetaStepper.setCurrentPosition(cpos - ppr);}
  if (cpos <= -ppr){AthetaStepper.setCurrentPosition(cpos + ppr);}
}

void setStepperSpeeds()
/*determine the velocity to command of each stepper. if either velocity value
 saturates, then make sure the velocity ratio is maintained*/
{
  //determine the velocities required for each axis to reach it's destination by next time step
  int rVel = (rTarget - ArStepper.currentPosition())*freq              //[dsteps /dt]
  int tVel = (minAng(thetaTarget,AthetaStepper.currentPosition()))*freq      //[dsteps/dt]

  //handle when either motor saturates, maintain speed ratio
  if (abs(rVel) > stepperVmx  || abs(tVel) > stepperVmx)
  {
      if (abs(rVel) >= abs(tVel))
      {
        ArStepper.setSpeed(stepperVmx);
        AthetaStepper.setSpeed(stepperVmx*tVel/abs(rVel));    //scale slower speed
      }

      else if(abs(rVel) < abs(tVel))
      {
        AthetaStepper.setSpeed(stepperVmx);
        ArStepper.setSpeed(stepperVmx * rVel / abs(tVel));   //scale slower speed
      }
  }

  //otherwise, set motor speeds right away
  else
  {
    ArStepper.setSpeed(rVel);
    AthetaStepper.setSpeed(rVel);
  }

}

//------------------------------ main ------------------------------------------
void setup()
{
    //setup serial
     Serial.begin(115200);

    //init adafruit servo drivers
    AFMS.begin();  // create with the default frequency 1.6KHz

    //setup servos
    ArStepper.setMaxSpeed(300.0);
    ArStepper.setSpeed(0);
    ArStepper.setAcceleration(100.0);

    AthetaStepper.setMaxSpeed(300.0);
    AthetaStepper.setSpeed(0);
    AthetaStepper.setAcceleration(100.0);

}

void loop()
{

   //wait for new command to come in
   while (waitForPacket)
   {
      //wait for fixed number of packets to accumulate
      if(Serial.available() > 4) //2 chars
      {
        //read in packet
        char rh = Serial.read();
        char rl = Serial.read();
        char thetah = Serial.read();
        char thetal = Serial.read();

        //read counts into an int
        rTarget = rh << 8 | rl
        thetaTarget = thetah << 8 | thetal

        Serial.println(rTarget,DEC);
        Serial.println(thetaTarget,DEC);

        // set stepper speeds
        setStepperSpeeds();

        //packet successfully transfered
        waitForPacket = false;
      }
   }

    while(~atDestination)
    {
      //rollover correction
      thetaROC()

      //check if each axis is at destination:
      bool rDest = ArStepper.currentPosition() != rTarget
      bool thetaDest = AthetaStepper.currentPosition() != thetaTarget

      //move one step
      if (rDest){ArStepper.run();}
      if (thetaDest){AthetaStepper.run();}

      //check if both servos are at their destinations
      atDestination = (rDest  && thetaDest);
    }

    //send request for next destination
    Serial.write(0xFF); // write all good.

    //reset flags
    waitForPacket = true;
    atDestination = false;

}
