#include <Sparki.h>
#include "SparkFunISL29125.h"
#include <Wire.h>
//#include "SFE_ISL29125.h"

//Global variables
String ballColor;
int ballLocationX = NULL;
int ballLocationY = NULL;
int state = 1;
int mapSizeX = 46; 
int mapSizeY = 56; 
int theta = 0;
int currentX = 0;
int currentY = 0;
int foundX;
int foundY;
int foundTheta;
int sbliX = 10; // ???
int sbliY = 0; // ???
int usTheta = 0;
int usDir = 0;
int moveDist;
int sweep = 5;
int sweepThresh = 25;
int state1Dir = 0;
int mintBinY = 15;
int blueBinY = 25;
int pinkBinY = 5;
int red = NULL;
int green = NULL;
int blue = NULL;
int moveDir = 0;

SFE_ISL29125 RGB_sensor;

void setup() {
	sparki.clearLCD();
	sparki.println("running setup...");
	sparki.updateLCD();
	sparki.gripperOpen(); //do we want to specify a value here?	
        delay(5000);
        sparki.gripperStop();
        sparki.servo(SERVO_CENTER);
        Serial.begin(115200);
        RGB_sensor.init();
        sparki.clearLCD();
}

//because this function will need to return a lot of information, perhaps it is better to keep it void and just update global vars. thoughts?
void planPath(int theta, int curX, int curY, int destX, int destY) {
	//instead of adjusting the angle to point towards the dest, it will be much easier to just have him readjust to point towards the bins
	int rotation = 0; //??? // <-- need to compute the angle of rotation needed to point him towards the bins
	// after rotating, he can just move horizontally and then vertically until he finds the right line with his ir
	int slopeX = destX - curX;
	int slopeY = destY - curY;
	//do we want to return the x and y distances? we should def use a function for this because sparki is gonna have to plan a path several times 
}

//using the ultrasound sensor REQUIRES a delay b/c it takes a long time to get the results
//right now the code is running faster than it takes for the ping to come back, which I think might solve the curse of the incessant beeping
//but what should this delay be??

int lookAround() {
        if (usDir == 0) { //move ultrasound left
            usTheta = usTheta - sweep;
            if (usTheta < -sweepThresh) { //left boundary
                usDir = 1; //switch dir
                usTheta = usTheta + sweep; //move right
            }
        }
        else { //move ultrasound right
             usTheta = usTheta + sweep;
             if (usTheta > sweepThresh) { //right boundary
                 usDir = 0; //switch dir
                 usTheta = usTheta - sweep; //move left
             }
        }
        sparki.servo(usTheta);
	int dist = sparki.ping();
	delay(100); //what should this be?
        sparki.clearLCD();
        sparki.println("Ultrasound Reading");
	sparki.print(dist);
        sparki.updateLCD();
	if (dist < 15 && dist > 0) {
                moveDist = dist;		
		return 0; //grab it up 
	}
	else {
		return 1; //no grab 
	}	
}

int readColor() {  
   sparki.println("reading color...");
   unsigned int red = (RGB_sensor.readRed() + RGB_sensor.readRed() + RGB_sensor.readRed() + RGB_sensor.readRed() + RGB_sensor.readRed()) / 5;
   unsigned int green = (RGB_sensor.readGreen() + RGB_sensor.readGreen() + RGB_sensor.readGreen() + RGB_sensor.readGreen() + RGB_sensor.readGreen()) / 5;
   unsigned int blue = (RGB_sensor.readBlue() + RGB_sensor.readBlue() + RGB_sensor.readBlue() + RGB_sensor.readBlue() + RGB_sensor.readBlue()) / 5;
   int sum = red + green + blue;
    sum = sum*10;
    String color;
    if (sum < 950) {
          color = "blue";
    }
    else if (sum > 1300) {
          color = "white";  
    }
    else {
          color = "pink";  
    }
   sparki.println(color);
   sparki.updateLCD();
   delay(3000);
   ballColor = color;
}

