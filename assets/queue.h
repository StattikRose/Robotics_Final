// queue holds an array of nodes and a size
struct queue
{
	// array of nodes
	struct node *array[16];

	// size of array
	int size;
};

// add entry to queue
void enqueue(struct queue *q, struct node *entry)
{
	// add the entry to the end of the queue
	q->array[q->size] = entry;

	// increment the size of the queue
	(q->size)++;
}

struct node *dequeue(struct queue *q)
{
	// take first entry of the queue
	struct node *first = q->array[0];

	// move every other entry of the queue to the "left"
	for (int index = 0; index < q->size; index++) 
		
		q->array[index] = q->array[index+1];

	// decrement the size of the queue
	(q->size)--;

	// return first entry of the queue
	return first;
}

int isEmpty(struct queue *q)
{
	return (q->size == 0);
}
