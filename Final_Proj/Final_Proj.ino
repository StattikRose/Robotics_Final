
#include <Sparki.h>
//#include <Wire.h>
#include "SparkFunISL29125.h"
#include <Wire.h>
#include "SFE_ISL29125.h"

//Global variables
//Feel free to change or add some, was just getting an idea of things
int ballColor = NULL;
int ballLocationX = NULL;
int ballLocationY = NULL;
int state = 1;
int mapSizeX = 10; // ???
int mapSizeY = 10; // ???
int currentX = 0;
int currentY = 0;

SFE_ISL29125 RGB_sensor;

void setup() {
	//sparki.gripperOpen(); //do we want to specify a value here?	
        Serial.begin(115200);
        RGB_sensor.init();
        sparki.clearLCD();
}

void lookAround() {
	dist = sparki.ping();
	if (dist < 5) {
		sparki.beep();
		return 0
	}
	else {
		return 1
	}
	//this function will be used to get ultrasound readings
}

int readColor() {
   unsigned int color = RGB_sensor.readStatus();
   Serial.println(color);
   sparki.updateLCD();
   return color;
}

int state1() {
	if (currentX < mapSizeX) {
		sparki.moveForward(1);
		currentX++;
	}
	else {
		sparki.moveLeft(90);
		sparki.moveForward(1);
		sparki.moveLeft(90);
	}
	vis = lookAround()
	if (vis == 1) {
		return 1; //didn't find anything, going back to state 1 
	}
	else {
		return 2; //found something! moving to state 2
	}
}

int state2() {
	color = readColor()
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

void loop() { 
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