int state1() {
        if (moveDir == 0) {
        	if (currentX < mapSizeX) {
        		sparki.moveForward(1);
        		currentX++;
        	}
        	else {
                        moveDir = 1;
        		sparki.moveLeft(90);
        		theta = theta - 90;
        		sparki.moveForward(10);
        		currentY = currentY + 10;
        		sparki.moveLeft(90);
        		theta = theta - 90;
        	}
        }
        else {
                if (currentX > 0) {
        		sparki.moveForward(1);
        		currentX--;
        	}
        	else {
                        moveDir = 0;
        		sparki.moveRight(90);
        		theta = theta + 90;
        		sparki.moveForward(10);
        		currentY = currentY + 10;
        		sparki.moveRight(90);
        		theta = theta + 90;
                        state1Dir = 0;
        	}  
        }
	int vis = lookAround();
	if (vis == 1) {
		return 1; //didn't find anything, going back to state 1 
	}
	else {
		sparki.println("object found"); 
		sparki.updateLCD();
                foundX = currentX;
                foundY = currentY;
		foundTheta = theta;
                int checkVis = lookAround();
                if (checkVis == 1) {
                    int wantedTheta = foundTheta;
                    int Turn = wantedTheta-theta;
                    if (abs(theta) < 90) {
                          sparki.moveLeft(Turn);
                    }
                    else {
                          sparki.moveRight(Turn);
                    }
                    sparki.println("oops!");
                    sparki.updateLCD();
                    return 1; //oops!
                }
                else {
      		    return 2; //found something! moving to state 2
                }      
	} 
}

int state2() {
        sparki.servo(SERVO_CENTER);
        //this code will only work properly if it always runs after state 1 (after a ball is detected)  <-- have to make sure this works!
        
        //use usTheta to get sparkis ultrasound rotation
        //we need to do a coordinate transformation to allow us to obtain the desired sparki rotation 
        //the usTheta variable is relative to sparki, but the theta variable is relative to the map
        //...or not? am i thinking about ^^^ this correctly?
        if (usTheta < 0) {
            sparki.moveRight(usTheta); //attempting to compensate for (-) usTheta value
            theta = theta - usTheta; 
        }
        else if (usTheta > 0) {
            sparki.moveRight(usTheta);
            theta = theta + usTheta; 
        } 
        sparki.print("theta:");
        sparki.println(theta);
        sparki.moveForward(moveDist);
        sparki.gripperClose();
        delay(3000);
        sparki.gripperStop();
        readColor(); 
        String temp = ballColor;
        bool bad = true;
        while(!bad){
          readColor();
          if(temp == ballColor)
            bad = false;
        }
	return 3;
}

// So, Sparki has an identified object in his hands
// Need to proceed to the SBLI
int state3() {
  // Determine the necessary rotation to get Sparki facing Down
  int wantedTheta = -180;
  int Turn = wantedTheta-theta;
  if (abs(theta) < 90) {
    sparki.moveLeft(Turn);
  }
  else {
    sparki.moveLeft(Turn);
  }
  int sbli1 = findSBLI();
  return 4;
}
  
int findSBLI() {
  // THRESHOLD BASED ON IR FROM WOOD
  int threshold = sparki.lineCenter();
  
  // BOOLEAN FLAG TO TELL SPARKI SBLI NOT FOUND
  int flag = 1;
  
  // UNTIL SBLI LINE
  while(flag) {
    // MOVE FORWARD
    sparki.moveForward(1);
    
    // CHECK IR READINGS
    int curIR = sparki.lineCenter();
    
    // IF SUFFICIENT CHANGE IN IR READING
    if (abs(curIR - threshold) > 500) {
          flag = 0;
    }   
  }
  
  // ALIGN SPARKI'S WHEEL WITH THE SBLI LINE 
  // MEASURE DISTANCE FROM IR TO WHEEL
  int senseSBLI = 3;
  sparki.moveForward(senseSBLI);
  
  // TURN SPARKI TO ALLIGN WITH THE SBLI
  sparki.moveRight(90);
  return 4;
}

