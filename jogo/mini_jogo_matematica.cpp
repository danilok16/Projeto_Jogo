#include"types.h"
//#include"mini_jogo_matematica.h"
#include"calc.h"


enum {
	SOMA, SUBTRACAO, MULTIPLICAO
};

bool carregar_imagem_mj2(ALLEGRO_BITMAP *pmj2[]);

bool resposta(int n1, int n2, int n3, int operacao) {
	int aux;
	switch (operacao) {
	case 1:
		aux = n1 + n2;
		if (aux != n3) {
			return false;
		}
		break;
	case 2:
		aux = n1 - n2;
		if (aux != n3) {
			return false;
		}
		break;
	case 3:
		aux = n1 * n2;
		if (aux != n3) {
			return false;
		}
		break;

	}
	return true;
}

Mini_jogo mini_jogo_matematica(ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_EVENT *evento) {
	ALLEGRO_FONT *font1 = NULL;
	ALLEGRO_FONT *font_tempo = NULL;
	ALLEGRO_BITMAP *pmj2[4];
	Mini_jogo result;
	result.dinheiro_ganho = 0;
	result.display_close = false;
	result.progresso = 0;
	result.error = false;
	result.id = 2;
	int rodada = 0;
	float dinheiro = 0;
	char str[4];
	bool fim = false;
	int i = 0;
	int operando1;
	int operando2;
	int operando3;
	int operacao;
	char operacao_str[4] = {' ', '+','-','x' };
	bool resp = true;
	bool draw_resp = false;
	int cont_frames = 0;
	int tempo = 60;
	bool gerar_numeros = true;
	bool jogar_novamente = true;
	
	font1 = al_load_font("arial.ttf", 50, NULL);
	font_tempo = al_load_font("arial.ttf", 30, NULL);//testar

	str[0] = '\0';
	str[1] = '\0';
	str[2] = '\0';
	str[3] = '\0';
	if (!carregar_imagem_mj2(pmj2)) {
		cout << "error" << endl;
		result.error = true;
		fim = true;
		//return result;
	}
	do {
		al_flush_event_queue(fila_eventos);
		while (!fim) {
			al_wait_for_event(fila_eventos, &*evento);
			if (gerar_numeros == true) {
				operando1 = numeroAleatorio(0, 20);
				operando2 = numeroAleatorio(0, 20);
				operacao = numeroAleatorio(1, 3);
				if (operacao == 3) {
					operando1 = numeroAleatorio(0, 10);
					operando2 = numeroAleatorio(0, 10);
				}
				else if (operacao == 2) {
					if (operando2 > operando1) {
						int aux = operando2;
						operando2 = operando1;
						operando1 = aux;
					}
				}
				resp = true;
				gerar_numeros = false;
			}
			if (evento->type == ALLEGRO_EVENT_MOUSE_AXES) {
				//cout << "x:" << evento->mouse.x << "y:" << evento->mouse.y << endl;
			}
			if (evento->type == ALLEGRO_EVENT_DISPLAY_CLOSE) {//fechar a tela no botão
				fim = true;
				result.display_close = true;
				al_flush_event_queue(fila_eventos);
			}
			else if (evento->type == ALLEGRO_EVENT_KEY_DOWN || evento->keyboard.keycode == ALLEGRO_KEY_ENTER) {
				//manipular_entrada(*evento, str);
				if (evento->type == ALLEGRO_EVENT_TIMER) {
					cont_frames++;
					if (cont_frames == 60) {
						tempo--;
						cont_frames = 0;
						cout << tempo << endl;
					}
				}
				al_draw_bitmap(pmj2[0], 0, 0, 0);
				al_draw_textf(font1, al_map_rgb(0, 0, 0), 50, 362, NULL, "%d", operando1);
				al_draw_textf(font1, al_map_rgb(0, 0, 0), 200, 362, NULL, "%c", operacao_str[operacao]);
				al_draw_textf(font1, al_map_rgb(0, 0, 0), 279, 362, NULL, "%d", operando2);
				al_draw_textf(font1, al_map_rgb(0, 0, 0), 570, 362, NULL, "%s", str);
				al_draw_textf(font_tempo, al_map_rgb(0, 0, 0), 130, 110, NULL, "%d", tempo);
				al_draw_textf(font1, al_map_rgb(0, 0, 0), 102, 14, NULL, "%1.f", dinheiro);
				al_draw_textf(font1, al_map_rgb(0, 0, 0), 175, 710, NULL, "%d", rodada);
				al_flip_display();
				while (i < 3 && evento->keyboard.keycode != ALLEGRO_KEY_ENTER && resp == true) {
					if (evento->type == ALLEGRO_EVENT_TIMER) {
						cont_frames++;
						if (cont_frames == 60) {
							tempo--;
							cont_frames = 0;
							cout << tempo << endl;
						}
					}
					al_draw_bitmap(pmj2[0], 0, 0, 0);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 50, 362, NULL, "%d", operando1);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 200, 362, NULL, "%c", operacao_str[operacao]);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 279, 362, NULL, "%d", operando2);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 570, 362, NULL, "%s", str);
					al_draw_textf(font_tempo, al_map_rgb(0, 0, 0), 130, 110, NULL, "%d", tempo);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 102, 14, NULL, "%1.f", dinheiro);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 175, 710, NULL, "%d", rodada);
					al_flip_display();
					al_wait_for_event(fila_eventos, &*evento);
					if (evento->type == ALLEGRO_EVENT_KEY_CHAR) {
						cout << evento->keyboard.unichar << endl;
						if (evento->keyboard.unichar == 8 && i >= 0) {
							str[i] = '\0';
							if (i > 0)
								i--;
						}
						else {
							str[i] = evento->keyboard.unichar;
							i++;
						}
						al_draw_bitmap(pmj2[0], 0, 0, 0);
						al_draw_textf(font1, al_map_rgb(0, 0, 0), 50, 362, NULL, "%d", operando1);
						al_draw_textf(font1, al_map_rgb(0, 0, 0), 200, 362, NULL, "%c", operacao_str[operacao]);
						al_draw_textf(font1, al_map_rgb(0, 0, 0), 279, 362, NULL, "%d", operando2);
						al_draw_textf(font1, al_map_rgb(0, 0, 0), 570, 362, NULL, "%s", str);
						al_draw_textf(font_tempo, al_map_rgb(0, 0, 0), 130, 110, NULL, "%d", tempo);
						al_draw_textf(font1, al_map_rgb(0, 0, 0), 102, 14, NULL, "%1.f", dinheiro);
						al_draw_textf(font1, al_map_rgb(0, 0, 0), 175, 710, NULL, "%d", rodada);
						al_flip_display();
					}
				}
				str[i] = '\0';
				sscanf(str, "%d", &operando3);
				if (resposta(operando1, operando2, operando3, operacao) == true) {
					al_draw_bitmap(pmj2[1], 0, 0, 0);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 50, 362, NULL, "%d", operando1);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 279, 362, NULL, "%d", operando2);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 570, 362, NULL, "%s", str);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 102, 14, NULL, "%1.f", dinheiro);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 175, 710, NULL, "%d", rodada);
					al_flip_display();
					al_rest(0.900000000);
					cout << "acertou!!" << endl;
					str[0] = '\0';
					str[1] = '\0';
					str[2] = '\0';
					str[3] = '\0';
					gerar_numeros = true;
					resp = false;
					tempo = 60;
					dinheiro++;
					rodada++;
				}
				else if ((resposta(operando1, operando2, operando3, 1) != true)) {
					al_draw_bitmap(pmj2[2], 0, 0, 0);
					al_flip_display();
					al_rest(0.900000000);
					fim = true;
					resp = false;
				}
				i = 0;
			}
			
		   else if (evento->type == ALLEGRO_EVENT_TIMER) {
				cont_frames++;
				if (cont_frames == 60) {
					tempo--;
					cont_frames = 0;
					cout << tempo << endl;
				}
				if (tempo > 0) {
					al_draw_bitmap(pmj2[0], 0, 0, 0);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 50, 362, NULL, "%d", operando1);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 200, 362, NULL, "%c", operacao_str[operacao]);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 279, 362, NULL, "%d", operando2);
					//al_draw_textf(font1, al_map_rgb(0, 0, 0), 500, 362, NULL, "%s", str);
					al_draw_textf(font_tempo, al_map_rgb(0, 0, 0), 130, 110, NULL, "%d", tempo);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 102, 14, NULL, "%1.f", dinheiro);
					al_draw_textf(font1, al_map_rgb(0, 0, 0), 175, 710, NULL, "%d", rodada);
					al_flip_display();
				}
				else {
					fim = true;
					resp = false;
				}
			}
			al_flip_display();
		}
		if (result.display_close == false) {
			al_draw_bitmap(pmj2[2], 0, 0, 0);
			al_draw_textf(font1, al_map_rgb(0, 0, 0), 102, 14, NULL, "%1.f", dinheiro);
			al_flip_display();
			al_wait_for_event(fila_eventos, &*evento);
			if (evento->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				if (evento->mouse.button & 1 && evento->mouse.x >= 293 && evento->mouse.x <= 463
					&& evento->mouse.y >= 579 && evento->mouse.y <= 658) {
					jogar_novamente = true;
					fim = false;
					result.dinheiro_ganho += dinheiro;
					result.progresso += rodada;
					dinheiro = 0;
					cont_frames = 0;
					tempo = 60;
					gerar_numeros = true;
					jogar_novamente = true;
					i = 0;
				}
			}
			else if (evento->mouse.button & 1 && evento->mouse.x >= 598 && evento->mouse.x <= 765
				&& evento->mouse.y >= 579 && evento->mouse.y <= 658) {
				jogar_novamente = false;
			}
		}
		else {
			jogar_novamente = false;
		}
	}while (jogar_novamente == true);
	al_flush_event_queue(fila_eventos);
	al_destroy_bitmap(pmj2[0]);
	al_destroy_bitmap(pmj2[1]);
	al_destroy_bitmap(pmj2[2]);
	al_destroy_font(font1);
	al_destroy_font(font_tempo);
	result.dinheiro_ganho += dinheiro;
	result.progresso += rodada;
	return result;
}

bool carregar_imagem_mj2(ALLEGRO_BITMAP *pmj2[]) {

	pmj2[0] = al_load_bitmap("mini_jogos\\mj2\\mj2_1.png");
	if (!pmj2[0]) {
		return false;
	}
	pmj2[1] = al_load_bitmap("mini_jogos\\mj2\\mj2_2.png");//azul
	if (!pmj2[1]) {
		al_destroy_bitmap(pmj2[0]);
		return false;
	}
	pmj2[2] = al_load_bitmap("mini_jogos\\mj2\\mj2_3.png");//amarelo
	if (!pmj2[2]) {
		al_destroy_bitmap(pmj2[0]);
		al_destroy_bitmap(pmj2[1]);
		return false;
	}

	return true;
}
