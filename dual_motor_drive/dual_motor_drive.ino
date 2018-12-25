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
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(400, 1);

// AccelStepper Code
// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
void forwardstep1() {
  myMotor->onestep(FORWARD, DOUBLE);
}

void backwardstep1() {
  myMotor->onestep(BACKWARD, DOUBLE);
}

void forwardstep2() {
  myMotor2->onestep(FORWARD, DOUBLE);
}

void backwardstep2() {
  myMotor2->onestep(BACKWARD, DOUBLE);
}

AccelStepper Astepper1(forwardstep1, backwardstep1); // Alternate constructor


void setup()
{
   Serial.begin(115200);           // set up Serial library at 9600 bps
   Serial.println("Stepper test!");
   //
    AFMS.begin();  // create with the default frequency 1.6KHz
   // //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
    Astepper1.setMaxSpeed(500.0);
    Astepper1.setAcceleration(100.0);
   // Astepper1.moveTo(324);
   Astepper1.setSpeed(400);
   Astepper1.setCurrentPosition(0);
}

// bruhhhhhhhhhhhhh
unsigned int integerValue=0;  // Max value is 65535
char incomingByte;

void loop()
{
  if (Serial.available()>0) {
    // parse the incoming bytes and assemble into integers
    integerValue = 0; 
    while(1) { 
      // force into a loop until 'n' is received
      incomingByte = Serial.read();
      Serial.println(incomingByte);
      if (incomingByte == '\n') break;   // exit the while(1), we're done receiving
      if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1
      integerValue *= 10;  // shift left 1 decimal place
      // convert ASCII to integer, add, and shift left 1 decimal place
      integerValue = ((incomingByte - 48) + integerValue);
    }
    Serial.println(integerValue);   // Do something with the value
//    Serial.println(ch);
//    int val = (int) ch;
//    Serial.println(val);
    if (integerValue > 500){
      // move 400 steps positive
      while (Astepper1.currentPosition() != -400){
        Astepper1.setSpeed(-400);
        Astepper1.run();
      }
      Serial.println('a');
      Astepper1.setCurrentPosition(0);
//      Serial.println("hello world");
} if (integerValue < 500) {
      while (Astepper1.currentPosition() != 400){
        Astepper1.setSpeed(400);
        Astepper1.run();
      }
      Astepper1.setCurrentPosition(0);
      Serial.println('a');
//      Serial.println("goodbye world");
    }
  }
  // delay(1000);
// Astepper1.runSpeed();

}
