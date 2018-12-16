#include"types.h"

void createQueue(Queue *q) {
	*q = (Tdescritor*)malloc(sizeof(Tdescritor));
	(*q)->inicio = NULL;
	(*q)->fim = NULL;
}
void destroyQueue(Queue *q) {
	TNoQueue *aux;

	while ((*q)->inicio != NULL) {
		aux = (*q)->inicio;
		(*q)->inicio = aux->prox;
		free(aux);
	}
	free(*q);
	*q = NULL;
}
void enQueue(Queue *q, int info) {
	TNoQueue *novo;

	novo = (TNoQueue*)malloc(sizeof(TNoQueue));
	novo->info = info;
	novo->prox = NULL;
	if ((*q)->inicio == NULL) {
		(*q)->inicio = novo;
		(*q)->fim = novo;
	}
	else {
	
		(*q)->fim->prox = novo;
		(*q)->fim = novo;
	}
}
int deQueue(Queue *q) {
	int info;
	TNoQueue *aux;

	aux = (*q)->inicio;
	(*q)->inicio = aux->prox;
	info = aux->info;
	if ((*q)->inicio == NULL) {
		(*q)->fim = NULL;
	}
	free(aux);
	return info;
}

int head(Queue q) {
	return q->inicio->info;
}

bool isFull(Queue q) {
	return false;
}

bool isEmpty(Queue q) {

	if (q->inicio == NULL) {
		return true;
	}
	else {
		return false;
	}
}
