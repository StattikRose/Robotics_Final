#include <Sparki.h>
#include "searchNsortSparki.h"

// /------^-----\
// |            |
// | 69  70  71 |
// | 68  64  67 |
// |  7  21   9 |
// | 22  25  13 |
// | 12  24  94 |
// |  8  28  90 |
// | 66  82  74 |
// \____________/
int searchNsortSparki::state0(){
	int code = sparki.readIR();

    switch(code){
	case 12: 
		pinkIR = sparki.lineCenter(); 
		break;
	case 24: 
		whiteIR = sparki.lineCenter(); 
		break;
	case 94: 
		blueIR = sparki.lineCenter(); 
		break;
	case 8: 
		pinkColor = readColor(); 
		break;
	case 28: 
		whiteColor = readColor(); 
		break;
	case 90: 
		blueColor = readColor(); 
		break;
	case 64: 
		return 1; 
		break;
	}
	return 0;
}

void searchNsortSparki::state1(){
	int vis, checkVis, Turn;

	if (moveDir == 0) {
    	if (currentX < mapSizeX) {
    		sparki.moveForward(1);
    		updateOdometry(1,0,0);
    	}
    	else {
            moveDir = 1;
    		sparki.moveLeft(90);
    		sparki.moveForward(10);
    		sparki.moveLeft(90);
    		updateOdometry(0,10,-180)
            sparki.servo(SERVO_CENTER);
    	}
    }
    else {
        if (currentX > 0) {
			sparki.moveForward(1);
			updateOdometry(1,0,0);
    	}
    	else {
            moveDir = 0;
    		sparki.moveRight(90);
    		sparki.moveForward(10);
    		sparki.moveRight(90);
    		updateOdometry(0,10,180)
            sparki.servo(SERVO_CENTER);
    	}  
    }
	vis = lookAround();
	if (vis == 0) {
		state = 1; //didn't find anything, going back to state 1 
	}
	else {
		sparki.println("object found"); 
		sparki.updateLCD();
        setFound();

        checkVis = lookAround();
        if (checkVis == 0) {
            Turn = foundTheta - getCurrentTheta();
            if (abs(getCurrentTheta()) < 90) {
                sparki.moveLeft(Turn);
                updateOdometry(0,0,(-1*Turn);
            }
            else {
                sparki.moveRight(Turn);
                updateOdometry(0,0,Turn);
            }
            sparki.println("oops!");
            sparki.updateLCD();
            state = 1; //oops!
            return;
        } 
        //found something! moving to state 2
        else {
		    state = 2;
        }      
	} 	
}

//this code will only work properly if it always runs after state 1 (after a ball is detected)  <-- have to make sure this works!
//use usTheta to get sparkis ultrasound rotation
//we need to do a coordinate transformation to allow us to obtain the desired sparki rotation 
//the usTheta variable is relative to sparki, but the theta variable is relative to the map
//...or not? am i thinking about ^^^ this correctly?
void searchNsortSparki::state2(){
	if (usTheta < 0) {
		sparki.moveRight(usTheta); //attempting to compensate for (-) usTheta value
		updateOdometry(0,0,(-1*usTheta));
	}
	else if (usTheta > 0) {
		sparki.moveRight(usTheta);
		updateOdometry(0,0,usTheta)
	} 

	sparki.servo(SERVO_CENTER);
        
    sparki.print("theta:");
    sparki.println(theta);
    sparki.moveForward(usDist);
    sparki.gripperClose();
    delay(3000);
    sparki.gripperStop();
    readColor(); 
}

void searchNsortSparki::state3(){
	// Determine the necessary rotation to get Sparki facing Down
	int wantedTheta = -180;
	int Turn = wantedTheta-theta;
	if (abs(theta) < 180) {
		sparki.moveRight(Turn);
	}
	else {
		sparki.moveLeft(Turn);
	}
	findSBLI();
}

void searchNsortSparki::state4(){
	int colorStripStart = sparki.edgeLeft();
	int colorStripEnd = sparki.edgeLeft();
            sparki.println(colorStripEnd);
            sparki.updateLCD();
	//Based on current IR reading move to an interstection
	//Pink
	if(colorStripStart < theColors.whiteIR && currentY <= mapSizeY/2){
		while(colorStripEnd < theColors.whiteIR){
			sparki.moveForward(1);
            updateOdometry(0,1,0);
			colorStripEnd = sparki.edgeLeft();
            sparki.println(colorStripEnd);
            sparki.updateLCD();
		}
	}
	//White
	else if(colorStripStart >= theColors.whiteIR){
		while(colorStripEnd >= theColors.whiteIR){
			sparki.moveForward(1);
            updateOdometry(0,1,0);                
			colorStripEnd = sparki.edgeLeft();
            sparki.println(colorStripEnd);
            sparki.updateLCD();
		}
	}
	//Blue
	else if(currentY >= mapSizeY/2){
		while(colorStripEnd < theColors.whiteIR){
			sparki.moveBackward(1);
            updateOdometry(0,1,0); 
			colorStripEnd = sparki.edgeLeft();
            sparki.println(colorStripEnd);
            sparki.updateLCD();
		}
	}
   
	//Once interstection is reached fix odometry
	//Started in pink
	if(currentY <= mapSizeY/2){
		updateOdometry(0,26);
	}
	//Started in white or blue
	else{
		updateOdometry(0,52);
	}

	//Find the correct bin (ball color corrisponds to greyscale)
	int moveDist = 0;
	if(ballColor == "white"){
		moveDist = fullMap.mintBinY - currentY;
        sparki.println("moving to white bin..");
	}
	else if(ballColor == "pink"){
		moveDist = fullMap.pinkBinY - currentY;
		sparki.println("moving to pink bin...");
	}
	else if(ballColor == "blue"){
		moveDist = fullMap.blueBinY - currentY;
        sparki.println("moving to blue bin...");
	}
    sparki.updateLCD();

    if(moveDist < 0){
      moveDist = moveDist * -1;
      sparki.moveBackward(moveDist);
      updateOdometry(0,(-1*moveDist),0);
    }
    else{
      sparki.moveForward(moveDist);
      updateOdometry(0,moveDist,0);
    }

    //Move so that grippers are over the bin
    sparki.moveLeft(90);
    updateOdometry(-180);
	while(sparki.lineCenter() > 900){
	    sparki.moveForward(1);
	    updateOdometry(1,0,0);
    }

	//Deliver ball
    sparki.moveBackward(1);
    currentX++;
    sparki.gripperOpen();
    delay(3000);
    sparki.gripperStop();
    sparki.moveLeft(180);
    updateOdometry(0);
    while(sparki.lineCenter() > 900){
      sparki.moveForward(1);
      updateOdometry(1,0,0);
    }
}

void searchNsortSparki::state5(){
	if(currentY < foundY){
		sparki.moveLeft(90);
		updateOdometry(0,0,-90);
		while(currentY < foundY){
			sparki.moveForward(1);
			updateOdometry(0,1,0);
		}
        sparki.moveRight(90);
        updateOdometry(0,0,90);
        while(currentX < foundX){
          sparki.moveForward(1);
          updateOdometry(1,0,0);
        }
    }
	else{
		sparki.moveRight(90);
		updateOdometry(0,0,90);
		while(currentY >foundY){
			sparki.moveForward(1);
			updateOdometry(0,-1,0);
		}
        sparki.moveLeft(90);
        updateOdometry(0,0,-90);
        while(currentX < foundX){
          sparki.moveForward(1);
          updateOdometry(1,0,0);
        }
	}
}

void searchNsortSparki::party(){
	
}

void searchNsortSparki::printLocation(){
	sparki.print("theta:");
    sparki.println(theta);
    sparki.print("x:");
    sparki.println(currentX);
    sparki.print("y:");
    sparki.println(currentY);
}

int searchNsortSparki::getCurrentTheta(){
	return currentTheta;
}

void searchNsortSparki::setFound(){
	foundX = currentX;
    foundY = currentY;
	foundTheta = theta;
}

//using the ultrasound sensor REQUIRES a delay b/c it takes a long time to get the results
//I changed this function a bit, it is now the only function that uses US.
int searchNsortSparki::lookAround(){
	const int sweepThresh = 35;
	const int sweetInc = 5;
	const int distMax = 15;
	int usDir = 0;

	//move ultrasound left
	if (usDir == 0) {
        usTheta = usTheta - sweepInc;
        //left boundary
        if (usTheta < -sweepThresh) { 
            usDir = 1; //switch dir
            usTheta = usTheta + sweepInc; //move right
        }
    }
    //move ultrasound right
    else {
         usTheta = usTheta + sweepInc;
         //right boundary
         if (usTheta > sweepThresh) {
             usDir = 0; //switch dir
             usTheta = usTheta - sweepInc; //move left
         }
    }
    sparki.servo(usTheta);
	usDist = sparki.ping();
	delay(100); //what should this be?

    sparki.clearLCD();
    sparki.print("Ultrasound Reading:");
	sparki.println(usDist);
	sparki.updateLCD();

	if (usDist < distMax && usDist > 0) {		
		return 1; //grab it up 
	}
	else {
		return 0; //no grab 
	}	
}

int searchNsortSparki::readColor(){
	sparki.println("reading color...");
   	unsigned int red = (RGB_sensor.readRed() + RGB_sensor.readRed() + RGB_sensor.readRed() + RGB_sensor.readRed() + RGB_sensor.readRed()) / 5;
   	unsigned int green = (RGB_sensor.readGreen() + RGB_sensor.readGreen() + RGB_sensor.readGreen() + RGB_sensor.readGreen() + RGB_sensor.readGreen()) / 5;
   	unsigned int blue = (RGB_sensor.readBlue() + RGB_sensor.readBlue() + RGB_sensor.readBlue() + RGB_sensor.readBlue() + RGB_sensor.readBlue()) / 5;
   	int sum = red + green + blue;
    sum = sum*10;
    return sum;
}

void searchNsortSparki::classifyColor(int sum){
	String color;
    if (sum <= theColors.blueColor - 50) {
          color = "blue";
    }
    else if (sum >= theColors.whiteColor - 50) {
          color = "white";  
    }
    else {
          color = "pink";  
    }
    foundItem.color = color;
   	sparki.println(color);
   	sparki.updateLCD();
   	delay(3000);
}

int searchNsortSparki::findLine(){
	// THRESHOLD BASED ON IR FROM WOOD
	int threshold = sparki.lineCenter();

	// BOOLEAN FLAG TO TELL SPARKI SBLI NOT FOUND
	int flag = 1;

	// UNTIL SBLI LINE
	while(flag) {
		// MOVE FORWARD
		sparki.moveForward(1);
		updateOdometry(1,0,0);
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
	updateOdometry(0,0,90);
}

void searchNsortSparki::updateOdometry(int moveX, int moveY, int moveTheta){
	currentX = currentX + moveX;
	currentY = currentY + moveY;
	currentTheta = currentTheta + moveTheta;
}

void searchNsortSparki::updateOdometry(int moveTheta){
	currentTheta = moveTheta;
}

void searchNsortSparki::updateOdometry(int moveX, int moveY){
	currentX = moveX;
	currentY = moveY;
}
