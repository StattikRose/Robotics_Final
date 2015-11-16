#include <Sparki.h>

// node keeps track of several things
struct node
{
	// whether there is a wall
	int wall;

	// whether a node has been visited
	int visited;

	// the nodes parent
	struct node *parent;

	// the action taken to get from the parent
	//enum direction action;	

	// the nodes neighbors
	struct node *left_neighbor;
	struct node *right_neighbor;
	struct node *up_neighbor;
	struct node *down_neighbor;
};

void setup()
{
  int * map[4][4];
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      struct node *ptr;
      ptr = &map[i][j];
      sparki.clearLCD(); 
      sparki.print(*map[i][j]); 
       sparki.updateLCD();
    }
  }
}

void loop()
{

}

