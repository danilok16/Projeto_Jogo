#include"types.h"

int numeroAleatorio(int menor, int maior) {
	return rand() % (maior - menor + 1) + menor;
}

float calcular_raio(float x2, float y2, float x1, float y1) {
	float raio;

	raio = sqrt(pow(abs(x1 - x2), 2) + pow(abs(y1 - y2), 2));
	return raio;
}