int state4() {
		int colorStripStart = sparki.lineLeft();
		int colorStripEnd = sparki.lineLeft();
                sparki.println(colorStripEnd);
                sparki.updateLCD();
		//Based on current IR reading move to an interstection
		//Pink
		if(colorStripStart < 970 && currentY <= mapSizeY/2){
			while(colorStripEnd < 970){
				sparki.moveForward(1);
                                currentY++;
				colorStripEnd = sparki.lineLeft();
                                sparki.println(colorStripEnd);
                                sparki.updateLCD();
			}
		}
		//Mint
		else if(colorStripStart >= 970){
			while(colorStripEnd >= 970){
				sparki.moveForward(1);
                                currentY++;
				colorStripEnd = sparki.lineLeft();
                                sparki.println(colorStripEnd);
                                sparki.updateLCD();
			}
		}
		//Blue
		else if(currentY >= mapSizeY/2){
			while(colorStripEnd < 970){
				sparki.moveBackward(1);
                                currentY--;
				colorStripEnd = sparki.lineLeft();
                                sparki.println(colorStripEnd);
                                sparki.updateLCD();
			}
		}
		//Once interstection is reached fix odometry
		//Started in White or Grey
		if(currentY <= mapSizeY/2){
			currentX =  10;// "LOWER INTERSECTION X";
			currentY =  25;//"LOWER INTERSECTION Y";
		}
		//Started in Black
		else{
			currentX =  10;//"UPPER INTERSECTION X";
			currentY =  50;//"UPPER INTERSECTION Y";
		}

		//Find the correct bin (ball color corrisponds to greyscale)
		int moveDist = 0;
		if(ballColor == "mint"){
			moveDist = mintBinY - currentY;
                        sparki.println("moving to mint bin..");
		}
		else if(ballColor == "pink"){
			moveDist = pinkBinY - currentY;

                        sparki.println("moving to pink bin...");
		}
		else if(ballColor == "blue"){
			moveDist =blueBinY - currentY;

                        sparki.println("moving to blue bin...");
		}
                else {
                        sparki.println("unrecognized color"); 
                }
                sparki.updateLCD();
    if(moveDist < 0){
      moveDist = moveDist * -1;
      sparki.moveBackward(moveDist);
      currentY = currentY - moveDist;
    }
    else{
      sparki.moveForward(moveDist);
      currentY = currentY + moveDist;
    }
    //Move so that grippers are over the bin
    sparki.moveLeft(90);
    theta = -180;
		while(sparki.lineCenter() > 900){
      sparki.moveForward(1);
      currentX--;
    }
		//Deliver ball
        sparki.moveBackward(1);
        currentX++;
        sparki.gripperOpen();
        delay(3000);
        sparki.gripperStop();
    sparki.moveLeft(180);
    theta = 0;
    while(sparki.lineCenter() > 900){
      sparki.moveForward(1);
      currentX++;
    }
	return 5;
}

int state5() {
	//use pathPlan() function here?
	//the location that sparki should return to is saved in the foundX and foundY vars. should we also include a foundTheta?
	// ^^^ made the foundTheta var just incase
	
	// Return to SBLI line?
	
	//Return to original position:
	sparki.moveRight(180);
	while(currentX < foundX){
		sparki.moveForward(1);
		currentX++;
	}
	if(currentY < foundY){
		sparki.moveLeft(90);
		while(currentY < foundY){
			sparki.moveForward(1);
			currentY++;
		}
        }
	else{
		sparki.moveRight(90);
		while(currentY >foundY){
			sparki.moveForward(1);
			currentY--;
		}
	}
    return 1;
}

void loop() { 
	//stuff for odometry
	if (theta <= -360) {
		theta = 0 - (360 + theta);  //someone check my math pls
	}
	else if (theta >= 360) {
		theta = 0 + (theta - 360);
	}

	//main loop
      	if (state == 1) //search the map, only move to state 2 if an object is detected
	{
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
