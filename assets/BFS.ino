// need this to print stuff
#include <Sparki.h>

// need this to use the queue
#include "queue.h"

// makes directions readable
enum direction 
{
	left, right, up, down, none
};

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
	enum direction action;	

	// the nodes neighbors
	struct node *left_neighbor;
	struct node *right_neighbor;
	struct node *up_neighbor;
	struct node *down_neighbor;
};

// breadth-first search
struct node *BFS(struct node map[4][4], int origin_x, int origin_y, int goal_x, int goal_y)
{
	// initialize current node at start position
	struct node *current = &map[origin_x][origin_y];
	current->visited = 1;

	// initialize queue of nodes
	struct queue nodes;
	nodes.size = 0;

	// initialize queue with origin
	enqueue(&nodes, current);

	// while the goal hasn't been reached
	while (current != &map[goal_x][goal_y])
	{
		// take node out of queue
		current = dequeue(&nodes);

		// check all of the neighbors

		// if left neighbor exists
		if (current->left_neighbor)
		{
			// if neighbor hasn't been visited and isn't a wall
			if (current->left_neighbor->visited == 0 && current->left_neighbor->wall == 0)
			{
				// mark as visited
				current->left_neighbor->visited = 1;

				// set parent to current node
				current->left_neighbor->parent = current;

				// add to queue
				enqueue(&nodes, current->left_neighbor);
			}
		}
	
		// if right neighbor exists
		if (current->right_neighbor)
		{
			// if neighbor hasn't been visited and isn't a wall
			if (current->right_neighbor->visited == 0 && current->right_neighbor->wall == 0)
			{
				// mark as visited
				current->right_neighbor->visited = 1;

				// set parent to current node
				current->right_neighbor->parent = current;

				// add to queue
				enqueue(&nodes, current->right_neighbor);
			}
		}

		// if down neighbor exists
		if (current->down_neighbor)
		{
			// if neighbor hasn't been visited and isn't a wall
			if (current->down_neighbor->visited == 0 && current->down_neighbor->wall == 0)
			{
				// mark as visited
				current->down_neighbor->visited = 1;

				// set parent to current node
				current->down_neighbor->parent = current;

				// add to queue
				enqueue(&nodes, current->down_neighbor);
			}
		}

		// if up neighbor exists
		if (current->up_neighbor)
		{
			// if neighbor hasn't been visited and isn't a wall
			if (current->up_neighbor->visited == 0 && current->up_neighbor->wall == 0)
			{
				// mark as visited
				current->up_neighbor->visited = 1;

				// set parent to current node
				current->up_neighbor->parent = current;

				// add to queue
				enqueue(&nodes, current->up_neighbor);
			}
		}
	}

	// return address of goal node
	return &map[goal_x][goal_y];
}

void setup()
{
}

void loop()
{
	sparki.clearLCD();

	// initialize nodes for map
	struct node node_0_0;
	node_0_0.wall = 0;

	struct node node_0_1;
	node_0_1.wall = 0;

	struct node node_0_2;
	node_0_2.wall = 0;

	struct node node_0_3;
	node_0_3.wall = 1;

	struct node node_1_0;
	node_1_0.wall = 1;

	struct node node_1_1;
	node_1_1.wall = 1;

	struct node node_1_2;
	node_1_2.wall = 0;

	struct node node_1_3;
	node_1_3.wall = 1;

	struct node node_2_0;
	node_2_0.wall = 0;

	struct node node_2_1;
	node_2_1.wall = 0;

	struct node node_2_2;
	node_2_2.wall = 0;

	struct node node_2_3;
	node_2_3.wall = 0;

	struct node node_3_0;
	node_3_0.wall = 1;

	struct node node_3_1;
	node_3_1.wall = 0;

	struct node node_3_2;
	node_3_2.wall = 1;

	struct node node_3_3;
	node_3_3.wall = 0;

	// initialize map of visited nodes
	struct node map[4][4] = {{node_0_0, node_0_1, node_0_2, node_0_3},
		   		 {node_1_0, node_1_1, node_1_2, node_1_3},
			   	 {node_2_0, node_2_1, node_2_2, node_2_3},
		   		 {node_3_0, node_3_1, node_3_2, node_3_3}};

	// iterate through nodes
	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			// initialize visited status
			map[row][column].visited = 0;

			// initialize parent status
			map[row][column].parent = NULL;

			// initialize actions
			map[row][column].action = none;

			// initialize neighbors

			// avoid left boundary
			if (column != 0) map[row][column].left_neighbor  = &map[row][column-1];
			else 		 map[row][column].left_neighbor  = NULL;

			// avoid right boundary
			if (column != 3) map[row][column].right_neighbor = &map[row][column+1];
			else 		 map[row][column].right_neighbor = NULL;

			// avoid top boundary
			if (row != 0)    map[row][column].up_neighbor    = &map[row-1][column];
  			else 		 map[row][column].up_neighbor    = NULL;

			// avoid bottom boundary
			if (row != 3)    map[row][column].down_neighbor  = &map[row+1][column];
  			else 		 map[row][column].down_neighbor  = NULL;
		}
	}

	// linked list traversal from goal to origin
	
	// set current node to goal
	struct node *current = BFS(map, 0, 0, 3, 1);

	// while current isn't null
	while (current)
	{
		// if current has a parent
		if (current->parent)
		{
			// set parent's action to get to current
			if      (current->parent == current->left_neighbor)  current->left_neighbor->action  = right;
			else if (current->parent == current->right_neighbor) current->right_neighbor->action = left;
			else if (current->parent == current->up_neighbor)    current->up_neighbor->action    = down;
			else if (current->parent == current->down_neighbor)  current->down_neighbor->action  = up;
		}

		// backtrack through parents to origin
		current = current->parent;
	}


	// print action matrix
	sparki.print("\n");

	for (int row = 0; row < 4; row++)
	{
		sparki.print("\n");

		for (int column = 0; column < 4; column++)
		{
			if (map[row][column].action == left)  sparki.print("l ");
			if (map[row][column].action == right) sparki.print("r ");
			if (map[row][column].action == down)  sparki.print("d ");
			if (map[row][column].action == up)    sparki.print("u ");
			if (map[row][column].action == none)  sparki.print("n ");
		}
	}	
	
	sparki.print("\n");

	sparki.updateLCD();
}
