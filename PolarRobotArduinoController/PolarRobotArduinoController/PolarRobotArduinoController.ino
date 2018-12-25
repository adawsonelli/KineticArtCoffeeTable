#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

//setup adafruit stepper motors
int PPR = 400; //pulses per revolution
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
bool atDestination = false;
bool waitForPacket = true;
char rTarget = 0;
char thetaTarget = 0;

void setup()
{  
    //setup serial
     Serial.begin(115200);
     
    //init adafruit servo drivers
    AFMS.begin();  // create with the default frequency 1.6KHz

    //setup servos
    ArStepper.setMaxSpeed(300.0);
    ArStepper.setSpeed(1000);
    ArStepper.setAcceleration(100.0);
   
    AthetaStepper.setMaxSpeed(300.0);
    AthetaStepper.setSpeed(1000);
    AthetaStepper.setAcceleration(100.0);
  
}

void loop()
{
   
   //wait for new command to come in
   while (waitForPacket)
   {
      //wait for fixed number of packets to accumulate
      if(Serial.available() > 1) //2 chars
      {
        waitForPacket = false;
        //read in packet
        rTarget = Serial.read();
        thetaTarget = Serial.read();
        
        // set stepper destinations and velocities
        ArStepper.moveTo(rTarget);
        AthetaStepper.moveTo(thetaTarget);
      }
   }
   
    while( ~atDestination)
    {
      //continue to run
      ArStepper.run();
      AthetaStepper.run();

      //check if the servos are at their destinations
      atDestination = (ArStepper.distanceToGo() == 0) && (AthetaStepper.distanceToGo() == 0);
    }

    //send request for next destination
    Serial.write(0xFF); // write all good. 

    //reset flags
    waitForPacket = true;
    atDestination = false;
  
}
