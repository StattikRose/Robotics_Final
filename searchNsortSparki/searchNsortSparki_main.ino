#include <Sparki.h>
#include "SparkFunISL29125.h"
#include <Wire.h>
//#include "SFE_ISL29125.h"
#include "searchNsortSparki.h"

SFE_ISL29125 RGB_sensor;

void setup() {
	searchNsort sNs;
	sparki.clearLCD();
	sparki.println("running setup...");
	sparki.updateLCD();
	sparki.gripperOpen();
    delay(5000);
    sparki.gripperStop();
    sparki.servo(SERVO_CENTER);
    Serial.begin(115200);
    RGB_sensor.init();
    sparki.clearLCD();
}

void loop() { 
    sparki.clearLCD();
    sNs.printLocation();
    sparki.updateLCD();

	//stuff for odometry
	if (theta <= -360) {
		theta = 0 - (360 + theta);  //someone check my math pls
	}
	else if (theta >= 360) {
		theta = 0 + (theta - 360);
	}

	//main loop 
	//calibrate
    if (state == 0){
        sparki.println("calibrating...");
        sparki.updateLCD();
        state = state0();
        sparki.clearLCD();
    }

    //search the map, only move to state 2 if an object is detected
  	else if (state == 1){
	sparki.println("searching for objects...");
	sparki.updateLCD();
	state = state1();
	sparki.clearLCD();
	}

	else if (state == 2) 
	{
        sparki.println("collecting object and reading color...");
		sparki.updateLCD();
		state = state2();
        sparki.clearLCD();
        }
	else if (state == 3)
	{
        sparki.println("returning to bins...");
		sparki.updateLCD();
		state = state3();
        sparki.clearLCD();
	}
	else if (state == 4)
	{
                sparki.println("finding correct bin...");
		sparki.println("Starting state 4...");
		sparki.updateLCD();
		state = state4();
                sparki.clearLCD();
	}
	else if (state == 5)
	{
                sparki.println("returning to previous location...");
		sparki.updateLCD();
		state = state5();
                sparki.clearLCD();
	}
	else
	{
                sparki.println("partying...");
		sparki.updateLCD();
                sparki.moveLeft(90); //spin game strong
                sparki.clearLCD();
		//all done!
	}
}	
