
#include <Sparki.h>
//#include <Wire.h>
//#include "SparkFunISL29125.h"
//#include <Wire.h>
//#include "SFE_ISL29125.h"

//we should not be including wire.h twice. does the order in which the headers are included matter? 
//need to put the libraries in our github repo so that everyone can access 

//Global variables
//Feel free to change or add some, was just getting an idea of things
int ballColor = NULL;
int ballLocationX = NULL;
int ballLocationY = NULL;
int state = 1;
int mapSizeX = 1000; // ???
int mapSizeY = 1000; // ???
int theta = 0;
int currentX = 0;
int currentY = 0;

//SFE_ISL29125 RGB_sensor;

void setup() {
	//sparki.gripperOpen(); //do we want to specify a value here?	
        Serial.begin(115200);
        //RGB_sensor.init();
        sparki.clearLCD();
}

//using the ultrasound sensor REQUIRES a delay b/c it takes a long time to get the results
//right now the code is running faster than it takes for the ping to come back, which I think might solve the curse of the incessant beeping
//but what should this delay be??

int lookAround() {
	//as of right now, this function just gets the cm ultrasound reading
	//perhaps we want him to swivel around and scope out whats good, i'll look into the math but it is probably easier to just have him look forward
		//have to  
	int dist = sparki.ping();
	delay(100); //what should this be?
	if (dist < 3 && dist > 0) {
                sparki.clearLCD();
                sparki.println(dist);
                sparki.updateLCD();
		
                //sparki.beep();
		return 0; //grab it up 
	}
	else {
		return 1; //no grab 
	}
	
}

int readColor() {  //why is broken ):
   //unsigned int color = RGB_sensor.readStatus();
   //Serial.println(color);
   //sparki.updateLCD();
    unsigned int color = 0;
    return color;
}

int state1() {
	if (currentX < mapSizeX) {
		sparki.moveForward(1);
		currentX++;
	}
	else {
		currentX = 0;
		sparki.moveLeft(90);
		theta = theta - 90;
		sparki.moveForward(10);
		currentX++;
		sparki.moveLeft(90);
		theta = theta - 90;
	}
	int vis = lookAround();
	if (vis == 1) {
		return 1; //didn't find anything, going back to state 1 
	}
	else {
		return 2; //found something! moving to state 2
	}
}

int state2() {
	int color = readColor();
	return 3;
}

int state3() {
	return 4;
}

int state4() {
	return 5;
}

int state5() {
	return 1;
}

//this might not be the best way of organizing our code. perhaps a class would be better? i.e a search class or something?
//because the code takes so long to execute he looks mad jerky
//which might be because sparki is unnecessarily looping through code here when he thinks he sees something which is all the time T_T

void loop() { 
	//stuff for odometry
	if (theta == -360) {
		theta = 0; 
	}
	else if (theta == 720) {
		theta = 0;
	}

	//main loop
      	if (state == 1) //search the map, only move to state 2 if an object is detected
	{
		state = state1();
	}
	else if (state == 2) 
	{
		state = state2();
	}
	else if (state == 3)
	{
		state = state3();
	}
	else if (state == 4)
	{
		state = state4();
	}
	else if (state == 5)
	{
		state = state5();
	}
	else
	{
		//all done!
	}
}	

//State 1 : Jared

//State 2 : Brian K.

//State 3 : Zack

//State 4 : Jen
	//Move from SBLI (South bin line intersection) to (0,0) cornor 
	//Resets odometry using localization

	//Split CCL (color coded line) into 3 regions and grey scale (or color)
	//each reagion to corrispond, use IR scanner on bottom of sparki to 
	//recognize the "color". When section that corrisponds to the ball
	//color is found sparki deposits ball

	//Sparki turns around and follows CCL to (0,0) then returns to SBLI  

//State 5 : Brian S.
	// Sparki moves from SBLI to the coordinates of the last location 
	// of where Sparki spotted the ball.
	
	// Clear global variables
	
	// Sparki will resume its patterned search in state 1
	
	

//State 6 : All
