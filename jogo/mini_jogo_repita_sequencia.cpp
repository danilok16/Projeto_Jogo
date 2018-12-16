#include"types.h"
//#include"mini_jogo_repita_sequencia.h"
#include"animacao_principal.h"
#include"calc.h"
#include"Queue.h"

bool carregar_imagem_mj1(ALLEGRO_BITMAP *pmj1[], ALLEGRO_BITMAP **text);
bool carregar_efeitos_sonoros(ALLEGRO_SAMPLE *efeitos[]);
void animacao_mj1(int *cont_frames, ALLEGRO_BITMAP *pmj1[], Mini_jogo_mj1 *jogo, Queue sequencia, ALLEGRO_FONT *font_status, float dinheiro, ALLEGRO_BITMAP **text, ALLEGRO_SAMPLE *efeitos[], int x, int y);
void tela_fim_de_jogo(ALLEGRO_BITMAP *pmj1[], ALLEGRO_FONT *font_status, float dinheiro);

Mini_jogo mini_jogo_repita_sequencia(ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_EVENT *evento, ALLEGRO_FONT *font_status, ALLEGRO_DISPLAY *janela) {

	//efeitos sonoros
	ALLEGRO_SAMPLE *efeitos[3];

	//imagens
	ALLEGRO_BITMAP *pmj1[6];
	ALLEGRO_BITMAP *text = NULL;
	int aux;
	int cont_frames = 0;
	bool fim = false;
	bool jogar_novamente = true;
	float dinheiro = 0;

	//variaveis mj1
	Mini_jogo_mj1 jogo;
	jogo.inicio_qtd_sequencia = 0;
	jogo.qtd_sequencia = 1;
	jogo.responder = false;
	bool flag = false;

	//variavel de retorno
	Mini_jogo result;
	result.dinheiro_ganho = 0;
	result.progresso = 0;
	result.error = false;
	result.id = 1;
	result.display_close = false;
	int  x = 0;
	int y = 0;
	x = evento->mouse.x - 20; y = evento->mouse.y - 20;
	//fila 
	Queue sequencia;
	Queue sequencia2;
	createQueue(&sequencia);
	createQueue(&sequencia2);
	
	if (!carregar_efeitos_sonoros(efeitos)) {
		result.error = true;
		return result;
	}
	if (!carregar_imagem_mj1(pmj1, &text)) { //testar retorno
		result.error = true;
		return result;
	}
	do {
		al_flush_event_queue(fila_eventos);
		while (!fim) {
			al_wait_for_event(fila_eventos, &*evento);//fila de eventos
			if (evento->type == ALLEGRO_EVENT_MOUSE_AXES) {
				x = evento->mouse.x -20; y = evento->mouse.y-20;

			}
			else if (evento->type == ALLEGRO_EVENT_DISPLAY_CLOSE) {//fechar a tela no botão
				fim = true;
				result.display_close = true;
				al_flush_event_queue(fila_eventos);
			}

			else if (evento->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && jogo.responder == true) {

				//azul
				if (evento->mouse.button & 1 && evento->mouse.x >= 218 && evento->mouse.x <= 453
					&& evento->mouse.y >= 140 && evento->mouse.y <= 360) {

					jogo.resp = 1;
					if (isEmpty(sequencia2) == false) {
						aux = deQueue(&sequencia2);
						if (jogo.resp != aux) {
							al_play_sample(efeitos[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							fim = true;
						}
						else
							al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
				}
				//amarelo
				else if (evento->mouse.button & 1 && evento->mouse.x >= 565 && evento->mouse.x <= 795
					&& evento->mouse.y >= 140 && evento->mouse.y <= 360) {

					jogo.resp = 2;
					if (isEmpty(sequencia2) == false) {
						aux = deQueue(&sequencia2);
						if (jogo.resp != aux) {
							al_play_sample(efeitos[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							fim = true;
						}
						else
							al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
				}
				//verde
				else if (evento->mouse.button & 1 && evento->mouse.x >= 218 && evento->mouse.x <= 453
					&& evento->mouse.y >= 430 && evento->mouse.y <= 659) {

					jogo.resp = 3;
					if (isEmpty(sequencia2) == false) {
						aux = deQueue(&sequencia2);
						if (jogo.resp != aux) {
							al_play_sample(efeitos[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							fim = true;
						}
						else
							al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
				}
				//vermelho
				else if (evento->mouse.button & 1 && evento->mouse.x >= 565 && evento->mouse.x <= 795
					&& evento->mouse.y >= 430 && evento->mouse.y <= 659) {

					jogo.resp = 4;
					if (isEmpty(sequencia2) == false) {
						aux = deQueue(&sequencia2);
						if (jogo.resp != aux) {
							al_play_sample(efeitos[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							fim = true;
						}
						else
							al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
				}
				if (fim != true) {
					jogo.draw_resposta = true;
					animacao_mj1(&cont_frames, pmj1, &jogo, sequencia, font_status, dinheiro, &text, efeitos, evento->mouse.x, evento->mouse.y);
					if (isEmpty(sequencia2) == true) {
						dinheiro += jogo.qtd_sequencia;
						jogo.qtd_sequencia++;
						jogo.inicio_qtd_sequencia = 0;
						jogo.responder = false;
						al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_rest(3);
					}
				}
			}
			if (evento->type == ALLEGRO_EVENT_TIMER) {
				if (jogo.inicio_qtd_sequencia < jogo.qtd_sequencia) {
					jogo.numero_aleatorio = numeroAleatorio(1, 4);
					enQueue(&sequencia, jogo.numero_aleatorio);
					enQueue(&sequencia2, jogo.numero_aleatorio);
					//animacao_mj1(&cont_frames, pmj1, background_mj1, p1, &jogo);
					jogo.inicio_qtd_sequencia++;
				}
				else if (jogo.inicio_qtd_sequencia == jogo.qtd_sequencia) {
					jogo.responder = true;
					//cont_frames++;
					animacao_mj1(&cont_frames, pmj1, &jogo, sequencia, font_status, dinheiro, &text, efeitos, evento->mouse.x, evento->mouse.y);
				}

			}
			al_flip_display();
		}
		if (result.display_close == false) {
			al_show_mouse_cursor(janela);
			tela_fim_de_jogo(pmj1, font_status, dinheiro);
			al_wait_for_event(fila_eventos, &*evento);
			if (evento->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				if (evento->mouse.button & 1 && evento->mouse.x >= 293 && evento->mouse.x <= 463
					&& evento->mouse.y >= 579 && evento->mouse.y <= 658) {
					jogar_novamente = true;
					fim = false;
					result.dinheiro_ganho += dinheiro;
					result.progresso += jogo.qtd_sequencia;
					dinheiro = 0;
					jogo.inicio_qtd_sequencia = 0;
					jogo.qtd_sequencia = 1;
					jogo.responder = false;
					bool flag = false;
					while (isEmpty(sequencia) == false) {
						deQueue(&sequencia);
					}
					while (isEmpty(sequencia2) == false) {
						deQueue(&sequencia2);
					}

				}
				else if (evento->mouse.button & 1 && evento->mouse.x >= 598 && evento->mouse.x <= 765
					&& evento->mouse.y >= 579 && evento->mouse.y <= 658) {
					jogar_novamente = false;
				}

			}
		}
		else {
			jogar_novamente = false;
		}
	}while (jogar_novamente == true);
	al_destroy_bitmap(pmj1[0]);
	al_destroy_bitmap(pmj1[1]);
	al_destroy_bitmap(pmj1[2]);
	al_destroy_bitmap(pmj1[3]);
	al_destroy_bitmap(pmj1[4]);
	al_destroy_bitmap(pmj1[5]);
	al_destroy_sample(efeitos[0]);
	al_destroy_sample(efeitos[1]);
	al_destroy_sample(efeitos[2]);
	destroyQueue(&sequencia);
	destroyQueue(&sequencia2);
	result.dinheiro_ganho += dinheiro;
	result.progresso += jogo.qtd_sequencia;
	return result;
}

bool carregar_imagem_mj1(ALLEGRO_BITMAP *pmj1[], ALLEGRO_BITMAP **text) {

	pmj1[0] = al_load_bitmap("mini_jogos\\mj1\\mj1_1.png");
	if (!pmj1[0]) {
		return false;
	}
	pmj1[1] = al_load_bitmap("mini_jogos\\mj1\\mj1_2.png");//azul
	if (!pmj1[1]) {
		al_destroy_bitmap(pmj1[0]);
		return false;
	}
	pmj1[2] = al_load_bitmap("mini_jogos\\mj1\\mj1_3.png");//amarelo
	if (!pmj1[2]) {
		al_destroy_bitmap(pmj1[0]);
		al_destroy_bitmap(pmj1[1]);
		return false;
	}
	pmj1[3] = al_load_bitmap("mini_jogos\\mj1\\mj1_4.png");//verde
	if (!pmj1[3]) {
		al_destroy_bitmap(pmj1[0]);
		al_destroy_bitmap(pmj1[1]);
		al_destroy_bitmap(pmj1[2]);
		return false;
	}
	pmj1[4] = al_load_bitmap("mini_jogos\\mj1\\mj1_5.png");//vermelho
	if (!pmj1[4]) {
		al_destroy_bitmap(pmj1[0]);
		al_destroy_bitmap(pmj1[1]);
		al_destroy_bitmap(pmj1[2]);
		al_destroy_bitmap(pmj1[3]);
		return false;
	}
	pmj1[5] = al_load_bitmap("mini_jogos\\mj1\\mj1_6.png");//fim de jogo
	if (!pmj1[5]) {
		al_destroy_bitmap(pmj1[0]);
		al_destroy_bitmap(pmj1[1]);
		al_destroy_bitmap(pmj1[2]);
		al_destroy_bitmap(pmj1[3]);
		al_destroy_bitmap(pmj1[4]);

		return false;
	}
	*text = al_load_bitmap("mini_jogos\\mj1\\mj1_text.png");//fim de jogo
	if (!*text) {
		al_destroy_bitmap(pmj1[0]);
		al_destroy_bitmap(pmj1[1]);
		al_destroy_bitmap(pmj1[2]);
		al_destroy_bitmap(pmj1[3]);
		al_destroy_bitmap(pmj1[5]);
		return false;
	}
	return true;
}

bool carregar_efeitos_sonoros(ALLEGRO_SAMPLE *efeitos[]) {

	efeitos[0] = al_load_sample("efeitos_sonoros\\mj1\\errou.ogg");
	if (!efeitos[0]) {
		return false;
	}
	efeitos[1] = al_load_sample("efeitos_sonoros\\mj1\\click.ogg");
	if (!efeitos[1]) {
		return false;
	}
	efeitos[2] = al_load_sample("efeitos_sonoros\\mj1\\resposta_certa.ogg");
	if (!efeitos[2]) {
		return false;
	}
	return true;
}

void animacao_mj1(int *cont_frames, ALLEGRO_BITMAP *pmj1[], Mini_jogo_mj1 *jogo, Queue sequencia, ALLEGRO_FONT *font_status, float dinheiro, ALLEGRO_BITMAP **text, ALLEGRO_SAMPLE *efeitos[], int x, int y) {
	int aux = 0, i = 0;
	bool desenho = false;


	//al_draw_bitmap(mouse[0], x, y, 0);
	//al_flip_display();
	if (jogo->draw_resposta == true) {
		aux = jogo->resp;
		if (aux == 1) {
			al_draw_bitmap(pmj1[1], 0, 0, 0);//azul
		}
		else if (aux == 2) {
			al_draw_bitmap(pmj1[2], 0, 0, 0);//amarelo
		}
		else if (aux == 3) {
			al_draw_bitmap(pmj1[3], 0, 0, 0);//vemelho
		}
		else if (aux == 4) {
			al_draw_bitmap(pmj1[4], 0, 0, 0);//verde
		}
		al_draw_textf(font_status, al_map_rgb(0, 0, 0), 175, 710, NULL, "%d", jogo->qtd_sequencia);
		al_draw_textf(font_status, al_map_rgb(0, 0, 0), 102, 14, NULL, "%1.f",dinheiro);
		al_draw_bitmap(*text, 260, 30, 0);
		al_flip_display();
		al_rest(0.100000000);
		al_draw_bitmap(pmj1[0], 0, 0, 0);
		al_draw_textf(font_status, al_map_rgb(0, 0, 0), 175, 710, NULL, "%d", jogo->qtd_sequencia);
		al_draw_textf(font_status, al_map_rgb(0, 0, 0), 102, 14, NULL, "%1.f", dinheiro);
		al_draw_bitmap(*text, 260, 30, 0);
		//al_draw_text(font_status, al_map_rgb(0, 0, 0), 280, 14, NULL, "REPITA A SEQUÊNCIA");
		al_flip_display();
		//al_rest(0.100000000);
		jogo->draw_resposta = false;
	}
	else {
		al_draw_bitmap(pmj1[0], 0, 0, 0);
		while (isEmpty(sequencia) == false) {
			aux = deQueue(&sequencia);
			//desenho = true;
			if (aux == 1) {
				al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				al_draw_bitmap(pmj1[1], 0, 0, 0);//azul
			}
			else if (aux == 2) {
				al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				al_draw_bitmap(pmj1[2], 0, 0, 0);//amarelo
			}
			else if (aux == 3) {
				al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				al_draw_bitmap(pmj1[3], 0, 0, 0);//vemelho
			}
			else if (aux == 4) {
				al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				al_draw_bitmap(pmj1[4], 0, 0, 0);//verde
			}
			al_draw_textf(font_status, al_map_rgb(0, 0, 0), 175, 710, NULL, "%d", jogo->qtd_sequencia);
			al_draw_textf(font_status, al_map_rgb(0, 0, 0), 102, 14, NULL, "%1.f", dinheiro);
			al_flip_display();
			al_rest(0.900000000);
			al_draw_bitmap(pmj1[0], 0, 0, 0);
			al_draw_textf(font_status, al_map_rgb(0, 0, 0), 175, 710, NULL, "%d", jogo->qtd_sequencia);
			al_draw_textf(font_status, al_map_rgb(0, 0, 0), 102, 14, NULL, "%1.f", dinheiro);
			al_flip_display();
			al_rest(0.100000000);
		}
		al_draw_textf(font_status, al_map_rgb(0, 0, 0), 175, 710, NULL, "%d", jogo->qtd_sequencia);
		al_draw_textf(font_status, al_map_rgb(0, 0, 0), 102, 14, NULL, "%1.f", dinheiro);
		al_draw_bitmap(*text, 260, 30, 0);
		//al_draw_bitmap(mouse[0], x, y, 0);
		//al_draw_text(font_status, al_map_rgb(0, 0, 0), 280, 14, NULL, "REPITA A SEQUENCIA");
		al_flip_display();
	}	
	jogo->responder = true;
}

void tela_fim_de_jogo(ALLEGRO_BITMAP *pmj1[], ALLEGRO_FONT *font_status, float dinheiro) {

	al_draw_bitmap(pmj1[5], 0, 0, 0);
	al_draw_textf(font_status, al_map_rgb(0, 0, 0), 102, 14, NULL, "%1.f", dinheiro);
	al_flip_display();
}



