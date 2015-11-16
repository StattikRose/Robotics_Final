#include <Sparki.h> // include the sparki library

int finished = 1;
int dropoff = 0;
unsigned int dist = 10;

void setup() 
{
  sparki.gripperOpen(6);
}

void loop() {
  int threshold = 700;
  
  int lineLeft   = sparki.lineLeft();   // measure the left IR sensor
  int lineCenter = sparki.lineCenter(); // measure the center IR sensor
  int lineRight  = sparki.lineRight();  // measure the right IR sensor

  if ( lineCenter < threshold ) // if line is below left line sensor
  {  
    sparki.moveForward(); // move forward
  }
  else{
    if ( lineLeft < threshold ) // if line is below left line sensor
    {  
      sparki.moveLeft(); // turn left
    }
  
    if ( lineRight < threshold ) // if line is below right line sensor
    {  
      sparki.moveRight(); // turn right
    }
  }
  if (lineLeft < threshold && lineCenter < threshold && lineRight < threshold){
    dropoff = 1;
  } 

  sparki.clearLCD(); // wipe the screen
  
  //sparki.print("Line Left: "); // show left line sensor on screen
  //sparki.println(lineLeft);
  
  //sparki.print("Line Center: "); // show center line sensor on screen
  //sparki.println(lineCenter);
  
  //sparki.print("Line Right: "); // show right line sensor on screen
  //sparki.println(lineRight);
  
  sparki.updateLCD(); // display all of the information written to the screen

  sparki.clearLCD();
    
  int cm = sparki.ping(); // measures the distance with Sparki's eyes
    
  sparki.print("Distance: "); 
  sparki.print(cm); // tells the distance to the computer
  sparki.println(" cm"); 
  sparki.updateLCD();
  if(finished == 1){
    if(cm != -1) // make sure its not too close or too far
    { 
        if(cm < 5) // if the distance measured is less than 2 centimeters
        {
            sparki.beep(); // beep!
            sparki.moveForward(3);
            sparki.gripperClose();
            //sparki.moveStop();
            delay(6000);
            sparki.gripperStop();
            finished = 0;
            sparki.moveLeft(180);
        }
    }
  }
  if(dropoff == 1){
     sparki.moveForward(dist);
     sparki.gripperOpen();
     delay(6000);
     sparki.gripperStop();
     sparki.moveBackward(dist+3);
     sparki.moveLeft(180);
     dropoff = 0; 
     dist = dist-5;
     finished = 1;
  }
  delay(100); // wait 0.1 seconds
}
