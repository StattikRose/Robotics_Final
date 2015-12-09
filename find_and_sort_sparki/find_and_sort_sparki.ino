
#include <Sparki.h>
#include "SparkFunISL29125.h"
#include <Wire.h>
//#include "SFE_ISL29125.h"

//Global variables
//Feel free to change or add some, was just getting an idea of things
int ballColor = NULL;
int ballLocationX = NULL;
int ballLocationY = NULL;
int state = 1;
int mapSizeX = 46; // ???
int mapSizeY = 46; // ???
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
int gray = NULL;
int white = NULL;
int black = NULL;
int BinY = NULL;
int red = NULL;
int green = NULL;
int blue = NULL;

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

int readColor() {  //why is broken ):
   sparki.println("reading color...");
   unsigned int color = RGB_sensor.readStatus();
   sparki.print("color:");
   sparki.println(color);
   sparki.updateLCD();
    //unsigned int color = 0;
    return color;
}

int state1() {
	if (currentX < mapSizeX) {
		sparki.moveForward(1);
		currentX++;
	}
	else {
                if (state1Dir == 0) {
		currentX = 0;
		sparki.moveLeft(90);
		theta = theta - 90;
		sparki.moveForward(10);
		currentY = currentY + 10;
		sparki.moveLeft(90);
		theta = theta - 90;
                state1Dir = 1;
                }
                else {
		currentX = 0;
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
                lookAround();
      		return 2; //found something! moving to state 2
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
            sparki.moveRight(usTheta); //bad convention, attempting to compensate for (-) usTheta value
            theta = theta - usTheta; 
        }
        else if (usTheta > 0) {
            sparki.moveRight(usTheta);
            theta = theta + usTheta; 
        } 
        sparki.print("theta");
        sparki.println(theta);
        sparki.moveForward(moveDist);   //IMPORTANT: this line ruins sparkis odometry estimate
        //OH FUCK here is where we have to do some crazy math
        //not even that crazy but still
        //need to update sparkis currentX and currentY! might not be able to use ints for this anymore
        sparki.gripperClose();
        delay(3000);
        sparki.gripperStop();
        int color = readColor();
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
    sparki.moveRight(Turn);
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
    if (abs(curIR - threshold) > 50) {
          flag = 0;
    }   
  }
  
  // ALLIGN SPARKI'S WHEEL WITH THE SBLI LINE 
  // MEASURE DISTANCE FROM IR TO WHEEL
  int senseSBLI = 2.2;
  sparki.moveForward(senseSBLI);
  
  // TURN SPARKI TO ALLIGN WITH THE SBLI
  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, 100);
  delay(200);
  sparki.motorStop(MOTOR_LEFT);
  return 4;
}

int state4() {
		int colorStripStart = sparki.lineCenter();
		int colorStripEnd = sparki.lineCenter();
		//Based on current IR reading move to an interstection
		//White
		if(colorStripStart > gray){
			sparki.moveRight(90);
			while(colorStripEnd > gray){
				sparki.moveForward();
				colorStripEnd = sparki.lineCenter();
			}
		}
		//Grey
		else if(colorStripStart > black){
			sparki.moveLeft(90);
			while(colorStripEnd < white){
				sparki.moveForward();
				colorStripEnd = sparki.lineCenter();
			}
		}
		//Black
		else{
			sparki.moveLeft(90);
			while(colorStripEnd < gray){
				sparki.moveForward();
				colorStripEnd = sparki.lineCenter();
			}
		}
		//Once interstection is reached fix odometry
		//Started in White or Grey
		if(colorStripStart > gray || colorStripStart > black){
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
		if(ballColor == red){
			moveDist = currentY - BinY;
		}
		else if(ballColor == green){
			moveDist = currentY - BinY;
		}
		else if(ballColor == blue){
			moveDist = currentY - BinY;
		}
                else {
                        sparki.println("unrecognized color");
                        sparki.updateLCD(); 
                }

		if(moveDist < 0){
			sparki.moveRight(180);
		}
		sparki.moveForward(moveDist);

		//Move so that grippers are over the bin
		//Move left or right then forward
		


		//Deliver ball
        sparki.gripperOpen();
        delay(3000);
        sparki.gripperStop();
	return 5;
}

int state5() {
	//use pathPlan() function here?
	//the location that sparki should return to is saved in the foundX and foundY vars. should we also include a foundTheta?
	// ^^^ made the foundTheta var just incase
        return 1;
}

//this might not be the best way of organizing our code. perhaps a class would be better? i.e a search class or something?
//because the code takes so long to execute he looks mad jerky
//which might be because sparki is unnecessarily looping through code here when he thinks he sees something which is all the time T_T

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
                delay(5000); //here b/c none of the other code is written so he just keeps looping back to states 1 and 2
	}
	else
	{
		//all done!
	}
}	
