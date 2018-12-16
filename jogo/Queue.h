#include"types.h"

void enQueue(Queue *q, int info);
int deQueue(Queue *q);
int head(Queue q);
bool isFull(Queue q);
bool isEmpty(Queue q);
void createQueue(Queue *q);
void destroyQueue(Queue *q);