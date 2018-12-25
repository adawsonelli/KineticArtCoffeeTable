#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(400, 2);

// AccelStepper Code
// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
void forwardstep1() {
  myMotor->onestep(FORWARD, DOUBLE);
}

void backwardstep1() {
  myMotor->onestep(BACKWARD, DOUBLE);
}

AccelStepper Astepper1(forwardstep1, backwardstep1); // Alternate constructor

void setup()
{
   Serial.begin(9600);           // set up Serial library at 9600 bps
   Serial.println("Stepper test!");

   AFMS.begin();  // create with the default frequency 1.6KHz
   //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
   Astepper1.setMaxSpeed(1000.0);
   Astepper1.setAcceleration(100.0);
   Astepper1.setSpeed(1000);
   //Astepper1.moveTo(324);
   //Astepper1.setSpeed(80);
}

// bruhhhhhhhhhhhhh

void loop()
{

Astepper1.runSpeed();
//delay(1);

}
