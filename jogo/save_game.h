#include"types.h"


ALLEGRO_FILE *criar_arquivo(char nome_arq[]);
Personagem load_file_personagem(ALLEGRO_FILE *arq);
void save_file_personagem(Personagem personagem, ALLEGRO_FILE *arq);

Cenario load_file_cenario(ALLEGRO_FILE *arq);
void save_file_cenario(Cenario cenario, ALLEGRO_FILE *arq);

TData load_file_data(ALLEGRO_FILE *arq);

void save_file_data(TData ultima_data, ALLEGRO_FILE *arq);

Alimento load_file_alimento(ALLEGRO_FILE *arq);
void load_file_alimento_vet(ALLEGRO_FILE *arq, Alimento alimento[]);
void save_file_alimento(Alimento alimento, ALLEGRO_FILE *arq);
void change_file_alimento(ALLEGRO_FILE *arq, Alimento a);

void load_file_aparencia(ALLEGRO_FILE *arq, Aparencia *aparencia);
void save_file_aparencia(Aparencia aparencia, ALLEGRO_FILE *arq);
