#include "types.h"
#include "animacao_principal.h"
#include"calc.h"
#include"save_game.h"


void exibir_animacao(int *cont_frames, int cenario, ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *vet_objetos_cenario[], ALLEGRO_BITMAP *personagem[], Personagem *p, INTERAGIR_COZINHA *interagir_alimento, INTERAGIR_BANHEIRO *interagir_banheiro, ALLEGRO_BITMAP *sprite_alimento, ALIMENTO_DESENHO *alimento
	, float pos_mouse_x, float pos_mouse_y, ALLEGRO_FONT *font_status) {

	p->regiao_x_folha = 0;
	p->regiao_y_folha = 0;
	if (cenario != TELA_STATUS) {
		/*---------------------exibir nivel do personagem --------------------------*/
		exibir_texto(*p, font_status, background, cenario);
		switch (p->status) {
		case PARADO:
			if (p->interagir == false) {
				if (*cont_frames >= 1 && *cont_frames < 200) {
					al_clear_to_color(al_map_rgb(0, 0, 0));
					/*-----------------desenhar cenario----------------*/
					al_draw_bitmap(background, 0, 0, 0);
					/*-----------------desenhar personagem-----------*/
					p->regiao_x_folha = 0;

					al_draw_bitmap_region(personagem[PARADO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
						p->pos_sprite_x, p->pos_sprite_y, 0);
					/*-----------------objetos do cenario----------------*/
					desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);

				}
				else if (*cont_frames == 200) {
					al_clear_to_color(al_map_rgb(0, 0, 0));
					al_draw_bitmap(background, 0, 0, 0);
					p->regiao_x_folha = 1 * p->largura_sprite;
					al_draw_bitmap_region(personagem[PARADO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
						p->pos_sprite_x, p->pos_sprite_y, 0);
					desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
				}
				else if (*cont_frames == 202) {
					al_clear_to_color(al_map_rgb(0, 0, 0));
					al_draw_bitmap(background, 0, 0, 0);
					p->regiao_x_folha = 2 * p->largura_sprite;
					al_draw_bitmap_region(personagem[PARADO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
						p->pos_sprite_x, p->pos_sprite_y, 0);
					*cont_frames = -1;
					desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
				}
			}
			else {
				if (*cont_frames == 1) {
					p->regiao_x_folha = 0;p->regiao_y_folha = 1 * p->altura_sprite;
					al_draw_bitmap_region(personagem[PARADO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
						p->pos_sprite_x, p->pos_sprite_y, 0);
				}
				else if (*cont_frames == 2) {
					p->regiao_x_folha = 1 * p->largura_sprite;p->regiao_y_folha = 1 * p->altura_sprite;
					al_draw_bitmap_region(personagem[PARADO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
						p->pos_sprite_x, p->pos_sprite_y, 0);
				}
				else if (*cont_frames == 3) {
					p->regiao_x_folha = 2 * p->largura_sprite;p->regiao_y_folha = 1 * p->altura_sprite;
					al_draw_bitmap_region(personagem[PARADO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
						p->pos_sprite_x, p->pos_sprite_y, 0);
				}
				else if (*cont_frames == 4) {
					p->regiao_x_folha = 0;p->regiao_y_folha = 2 * p->altura_sprite;
					al_draw_bitmap_region(personagem[PARADO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
						p->pos_sprite_x, p->pos_sprite_y, 0);
				}
				else if (*cont_frames == 5) {
					p->regiao_x_folha = 1 * p->largura_sprite;p->regiao_y_folha = 2 * p->altura_sprite;
					al_draw_bitmap_region(personagem[PARADO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
						p->pos_sprite_x, p->pos_sprite_y, 0);
				}
				else if (*cont_frames == 6) {
					p->regiao_x_folha = 2 * p->largura_sprite;p->regiao_y_folha = 2 * p->altura_sprite;
					al_draw_bitmap_region(personagem[PARADO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
						p->pos_sprite_x, p->pos_sprite_y, 0);
					*cont_frames = -80;
					p->interagir = false;
				}

			}

			break;
		case COM_SONO:
			if (*cont_frames >= 1 && *cont_frames <= 150) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				/*-----------------desenhar cenario----------------*/
				al_draw_bitmap(background, 0, 0, 0);
				/*-----------------desenhar personagem-----------*/
				p->regiao_x_folha = 0;
				al_draw_bitmap_region(personagem[COM_SONO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				/*-----------------objetos do cenario----------------*/
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames >= 150 && *cont_frames <= 165) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(background, 0, 0, 0);
				p->regiao_x_folha = 1 * p->largura_sprite;
				al_draw_bitmap_region(personagem[COM_SONO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames >= 165 && *cont_frames <= 300) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(background, 0, 0, 0);
				p->regiao_x_folha = 2 * p->largura_sprite;
				al_draw_bitmap_region(personagem[COM_SONO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames >= 300 && *cont_frames <= 305) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(background, 0, 0, 0);
				p->regiao_x_folha = 3 * p->largura_sprite;
				al_draw_bitmap_region(personagem[COM_SONO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames >= 305 && *cont_frames <= 310) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(background, 0, 0, 0);
				p->regiao_x_folha = 4 * p->largura_sprite;
				al_draw_bitmap_region(personagem[COM_SONO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);

			}
			else if (*cont_frames >= 310 && *cont_frames <=315) {
				p->regiao_x_folha = 0;p->regiao_y_folha = 1 * p->altura_sprite;
				al_draw_bitmap_region(personagem[COM_SONO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames >= 315 && *cont_frames<=320) {
				p->regiao_x_folha = 1 * p->largura_sprite;p->regiao_y_folha = 1 * p->altura_sprite;
				al_draw_bitmap_region(personagem[COM_SONO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames >= 320 && *cont_frames <=325) {
				p->regiao_x_folha = 2 * p->largura_sprite;p->regiao_y_folha = 1 * p->altura_sprite;
				al_draw_bitmap_region(personagem[COM_SONO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames >= 325 && *cont_frames <=330) {
				p->regiao_x_folha = 3 * p->largura_sprite;p->regiao_y_folha = 1 * p->altura_sprite;
				al_draw_bitmap_region(personagem[COM_SONO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames >= 330) {
				p->regiao_x_folha = 4 * p->largura_sprite;p->regiao_y_folha = 1 * p->altura_sprite;
				al_draw_bitmap_region(personagem[COM_SONO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
				*cont_frames = -5;
			}
			*cont_frames++;
			break;
		case COMENDO:
			if (*cont_frames == 1) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				/*-----------------desenhar cenario----------------*/
				al_draw_bitmap(background, 0, 0, 0);
				/*-----------------desenhar personagem-----------*/
				p->regiao_x_folha = 0;
				al_draw_bitmap_region(personagem[COMENDO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				/*-----------------objetos do cenario----------------*/
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames == 2) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(background, 0, 0, 0);
				p->regiao_x_folha = 1 * p->largura_sprite;
				al_draw_bitmap_region(personagem[COMENDO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames == 3) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(background, 0, 0, 0);
				p->regiao_x_folha = 2 * p->largura_sprite;
				al_draw_bitmap_region(personagem[COMENDO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames == 4) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(background, 0, 0, 0);
				p->regiao_x_folha = 3 * p->largura_sprite;
				al_draw_bitmap_region(personagem[COMENDO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames == 5) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(background, 0, 0, 0);
				p->regiao_x_folha = 4 * p->largura_sprite;
				al_draw_bitmap_region(personagem[COMENDO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);

			}
			else if (*cont_frames == 6) {
				p->regiao_x_folha = 0;p->regiao_y_folha = 1 * p->altura_sprite;
				al_draw_bitmap_region(personagem[COMENDO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames == 7) {
				p->regiao_x_folha = 1 * p->largura_sprite;p->regiao_y_folha = 1 * p->altura_sprite;
				al_draw_bitmap_region(personagem[COMENDO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames == 8) {
				p->regiao_x_folha = 2 * p->largura_sprite;p->regiao_y_folha = 1 * p->altura_sprite;
				al_draw_bitmap_region(personagem[COMENDO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames == 9) {
				p->regiao_x_folha = 3 * p->largura_sprite;p->regiao_y_folha = 1 * p->altura_sprite;
				al_draw_bitmap_region(personagem[COMENDO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
			}
			else if (*cont_frames == 10) {
				p->regiao_x_folha = 4 * p->largura_sprite;p->regiao_y_folha = 1 * p->altura_sprite;
				al_draw_bitmap_region(personagem[COMENDO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
				*cont_frames = 0;
				p->status = PARADO;
			}
			p->status = PARADO;
			*cont_frames++;
			break;
		case DORMINDO:
			if (*cont_frames >= 1 && *cont_frames < 100) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				/*-----------------desenhar cenario----------------*/
				al_draw_bitmap(background, 0, 0, 0);
				/*-----------------desenhar personagem-----------*/
				p->regiao_x_folha = 0;
				al_draw_bitmap_region(personagem[DORMINDO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
			}
			else if (*cont_frames >= 100 && *cont_frames <= 200) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(background, 0, 0, 0);
				p->regiao_x_folha = 1 * p->largura_sprite;
				al_draw_bitmap_region(personagem[DORMINDO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
			}
			else if (*cont_frames > 200) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(background, 0, 0, 0);
				p->regiao_x_folha = 2 * p->largura_sprite;
				al_draw_bitmap_region(personagem[DORMINDO], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				if (*cont_frames > 300)
					*cont_frames = -1;
			}
			break;
		case DOENTE:
			if (*cont_frames >= 1 && *cont_frames < 200) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				/*-----------------desenhar cenario----------------*/
				al_draw_bitmap(background, 0, 0, 0);
				/*-----------------desenhar personagem-----------*/
				p->regiao_x_folha = 0;
				al_draw_bitmap_region(personagem[DOENTE], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				/*-----------------objetos do cenario----------------*/
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);

			}
			else if (*cont_frames == 200) {
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_draw_bitmap(background, 0, 0, 0);
				p->regiao_x_folha = 1 * p->largura_sprite;
				al_draw_bitmap_region(personagem[DOENTE], p->regiao_x_folha, p->regiao_y_folha, p->largura_sprite, p->altura_sprite,
					p->pos_sprite_x, p->pos_sprite_y, 0);
				desenhar_objetos(vet_objetos_cenario, interagir_alimento, interagir_banheiro, sprite_alimento, alimento, cenario, pos_mouse_x, pos_mouse_y);
				*cont_frames = -15;
			}
			break;
		case SEM_INTERAGIR:
			al_draw_bitmap(background, 0, 0, 0);
		
			/*if (interagir_alimento->abrir_geladeira == true) {
				desenhar_geladeira(background, vet_objetos_cenario);
			}*/
			if (cenario >= LOJA_MENU && cenario <= LOJA_PERSONAGEM_3) {
				al_draw_textf(font_status, al_map_rgb(0, 0, 0), 102, 14, NULL, "%1.f", p->dinheiro);
			}
			break;
		}
	}
	else {
		al_draw_bitmap(background, 0, 0, 0);
	}
	exibir_texto(*p, font_status, background, cenario);
}


void desenhar_objetos(ALLEGRO_BITMAP *vetor_imagens[], INTERAGIR_COZINHA *interagir_alimento, INTERAGIR_BANHEIRO *interagir_banheiro
	, ALLEGRO_BITMAP *sprite_alimento, ALIMENTO_DESENHO *a, int cenario, int x, int y) {

	switch (cenario) {
	case SALA:break;
	case COZINHA: al_draw_bitmap(vetor_imagens[MESA], 320, 620, 0);
		break;
	case BANHEIRO:
		if (interagir_banheiro->movimentar_sabonete == true) { //movimenta o sabonete
			al_draw_bitmap(vetor_imagens[SABONETE], x - 10, y - 10, 0);
		}
		else {//desenha o sabonete no lugar certo
			al_draw_bitmap(vetor_imagens[SABONETE], 710, 195, 0);
		}
		break;
	}
}

void desenhar_geladeira(ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *sprite_alimento, ALIMENTO_DESENHO a, Alimento vet_alimento[]) {
	int matriz_alimento[15][10];
	int i = 1, j;
	int x = 0;
	int y = 0;

	/*for (int i = 1; i < 2;i++) {
		cout << vet_alimento[i].id_alimento;
		cout << vet_alimento[i].quantidade;
		cout << vet_alimento[i].preco;
	}*/
	/*-----------------desenhar cenario----------------*/
	al_draw_bitmap(background, 0, 0, 0);
	/*-----------------desenhar alimentos----------------*/

	
	/*for (i = 1; i < 29; i++) {
		cout << vet_alimento[1].id_alimento;
		//system("pause");
		a.pos_sprite_x = a.pos_sprite_x * x;
		a.pos_sprite_y = a.pos_sprite_y *y;
		al_draw_bitmap_region(sprite_alimento, a.regiao_x_folha, a.regiao_y_folha, a.largura_sprite, a.altura_sprite,
			a.pos_sprite_x, a.pos_sprite_y, 0);
		x += 10;

	}*/
	/*if (vet_alimento[1].id_alimento == 0) {
		al_draw_bitmap_region(sprite_alimento, a.regiao_x_folha, a.regiao_y_folha, a.largura_sprite, a.altura_sprite,
			a.pos_sprite_x, a.pos_sprite_y, 0);
	}
	if (vet_alimento[2].id_alimento == 2) {
		a.regiao_x_folha = 1 * a.largura_sprite;
		al_draw_bitmap_region(sprite_alimento, a.regiao_x_folha, a.regiao_y_folha, a.largura_sprite, a.altura_sprite,
			a.pos_sprite_x + 60, a.pos_sprite_y -80, 0);
	}*/
}

void chuveiro() {
	int pingo_agua_x = 448, pingo_agua_y = 84, cont = 0, n, y_desloca_pingo, x_desloca_pingo_direita = 560,
		x_desloca_pingo_esquerda = 448;

	n = numeroAleatorio(0, 1);
	if (n == 0) {
		pingo_agua_y = 84;
		y_desloca_pingo = 5;
	}
	else {
		pingo_agua_y = 100;
		y_desloca_pingo = 10;
	}
	while (cont < 1800) {
		if (pingo_agua_x > x_desloca_pingo_direita) {
			pingo_agua_x = x_desloca_pingo_esquerda;
			pingo_agua_y += y_desloca_pingo;
			x_desloca_pingo_direita++;
			x_desloca_pingo_esquerda--;
		}
		al_draw_filled_circle(pingo_agua_x, pingo_agua_y, 1, al_map_rgb(0, 200, 255));
		cont++;
		pingo_agua_x += 5;
	}
}

void ensaboar(Personagem p, ALLEGRO_BITMAP *bolhas, Cenario c, int cont) {
	int  n1, n2;
	float raio;
	bool desenha_bolha = false;

	if (c.cenario_atual < LOJA_MENU) {
		if (cont >= 100 && cont < 300) {

			/*-----------------desenhar cenario----------------*/
			/*-----------------desenhar personagem-----------*/
			p.regiao_x_folha = 0;
			al_draw_bitmap_region(bolhas, p.regiao_x_folha, p.regiao_y_folha, p.largura_sprite, p.altura_sprite,
				p.pos_sprite_x, p.pos_sprite_y, 0);

		}
		else if (cont > 300 && cont < 600) {


			p.regiao_x_folha = 1 * p.largura_sprite;
			al_draw_bitmap_region(bolhas, p.regiao_x_folha, p.regiao_y_folha, p.largura_sprite, p.altura_sprite,
				p.pos_sprite_x, p.pos_sprite_y, 0);

		}
		else if (cont > 600) {
			//al_clear_to_color(al_map_rgb(0, 0, 0));

			p.regiao_x_folha = 2 * p.largura_sprite;
			al_draw_bitmap_region(bolhas, p.regiao_x_folha, p.regiao_y_folha, p.largura_sprite, p.altura_sprite,
				p.pos_sprite_x, p.pos_sprite_y, 0);

		}
	}
}

void desenhar_sujeira(Personagem p, ALLEGRO_BITMAP *sujeira) {

	al_draw_bitmap(sujeira, p.pos_sprite_x+10, p.pos_sprite_y+7, 0);
	/*	al_draw_bitmap_region(sujeira, p.regiao_x_folha, p.regiao_y_folha, p.largura_sprite, p.altura_sprite,
			p.pos_sprite_x, p.pos_sprite_y, 0);

		al_draw_bitmap_region(bolhas, p.regiao_x_folha, p.regiao_y_folha, p.largura_sprite, p.altura_sprite,
			p.pos_sprite_x, p.pos_sprite_y, 0);*/
}

void exibir_texto(Personagem p, ALLEGRO_FONT *font_status, ALLEGRO_BITMAP *background, int cenario) {

	if (cenario == TELA_STATUS) {
		al_draw_textf(font_status, al_map_rgb(0, 0, 0), 426, 172, NULL, "%2.f R$", p.dinheiro);
		al_draw_textf(font_status, al_map_rgb(0, 0, 0), 328, 258, NULL, "%d - %2.f%%", p.nivel, p.progresso.progresso_nivel);
		al_draw_textf(font_status, al_map_rgb(0, 0, 0), 400, 338, NULL, "%2.f%%", p.progresso.porcentagem_energia);
		al_draw_textf(font_status, al_map_rgb(0, 0, 0), 368, 420, NULL, "%2.f%%", p.progresso.porcentagem_fome);
		al_draw_textf(font_status, al_map_rgb(0, 0, 0), 388, 502, NULL, "%2.f%%", p.progresso.porcentagem_limpeza);
		al_draw_textf(font_status, al_map_rgb(0, 0, 0), 334, 580, NULL, "%2.f%%", p.progresso.porcentagem_saude);
	}
	else if (cenario <= SALA_DE_JOGOS) {
		al_draw_textf(font_status, al_map_rgb(0, 0, 0), 964, 18, NULL, "%d", p.nivel);
	}



}

void exibir_aviso(ALLEGRO_BITMAP *tela_aviso[], int aviso) {

	al_draw_bitmap(tela_aviso[aviso], 0, 0, 0);
}

void desenhar_alimento_mesa(ALLEGRO_BITMAP *sprite_alimento, ALLEGRO_FILE *arq, INTERAGIR_COZINHA *interagir_alimento, ALIMENTO_DESENHO *a, int x, int y, int id, Alimento vet_alimento[], ALLEGRO_FONT *font){
    int i = 1;
	int pos_aux = id - 1;

	a->regiao_x_folha = 0;
	a->regiao_y_folha = 0;
	a->largura_sprite = 82;
	a->altura_sprite = 77;
	a->pos_sprite_x = 480;
	a->pos_sprite_y = 590;

	if (id > 5 && id < 10) {
		a->regiao_y_folha = 1 * a->altura_sprite;
		pos_aux = pos_aux - 5;
	}
	else if (id > 10 && id < 15) {
		a->regiao_y_folha = 2 * a->altura_sprite;
		pos_aux = pos_aux - 10;
	}
	else if (id > 15 && id < 20) {
		a->regiao_y_folha = 2 * a->altura_sprite;
		pos_aux = pos_aux - 10;
	}
	a->regiao_x_folha = pos_aux * a->largura_sprite-6;

	if (vet_alimento[id].quantidade > 0 && vet_alimento[id].quantidade < 30) {
		if (interagir_alimento->movimentar_alimento == true && interagir_alimento->comer == false) {
			a->pos_sprite_x = x - 18; a->pos_sprite_y = y - 18;
			al_draw_bitmap_region(sprite_alimento, a->regiao_x_folha, a->regiao_y_folha, a->largura_sprite, a->altura_sprite,
				a->pos_sprite_x, a->pos_sprite_y, 0);
			al_draw_textf(font, al_map_rgb(0, 0, 0), 500, 635, NULL, "%d", vet_alimento[id].quantidade);
			
		}
		else if (interagir_alimento->comer == false) {
			al_draw_bitmap_region(sprite_alimento, a->regiao_x_folha, a->regiao_y_folha, a->largura_sprite, a->altura_sprite,
				a->pos_sprite_x, a->pos_sprite_y, 0);
			al_draw_textf(font, al_map_rgb(0, 0, 0), 500, 635, NULL, "%d", vet_alimento[id].quantidade);
		}
	}
}


void desenhar_mouse(ALLEGRO_BITMAP *mouse_of, ALLEGRO_BITMAP *mouse_on, bool status, int x, int y) {

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_bitmap(mouse_of, x, y, 0);
}

void desenhar_acessorio_persnoagem(ALLEGRO_BITMAP *acessorio, Personagem p, Cenario c, int id_acessorio) {

	if (c.cenario_atual >= GUARDA_ROUPA_1 && c.cenario_atual <= GUARDA_ROUPA_2) {
		if (id_acessorio == ACESSORIO_1) {
			
			p.pos_sprite_x = 668; p.pos_sprite_y = 460;
		}
		else if (id_acessorio == ACESSORIO_2) {
			p.pos_sprite_x = 668; p.pos_sprite_y = 450;
		}
		else if (id_acessorio == ACESSORIO_3) {
			p.pos_sprite_x = 668; p.pos_sprite_y = 460;
		}
		else if (id_acessorio == ACESSORIO_4) {
			p.pos_sprite_x = 668; p.pos_sprite_y = 460;
		}
		else if (id_acessorio == ACESSORIO_5) {
			p.pos_sprite_x = 668; p.pos_sprite_y = 460;
		}
		else if (id_acessorio == ACESSORIO_6) {
			p.pos_sprite_x = 668; p.pos_sprite_y = 460;
		}
		else if (id_acessorio == ACESSORIO_7) {
			p.pos_sprite_x = 655; p.pos_sprite_y = 550;
		}
		else if (id_acessorio == ACESSORIO_8) {
			p.pos_sprite_x = 655; p.pos_sprite_y = 550;
		}
	}
	else {
		if (id_acessorio == ACESSORIO_1) {
			p.pos_sprite_x = 424; p.pos_sprite_y = 460;
		}
		else if (id_acessorio == ACESSORIO_2) {
			p.pos_sprite_x = 424; p.pos_sprite_y = 450;
		}
		else if (id_acessorio == ACESSORIO_3) {
			p.pos_sprite_x = 424; p.pos_sprite_y = 460;
		}
		else if (id_acessorio == ACESSORIO_4) {
			p.pos_sprite_x = 424; p.pos_sprite_y = 460;
		}
		else if (id_acessorio == ACESSORIO_5) {
			p.pos_sprite_x = 424; p.pos_sprite_y = 460;
		}
		else if (id_acessorio == ACESSORIO_6) {
			p.pos_sprite_x = 424; p.pos_sprite_y = 460;
		}
		else if (id_acessorio == ACESSORIO_7) {
			p.pos_sprite_x = 417; p.pos_sprite_y = 550;
		}
		else if (id_acessorio == ACESSORIO_8) {
			p.pos_sprite_x = 417; p.pos_sprite_y = 550;
		}
	}

	al_draw_bitmap(acessorio, p.pos_sprite_x, p.pos_sprite_y, 0);
}
