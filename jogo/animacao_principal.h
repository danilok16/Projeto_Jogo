#include "types.h"


void exibir_animacao(int *cont_frames, int cenario, ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *vet_objetos_cenario[], ALLEGRO_BITMAP *personagem[], Personagem *p, INTERAGIR_COZINHA *interagir_alimento, INTERAGIR_BANHEIRO *interagir_banheiro, ALLEGRO_BITMAP *sprite_alimento, ALIMENTO_DESENHO *alimento
	, float pos_mouse_x, float pos_mouse_y, ALLEGRO_FONT *font_status);

void desenhar_objetos(ALLEGRO_BITMAP *vetor_imagens[], INTERAGIR_COZINHA *interagir_alimento, INTERAGIR_BANHEIRO *interagir_banheiro
	, ALLEGRO_BITMAP *sprite_alimento, ALIMENTO_DESENHO *a, int cenario, int x, int y);

void desenhar_alimento_mesa(ALLEGRO_BITMAP *sprite_alimento, ALLEGRO_FILE *arq, INTERAGIR_COZINHA *interagir_alimento, ALIMENTO_DESENHO *a, int x, int y, int id, Alimento vet_alimento[], ALLEGRO_FONT *font);

void exibir_texto(Personagem p, ALLEGRO_FONT *font_status, ALLEGRO_BITMAP *background, int cenario);

void desenhar_geladeira(ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *sprite_alimento, ALIMENTO_DESENHO a, Alimento vet_alimento[]);

void exibir_aviso(ALLEGRO_BITMAP *tela_aviso[], int aviso);
void chuveiro();

void ensaboar(Personagem p, ALLEGRO_BITMAP *bolhas, Cenario c, int cont);

void desenhar_mouse(ALLEGRO_BITMAP *mouse_of, ALLEGRO_BITMAP *mouse_on, bool status, int x, int y);

void desenhar_sujeira(Personagem p, ALLEGRO_BITMAP *sujeira);

void desenhar_acessorio_persnoagem(ALLEGRO_BITMAP *acessorio, Personagem p, Cenario c, int id_acessorio);

