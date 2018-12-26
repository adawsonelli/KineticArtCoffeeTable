void setup()
{
   Serial.begin(115200);           // set up Serial library at 9600 bps
//   Serial.println("Stepper test!");
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
if (Serial.available()) {
    char ch = Serial.read();
    if (ch == '1'){
      // move 400 steps positive
      while (Astepper1.currentPosition() != -400){
        Astepper1.setSpeed(-400);
        Astepper1.run();
      }
      Serial.println('a');
      Astepper1.setCurrentPosition(0);
//      Serial.println("hello world");
    } if (ch == '2') {
      while (Astepper1.currentPosition() != 400){
        Astepper1.setSpeed(400);
        Astepper1.run();
      }
      Astepper1.setCurrentPosition(0);
      Serial.println('a');
//      Serial.println("goodbye world");
    }
  }
}
