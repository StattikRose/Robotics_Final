//Global variables
//Feel free to change or add some, was just getting an idea of things
int ballColor = NULL;
int ballLocationX = NULL;
int ballLocationY = NULL;
int currentState = 1;


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
