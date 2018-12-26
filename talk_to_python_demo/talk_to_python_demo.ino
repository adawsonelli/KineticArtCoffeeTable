void setup()
{
   Serial.begin(115200);           // set up Serial library at 9600 bps
   Serial.println("Stepper test!");
   //
   // AFMS.begin();  // create with the default frequency 1.6KHz
   // //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
   // Astepper1.setMaxSpeed(200.0);
   // Astepper1.setAcceleration(100.0);
   // Astepper1.moveTo(324);
   //Astepper1.setSpeed(80);
}

// bruhhhhhhhhhhhhh

void loop()
{

Serial.println("hello world");
delay(1000);
// Astepper1.runSpeed();

}
