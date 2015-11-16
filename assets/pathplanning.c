#import queue.h

struct node 
{
	int wall;
	int visited;
	struct node *parent;
	struct node *left;
	struct node *right;
	struct node *up;
	struct node *down;
};

