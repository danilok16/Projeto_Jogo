#include"types.h"
#include"calc.h"



bool carregar_imagem_mj3(ALLEGRO_BITMAP *pagina[], ALLEGRO_BITMAP **personagem_1, ALLEGRO_BITMAP *obejtos[], ALLEGRO_BITMAP **sprite_som);

void animacao_mj3(ALLEGRO_BITMAP *pagina[], ALLEGRO_BITMAP *personagem_1, ALLEGRO_BITMAP *objeto[], ALLEGRO_FONT *text, float x, float y, int angulo, int obj, ALLEGRO_BITMAP *sprite_som, int cont_sprite_som);

int validar_palavra(char palavra_1[], char palavra_2[], char letra);

bool buscar_letra(char palavra_1[], char letra);

bool verificar_qtd_letra(char palavra_1[], char palavra_2[], char letra);

void set_palavra(char *str, int n);

void inicializar_string(char palavra_1[], char *str);

void inserir_letra_orndenado(char palavra_1[], char *str, char letra);

void tela_fim_de_jogo_3(ALLEGRO_BITMAP *pmj3[], ALLEGRO_FONT *font_status, float dinheiro);

bool selecionar_musica(ALLEGRO_AUDIO_STREAM **musica, int *vet);

bool carregar_efeitos_sonoros_mj3(ALLEGRO_SAMPLE *efeitos[]);

void inicializar_vetor(int *vet, int tam);

int numero_aletorio_nao_repetido(int *vet, int tam);


enum TECLAS { CIMA, BAIXO, DIREITA, ESQUERDA };

enum OBJETOS{CARRO, BONECA, SUBMARINO, ROBO, DINOSSAURO, APITO, BOLA, TREM, PIPA};


Mini_jogo mini_jogo_chuva_de_letras(ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_EVENT *evento, ALLEGRO_TIMER *temporizador) {
	float FPS = 60;

	ALLEGRO_AUDIO_STREAM *musica = NULL;
	ALLEGRO_SAMPLE *efeitos[3];

	ALLEGRO_BITMAP *pagina[3];
	ALLEGRO_BITMAP *personagem_1 = NULL;
	ALLEGRO_BITMAP *sprite_som = NULL;

	ALLEGRO_BITMAP *objetos[20]; 
	ALLEGRO_FONT *text = NULL;
	ALLEGRO_FONT *texto_final = NULL;
	ALLEGRO_FONT *text2 = NULL;
	ALLEGRO_FONT *text3 = NULL;
	bool fim = false;

	Mini_jogo result;
	result.dinheiro_ganho = 0;
	result.progresso = 0;
	result.error = false;
	result.id = 3;
	result.display_close = false;

	int selecionar_obj = numeroAleatorio(0, 8);
	bool teclas[] = { false, false, false, false };
	float pos_x = 0, pos_y = 660; //posicao personagem
	int i = 0;
	char palavra_obj[50];set_palavra(palavra_obj, selecionar_obj);;
	char palavra[50];
	char palavra_final[50];inicializar_string(palavra_obj, palavra_final);
	int n = 0;
	int velocidade = 7;
	int vidas = 10;
	float dinheiro = 0;
	int rodada = 1;
	bool jogar_novamente = true;
	bool novo = false;
	int cont_sprit_som = 0;
	int vet[13]; inicializar_vetor(vet, 12);//musica
	int vet2[9];  inicializar_vetor(vet2, 9);
	int cont_music = 0;
	int angulo = 0;
	int raio = 0;

	float pos_x_text_1 = -400, pos_y_text_1 = 350;
	int cont_pos_text_1 = 0;

	float pos_x_text_2 = 1024, pos_y_text_2 = 300;
	int cont_pos_text_2 = 0;

	float pos_x1 = numeroAleatorio(0, 760);
	float pos_y1 = -15;

	float pos_x2 = numeroAleatorio(0, 760);
	float pos_y2 = -100;

	float pos_x3 = numeroAleatorio(0, 760);
	float pos_y3 = -300;

	float pos_x4 = numeroAleatorio(0, 760);
	float pos_y4 = -500;

	float pos_x5 = numeroAleatorio(0, 760);
	float pos_y5 = -700;

	float pos_x6 = numeroAleatorio(0, 760);
	float pos_y6 = -900;

	float pos_x7 = numeroAleatorio(0, 760);
	float pos_y7 = -1100;

	bool desenhar_l1 = true;
	bool desenhar_l2 = true;
	bool desenhar_l3 = true;
	bool desenhar_l4 = true;
	bool desenhar_l5 = true;
	bool desenhar_l6 = true;
	bool desenhar_l7 = true;
	bool desenhar_text_1 = true;
	bool desenhar_text_2 = false;
	int velocidade_text = 3;

	char letras[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','x','y','z','1'};
	int n1, n2, n3, n4, n5, n6, n7;
	n1 = numeroAleatorio(0, 24);
	n2 = numeroAleatorio(0, 24);
	n3 = numeroAleatorio(0, 24);
	n4 = numeroAleatorio(0, 24);
	n5 = numeroAleatorio(0, 24);
	n6 = numeroAleatorio(0, 24);
	n7 = numeroAleatorio(0, 24);
	
	int r1, g1, b1;//cores
	r1 = numeroAleatorio(20, 255);
	g1 = numeroAleatorio(20, 255);
	b1 = numeroAleatorio(20, 255);
	int r2, g2, b2;//cores
	r2 = numeroAleatorio(20, 255);
	g2 = numeroAleatorio(20, 255);
	b2 = numeroAleatorio(20, 255);
	int r3, g3, b3;//cores
	r3 = numeroAleatorio(20, 255);
	g3 = numeroAleatorio(20, 255);
	b3 = numeroAleatorio(20, 255);
	int r4, g4, b4;//cores
	r4 = numeroAleatorio(20, 255);
	g4 = numeroAleatorio(20, 255);
	b4 = numeroAleatorio(20, 255);
	int r5, g5, b5;//cores
	r5 = numeroAleatorio(20, 255);
	g5 = numeroAleatorio(20, 255);
	b5 = numeroAleatorio(20, 255);
	int r6, g6, b6;//cores
	r6 = numeroAleatorio(20, 255);
	g6 = numeroAleatorio(20, 255);
	b6 = numeroAleatorio(20, 255);
	int r7, g7, b7;//cores
	r7 = numeroAleatorio(20, 255);
	g7 = numeroAleatorio(20, 255);
	b7 = numeroAleatorio(20, 255);

	selecionar_musica(&musica, vet);
	if (!carregar_imagem_mj3(pagina, &personagem_1, objetos, &sprite_som)) {
		result.error = true;
		return result;
	}
	if (!carregar_efeitos_sonoros_mj3(efeitos)) {
		result.error = true;
		return result;
	}

	else {
		text = al_load_font("fontes\\mj3_fonte_3.ttf",80,NULL);
		if (!text) {
			al_destroy_bitmap(pagina[1]);
			result.error = true;
			return result;
		}
		text2 = al_load_font("fontes\\mj3_fonte_1.ttf", 70, NULL);
		if (!text2) {
			al_destroy_bitmap(pagina[1]);
			al_destroy_font(text);
			result.error = true;
			return result;
		}
		text3 = al_load_font("fontes\\mj3_fonte_3.ttf", 40, NULL);
		if (!text3) {
			al_destroy_bitmap(pagina[1]);
			al_destroy_font(text);
			al_destroy_font(text2);
			result.error = true;
			return result;
		}
		texto_final = al_load_font("fontes\\mj3_fonte_3.ttf", 60, NULL);
		if (!texto_final) {
			al_destroy_bitmap(pagina[1]);
			al_destroy_font(text);
			al_destroy_font(text2);
			al_destroy_font(text3);
			result.error = true;
			return result;
		}

	}

	do {

	while (!fim) {
		al_wait_for_event(fila_eventos, &*evento);//fila de eventos
		if (evento->type == ALLEGRO_EVENT_MOUSE_AXES) {
			//cout << "x:" << evento->mouse.x << "y:" << evento->mouse.y << endl;
		}
		else if (evento->type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			fim = true;
			result.display_close = true;
			al_flush_event_queue(fila_eventos);
		}
		
		if (evento->type == ALLEGRO_EVENT_KEY_DOWN) {
			if (evento->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				fim = true;
			}
			switch (evento->keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				teclas[CIMA] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				teclas[BAIXO] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				teclas[DIREITA] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				teclas[ESQUERDA] = true;
				break;
			}
			
		}
		//se a tecla parar de ser pressionada
		if (evento->type == ALLEGRO_EVENT_KEY_UP) {
			switch (evento->keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				teclas[CIMA] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				teclas[BAIXO] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				teclas[DIREITA] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				teclas[ESQUERDA] = false;
				break;
			}
		}

		pos_x -= teclas[ESQUERDA] * 5;
		pos_x += teclas[DIREITA] * 5;
		angulo -= teclas[ESQUERDA] * 1;
		angulo += teclas[DIREITA] * 1;

		if (pos_x < 0) {
			pos_x = 0;
		}
		if (pos_x > 680) {
			pos_x = 680;
		}

		if (evento->type == ALLEGRO_EVENT_TIMER) {

			cont_sprit_som++;
			if (cont_sprit_som > 80) {
				cont_sprit_som = 0;
			}

			pos_y1 += velocidade;
			pos_y2 += velocidade;
			pos_y3 += velocidade;
			pos_y4 += velocidade;
			pos_y5 += velocidade;
			pos_y6 += velocidade;
			pos_y7 += velocidade;
			if(desenhar_text_1)
			pos_x_text_1 += velocidade_text;
			if (desenhar_text_2)
			pos_x_text_2 -= velocidade_text;
			
			if (pos_y1 > 770) {
				pos_x1 = numeroAleatorio(0, 760);
				pos_y1 = -15;
				n1 = numeroAleatorio(0, 24);
				desenhar_l1 = true;
				r1 = numeroAleatorio(20, 255);
				g1 = numeroAleatorio(20, 255);
				b1 = numeroAleatorio(20, 255);

			}
			if (pos_y2 > 770) {
				pos_x2 = numeroAleatorio(0, 760);
				pos_y2 = -100;
				n2 = numeroAleatorio(0, 24);
				desenhar_l2 = true;
				r2 = numeroAleatorio(20, 255);
				g2 = numeroAleatorio(20, 255);
				b2 = numeroAleatorio(20, 255);
			}

			if (pos_y3 > 770) {
				pos_x3 = numeroAleatorio(0, 760);
				pos_y3 = -300;
				n3 = numeroAleatorio(0, 24);
				desenhar_l3 = true;
				r3 = numeroAleatorio(20, 255);
				g3 = numeroAleatorio(20, 255);
				b3 = numeroAleatorio(20, 255);
			}

			if (pos_y4 > 770) {
				pos_x4 = numeroAleatorio(0, 760);
				pos_y4 = -500;
				n4 = numeroAleatorio(0, 24);
				desenhar_l4 = true;
				r4 = numeroAleatorio(20, 255);
				g4 = numeroAleatorio(20, 255);
				b4 = numeroAleatorio(20, 255);
			}
			if (pos_y5 > 770) {
				pos_x5 = numeroAleatorio(0, 760);
				pos_y5 = -700;
				n5 = numeroAleatorio(0, 24);
				desenhar_l5 = true;
				r5 = numeroAleatorio(20, 255);
				g5 = numeroAleatorio(20, 255);
				b5 = numeroAleatorio(20, 255);
			}
			if (pos_y6 > 770) {
				pos_x6 = numeroAleatorio(0, 760);
				pos_y6 = -900;
				n6 = numeroAleatorio(0, 24);
				desenhar_l6 = true;
				r6 = numeroAleatorio(20, 255);
				g6 = numeroAleatorio(20, 255);
				b6 = numeroAleatorio(20, 255);
			}
			if (pos_y7 > 770) {
				pos_x7 = numeroAleatorio(0, 760);
				pos_y7 = -1100;
				n7 = numeroAleatorio(0, 24);
				desenhar_l7 = true;
				r7 = numeroAleatorio(20, 255);
				g7 = numeroAleatorio(20, 255);
				b7 = numeroAleatorio(20, 255);
			}
			if (pos_x_text_1 > 1024) {
				desenhar_text_1 = false;
				pos_x_text_1 = -400;
			}

			if (pos_x_text_2 < -600) {
				desenhar_text_2 = false;
				pos_x_text_2 = 1024;
			}

			/*-------------------------------COLISÕES-----------------------------------------*/
			if (pos_x1 >= pos_x && pos_x1 <= pos_x + 95 && pos_y1 >= pos_y && pos_y1 <= pos_y + 95 && desenhar_l1 == true) {
				desenhar_l1 = false;
				palavra[i] = letras[n1];
				palavra[i + 1] = '\0';
				i++;
				if (validar_palavra(palavra_obj, palavra, letras[n1])==0) {
					vidas--;
					i--;
					al_play_sample(efeitos[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else if (validar_palavra(palavra_obj, palavra, letras[n1]) == 2) {
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n1]);
					al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else {
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n1]);
					novo = true;
				}
			}
			if (pos_x2 >= pos_x && pos_x2 <= pos_x + 95 && pos_y2 >= pos_y && pos_y2 <= pos_y + 95 && desenhar_l2 == true) {
				desenhar_l2 = false;
				palavra[i] = letras[n2];
				palavra[i + 1] = '\0';
				i++;
				if (validar_palavra(palavra_obj, palavra, letras[n2])==0) {
					vidas--;
					i--;
					al_play_sample(efeitos[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else if (validar_palavra(palavra_obj, palavra, letras[n2]) == 2){
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n2]);
					al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

				}
				else {
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n2]);
					novo = true;
				}
			}
			if (pos_x3 >= pos_x && pos_x3 <= pos_x + 95 && pos_y3 >= pos_y && pos_y3 <= pos_y + 95 && desenhar_l3 == true) {
				desenhar_l3 = false;
				palavra[i] = letras[n3];
				palavra[i + 1] = '\0';
				i++;
				if (validar_palavra(palavra_obj, palavra, letras[n3])==0) {
					vidas--;
					i--;
					al_play_sample(efeitos[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else if (validar_palavra(palavra_obj, palavra, letras[n3]) == 2) {
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n3]);
					al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else {
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n3]);
					novo = true;
				}
				//cout << palavra << endl;
			}
			if (pos_x4 >= pos_x && pos_x4 <= pos_x + 95 && pos_y4 >= pos_y && pos_y4 <= pos_y + 95 && desenhar_l4 == true) {
				desenhar_l4 = false;
				palavra[i] = letras[n4];
				palavra[i + 1] = '\0';
				i++;
				if (validar_palavra(palavra_obj, palavra, letras[n4])==0) {
					vidas--;
					i--;
					al_play_sample(efeitos[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else if (validar_palavra(palavra_obj, palavra, letras[n4]) == 2){
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n4]);
					al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else {
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n4]);
					novo = true;
				}
			}
			if (pos_x5 >= pos_x && pos_x5 <= pos_x + 95 && pos_y5 >= pos_y && pos_y5 <= pos_y + 95 && desenhar_l5 == true) {
				desenhar_l5 = false;
				palavra[i] = letras[n5];
				palavra[i + 1] = '\0';
				i++;
				if (validar_palavra(palavra_obj, palavra, letras[n5])==0) {
					vidas--;
					i--;
					al_play_sample(efeitos[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else if (validar_palavra(palavra_obj, palavra, letras[n5]) == 2){
				
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n5]);
					al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else {
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n4]);
					novo = true;
				}
			}
			if (pos_x6 >= pos_x && pos_x6 <= pos_x + 95 && pos_y6 >= pos_y && pos_y6 <= pos_y + 95 && desenhar_l6 == true) {
				desenhar_l6 = false;
				palavra[i] = letras[n6];
				palavra[i + 1] = '\0';
				i++;
				if (validar_palavra(palavra_obj, palavra, letras[n6])==0) {
					vidas--;
					i--;
					al_play_sample(efeitos[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else if (validar_palavra(palavra_obj, palavra, letras[n6]) == 2){
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n6]);
					al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else {
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n6]);
					novo = true;
				}
			}
			if (pos_x7 >= pos_x && pos_x7 <= pos_x + 95 && pos_y7 >= pos_y && pos_y7 <= pos_y + 95 && desenhar_l7 == true) {
				desenhar_l7 = false;
				palavra[i] = letras[n7];
				palavra[i + 1] = '\0';
				i++;
				if (validar_palavra(palavra_obj, palavra, letras[n7]) == 0) {
					vidas--;
					i--;
					al_play_sample(efeitos[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else if (validar_palavra(palavra_obj, palavra, letras[n7]) == 2) {
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n7]);
					al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				else {
					inserir_letra_orndenado(palavra_obj, palavra_final, letras[n7]);
					novo = true;
				}
				//cout << palavra << endl;
			}

			if (novo == true) {
				selecionar_obj = numero_aletorio_nao_repetido(vet2, 8);
				set_palavra(palavra_obj, selecionar_obj);
				inicializar_string(palavra_obj, palavra);
				inicializar_string(palavra_obj, palavra_final);
				i = 0;
				//velocidade++;
				rodada++;
				dinheiro += 5;
				if (vidas < 10) {
					vidas++;
				}
				cont_music++;
				if (cont_music == 12) {
					inicializar_vetor(vet, 12);
				}
				/*for (int i = 0; i < 9; i++) {
					cout << "vetor: " << vet2[i] << ",";
				}*/
				desenhar_text_1 = true;
				desenhar_text_2 = true;
				pos_x_text_1 = -400;
				pos_x_text_2 = 1024;

				selecionar_musica(&musica, vet);
				FPS += 15.0;

				al_set_timer_speed(temporizador, 1.0 / FPS);
				novo = false;

			}
			if (vidas == 0) {
				fim = true;
			}
		}
		animacao_mj3(pagina, personagem_1, objetos, text, pos_x, pos_y, angulo, selecionar_obj, sprite_som, cont_sprit_som);
		if (desenhar_l1) {
			al_draw_textf(text, al_map_rgb(r1, g1, b1), pos_x1, pos_y1, NULL, "%c", letras[n1]);
		}
		if (desenhar_l2) {
			al_draw_textf(text, al_map_rgb(r2, g2, b2), pos_x2, pos_y2, NULL, "%c", letras[n2]);
		}
		if (desenhar_l3) {
			al_draw_textf(text, al_map_rgb(r3, g3, b3), pos_x3, pos_y3, NULL, "%c", letras[n3]);
		}
		if (desenhar_l4) {
			al_draw_textf(text, al_map_rgb(r4, g4, b4), pos_x4, pos_y4, NULL, "%c", letras[n4]);
		}
		if (desenhar_l5) {
			al_draw_textf(text, al_map_rgb(r5, g5, b5), pos_x5, pos_y5, NULL, "%c", letras[n5]);
		}
		if (desenhar_l6) {
			al_draw_textf(text, al_map_rgb(r6, g6, b6), pos_x6, pos_y6, NULL, "%c", letras[n6]);
		}
		if (desenhar_l7) {
			al_draw_textf(text, al_map_rgb(r7, g7, b7), pos_x7, pos_y7, NULL, "%c", letras[n7]);
		}
		if (desenhar_text_1) {
			al_draw_text(text3, al_map_rgb(numeroAleatorio(50,255), numeroAleatorio(50,255), numeroAleatorio(50,255)), pos_x_text_1, pos_y_text_1,0, "FORME A PALAVRA!!");
		}
		if (desenhar_text_2) {
			al_draw_text(text3, al_map_rgb(numeroAleatorio(50, 255), numeroAleatorio(50, 255), numeroAleatorio(50, 255)), pos_x_text_2, pos_y_text_2, 0, "*PARABENS*");
		}

		al_draw_textf(texto_final, al_map_rgb(r4, b4, g4), 770, 110, NULL, "%s", palavra_final);
		al_draw_textf(text2, al_map_rgb(r1, b1, g1), 850, 190, NULL, "%d", vidas);
		al_draw_textf(text2, al_map_rgb(r2, b2, g2), 850, 258, NULL, "%1.f", dinheiro);
		al_draw_textf(text2, al_map_rgb(r3, b3, g3), 850, 340, NULL, "%d", rodada);
		al_draw_rotated_bitmap(personagem_1, 50, 50, pos_x + 45, pos_y + 30, angulo, 0);
		al_flip_display();
	}
	if (result.display_close == false) {
		tela_fim_de_jogo_3(pagina, text2, dinheiro);
		al_wait_for_event(fila_eventos, &*evento);
		if (evento->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (evento->mouse.button & 1 && evento->mouse.x >= 293 && evento->mouse.x <= 463
				&& evento->mouse.y >= 579 && evento->mouse.y <= 658) {
				jogar_novamente = true;
				fim = false;
				result.dinheiro_ganho += dinheiro;
				result.progresso += rodada * 2;
				dinheiro = 0;
				rodada = 1;
				vidas = 10;
				velocidade++;
				FPS = 60.0;
				al_set_timer_speed(temporizador, 1.0 / FPS);
				selecionar_musica(&musica, vet);
				selecionar_obj = numero_aletorio_nao_repetido(vet2, 8);
				set_palavra(palavra_obj, selecionar_obj);
				inicializar_string(palavra_obj, palavra);
				inicializar_string(palavra_obj, palavra_final);
				i = 0;
				
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

	al_destroy_bitmap(pagina[1]);
	al_destroy_bitmap(pagina[2]);
	al_destroy_bitmap(personagem_1);
	al_destroy_font(text);
	al_destroy_font(text2);
	al_destroy_font(text3);
	al_destroy_font(texto_final);
	al_drain_audio_stream(musica);
	al_destroy_audio_stream(musica);
	result.dinheiro_ganho += dinheiro;
	result.progresso += rodada * 2;
	FPS = 60.0;
	al_set_timer_speed(temporizador, 1.0 / FPS);
	return result;
}


bool carregar_imagem_mj3(ALLEGRO_BITMAP *pagina[], ALLEGRO_BITMAP **personagem_1, ALLEGRO_BITMAP *objeto[], ALLEGRO_BITMAP **sprite_som) {

	pagina[1] = al_load_bitmap("mini_jogos\\mj3\\mj3_pg_1.png");
	if (!pagina[1]) {
		return false;
	}
	pagina[2] = al_load_bitmap("mini_jogos\\mj3\\mj3_pg_2.png");
	if (!pagina[2]) {
		al_destroy_bitmap(pagina[1]);
		return false;
	}
	*personagem_1 = al_load_bitmap("mini_jogos\\mj3\\mj3_personagem_1.png");
	if (!*personagem_1) {
		al_destroy_bitmap(pagina[1]);
		al_destroy_bitmap(pagina[2]);
		return false;
	}
	objeto[CARRO] = al_load_bitmap("mini_jogos\\mj3\\mj3_im_1.png");
	if (!objeto[CARRO]) {
		al_destroy_bitmap(pagina[1]);
		al_destroy_bitmap(pagina[2]);
		al_destroy_bitmap(*personagem_1);
		return false;
	}
	objeto[BONECA] = al_load_bitmap("mini_jogos\\mj3\\mj3_im_2.png");
	if (!objeto[BONECA]) {
		al_destroy_bitmap(pagina[1]);
		al_destroy_bitmap(pagina[2]);
		al_destroy_bitmap(*personagem_1);
		al_destroy_bitmap(objeto[CARRO]);
		return false;
	}
	objeto[SUBMARINO] = al_load_bitmap("mini_jogos\\mj3\\mj3_im_3.png");
	if (!objeto[SUBMARINO]) {
		al_destroy_bitmap(pagina[1]);
		al_destroy_bitmap(pagina[2]);
		al_destroy_bitmap(*personagem_1);
		al_destroy_bitmap(objeto[CARRO]);
		al_destroy_bitmap(objeto[BONECA]);
		return false;
	}
	objeto[ROBO] = al_load_bitmap("mini_jogos\\mj3\\mj3_im_4.png");
	if (!objeto[ROBO]) {
		al_destroy_bitmap(pagina[1]);
		al_destroy_bitmap(pagina[2]);
		al_destroy_bitmap(*personagem_1);
		al_destroy_bitmap(objeto[CARRO]);
		al_destroy_bitmap(objeto[BONECA]);
		al_destroy_bitmap(objeto[SUBMARINO]);
		return false;
	}
	objeto[DINOSSAURO] = al_load_bitmap("mini_jogos\\mj3\\mj3_im_5.png");
	if (!objeto[DINOSSAURO]) {
		al_destroy_bitmap(pagina[1]);
		al_destroy_bitmap(pagina[2]);
		al_destroy_bitmap(*personagem_1);
		al_destroy_bitmap(objeto[CARRO]);
		al_destroy_bitmap(objeto[BONECA]);
		al_destroy_bitmap(objeto[SUBMARINO]);
		al_destroy_bitmap(objeto[ROBO]);
		return false;
	}
	objeto[APITO] = al_load_bitmap("mini_jogos\\mj3\\mj3_im_6.png");
	if (!objeto[APITO]) {
		al_destroy_bitmap(pagina[1]);
		al_destroy_bitmap(pagina[2]);
		al_destroy_bitmap(*personagem_1);
		al_destroy_bitmap(objeto[CARRO]);
		al_destroy_bitmap(objeto[BONECA]);
		al_destroy_bitmap(objeto[SUBMARINO]);
		al_destroy_bitmap(objeto[ROBO]);
		al_destroy_bitmap(objeto[DINOSSAURO]);
		return false;
	}
	objeto[BOLA] = al_load_bitmap("mini_jogos\\mj3\\mj3_im_7.png");
	if (!objeto[BOLA]) {
		al_destroy_bitmap(pagina[1]);
		al_destroy_bitmap(pagina[2]);
		al_destroy_bitmap(*personagem_1);
		al_destroy_bitmap(objeto[CARRO]);
		al_destroy_bitmap(objeto[BONECA]);
		al_destroy_bitmap(objeto[SUBMARINO]);
		al_destroy_bitmap(objeto[ROBO]);
		al_destroy_bitmap(objeto[DINOSSAURO]);
		al_destroy_bitmap(objeto[APITO]);
		return false;
	}

	objeto[TREM] = al_load_bitmap("mini_jogos\\mj3\\mj3_im_8.png");
	if (!objeto[TREM]) {
		al_destroy_bitmap(pagina[1]);
		al_destroy_bitmap(pagina[2]);
		al_destroy_bitmap(*personagem_1);
		al_destroy_bitmap(objeto[CARRO]);
		al_destroy_bitmap(objeto[CARRO]);
		al_destroy_bitmap(objeto[BONECA]);
		al_destroy_bitmap(objeto[SUBMARINO]);
		al_destroy_bitmap(objeto[ROBO]);
		al_destroy_bitmap(objeto[DINOSSAURO]);
		al_destroy_bitmap(objeto[APITO]);
		al_destroy_bitmap(objeto[BOLA]);
		return false;
	}
	objeto[PIPA] = al_load_bitmap("mini_jogos\\mj3\\mj3_im_9.png");
	if (!objeto[PIPA]) {
		al_destroy_bitmap(pagina[1]);
		al_destroy_bitmap(pagina[2]);
		al_destroy_bitmap(*personagem_1);
		al_destroy_bitmap(objeto[CARRO]);
		al_destroy_bitmap(objeto[CARRO]);
		al_destroy_bitmap(objeto[BONECA]);
		al_destroy_bitmap(objeto[SUBMARINO]);
		al_destroy_bitmap(objeto[ROBO]);
		al_destroy_bitmap(objeto[DINOSSAURO]);
		al_destroy_bitmap(objeto[APITO]);
		al_destroy_bitmap(objeto[BOLA]);
		al_destroy_bitmap(objeto[TREM]);
		return false;
	}
	*sprite_som = al_load_bitmap("mini_jogos\\mj3\\mj3_sprite_som.png");
	if (!*sprite_som) {
		al_destroy_bitmap(pagina[1]);
		al_destroy_bitmap(pagina[2]);
		al_destroy_bitmap(*personagem_1);
		al_destroy_bitmap(objeto[CARRO]);
		al_destroy_bitmap(objeto[CARRO]);
		al_destroy_bitmap(objeto[BONECA]);
		al_destroy_bitmap(objeto[SUBMARINO]);
		al_destroy_bitmap(objeto[ROBO]);
		al_destroy_bitmap(objeto[DINOSSAURO]);
		al_destroy_bitmap(objeto[APITO]);
		al_destroy_bitmap(objeto[BOLA]);
		al_destroy_bitmap(objeto[TREM]);
		al_destroy_bitmap(objeto[PIPA]);
		return false;

	}
	return true;
	
}

void animacao_mj3(ALLEGRO_BITMAP *pagina[], ALLEGRO_BITMAP *personagem_1, ALLEGRO_BITMAP *objeto[], ALLEGRO_FONT *text, float x, float y, int angulo,  int obj, ALLEGRO_BITMAP *sprite_som, int cont_sprite_som){
	float regiao_x_folha = 0;
	float regiao_y_folha = 0;
	float largura_sprite = 252;
	float altura_sprite = 429;

	al_draw_bitmap(pagina[1],0,0,0);
	al_draw_rotated_bitmap(personagem_1,50,50, x+45, y+30,angulo, 0);

	al_draw_bitmap(objeto[obj], 830, 15, 0);
	if (cont_sprite_som >= 0 && cont_sprite_som <= 20) {
		al_draw_bitmap_region(sprite_som, regiao_x_folha, regiao_y_folha, largura_sprite, altura_sprite, 766, 342, 0);
	}
	else if (cont_sprite_som >= 21 && cont_sprite_som <= 40) {
		regiao_x_folha = 1 * largura_sprite;
		al_draw_bitmap_region(sprite_som, regiao_x_folha, regiao_y_folha, largura_sprite, altura_sprite, 766, 342, 0);

	}
	else if (cont_sprite_som >= 41 && cont_sprite_som <= 60) {
		regiao_x_folha = 2 * largura_sprite;
		al_draw_bitmap_region(sprite_som, regiao_x_folha, regiao_y_folha, largura_sprite, altura_sprite, 766, 342, 0);

	}
	else if (cont_sprite_som >= 61 && cont_sprite_som <= 80) {
		regiao_x_folha = 3 * largura_sprite;
		al_draw_bitmap_region(sprite_som, regiao_x_folha, regiao_y_folha, largura_sprite, altura_sprite, 766, 342, 0);

	}
}
 
int validar_palavra(char palavra_1[], char palavra_2[], char letra) {
	int tam_1;
	int tam_2;

	if (buscar_letra(palavra_1, letra)) {
		if (verificar_qtd_letra(palavra_1, palavra_2, letra)) {
			tam_1 = strlen(palavra_1);
			tam_2 = strlen(palavra_2);
			if (tam_2 == tam_1) {
				return 1;
			}
			else if (tam_2 < tam_1) {
				return 2;
			}
		}
	}
	return 0;
}

bool buscar_letra(char palavra_1[], char letra) {
	int tam = strlen(palavra_1);

	for (int i = 0; i < tam; i++) {
		if (palavra_1[i] == letra) {
			return true;
		}
	}
	return false;
}

bool verificar_qtd_letra(char palavra_1[], char palavra_2[], char letra) {
	int tam = strlen(palavra_1);
	int qtd_1 = 0;
	int qtd_2 = 0;

	for (int i = 0; i < tam; i++) {
		if (palavra_1[i] == letra) {
			qtd_1++;
		}
	}
	tam = strlen(palavra_2) ;
	for (int i = 0; i < tam; i++) {
		if (palavra_2[i] == letra) {
			qtd_2++;
		}
	}
	if (qtd_2 > qtd_1) {
		return false;
	}
	return true;
}

void set_palavra(char *str, int n) {

	switch (n) {
	case CARRO:
		strcpy(str, "carro");
		break;
	case BONECA:
		strcpy(str, "boneca");
		break;
	case SUBMARINO:
		strcpy(str, "submarino");
		break;
	case ROBO:
		strcpy(str, "robo");
		break;
	case DINOSSAURO:
		strcpy(str, "dinossauro");
		break;
	case APITO:
		strcpy(str, "apito");
		break;
	case BOLA:
		strcpy(str, "bola");
		break;
	case TREM:
		strcpy(str, "trem");
		break;
	case PIPA:
		strcpy(str, "pipa");
		break;
	}

}

void inicializar_string(char palavra_1[], char *str) {
	int i = 0;
	for (i ; palavra_1[i] != '\0';i++) {
		str[i] = ' ';
	}

	str[i] = '\0';
	
}

void inserir_letra_orndenado(char palavra_1[], char *str, char letra) {
	int cont=0;

	cout << letra << endl;
	for (int i = 0; palavra_1[i] != '\0';i++) {
		if (palavra_1[i] == letra) {
			if (str[i] != letra) {
				str[i] = letra;
				break;
			}
			else {
				str[i] = letra;
			}
		}
	}

//	cout << palavra_1 << endl;
	//printf("%s\n", str);
	//cout << str << endl;

}

void tela_fim_de_jogo_3(ALLEGRO_BITMAP *pmj3[], ALLEGRO_FONT *font_status, float dinheiro) {

	al_draw_bitmap(pmj3[2], 0, 0, 0);
	al_draw_textf(font_status, al_map_rgb(0, 0, 0), 102, 1, NULL, "%1.f", dinheiro);
	al_flip_display();
}

bool selecionar_musica(ALLEGRO_AUDIO_STREAM **musica, int *vet) {
	int n = numero_aletorio_nao_repetido(vet, 11);

	if (*musica != NULL) {
		al_drain_audio_stream(*musica);
		al_destroy_audio_stream(*musica);
	}
	switch (n) {
	case 0:
		*musica = al_load_audio_stream("efeitos_sonoros\\mj3\\mj3_m_1.ogg", 4, 1024);
		if (!*musica) {
			return false;
		}
		break;
	case 1:
		*musica = al_load_audio_stream("efeitos_sonoros\\mj3\\mj3_m_2.ogg", 4, 1024);
		if (!*musica) {
			return false;
		}
		break;
	case 2:
		*musica = al_load_audio_stream("efeitos_sonoros\\mj3\\mj3_m_3.ogg", 4, 1024);
		if (!*musica) {
			return false;
		}
		break;
	case 3:
		*musica = al_load_audio_stream("efeitos_sonoros\\mj3\\mj3_m_4.ogg", 4, 1024);
		if (!*musica) {
			return false;
		}
		break;
	case 4:
		*musica = al_load_audio_stream("efeitos_sonoros\\mj3\\mj3_m_5.ogg", 4, 1024);
		if (!*musica) {
			return false;
		}
		break;
	case 5:
		*musica = al_load_audio_stream("efeitos_sonoros\\mj3\\mj3_m_6.ogg", 4, 1024);
		if (!*musica) {
			return false;
		}
		break;
	case 6:
		*musica = al_load_audio_stream("efeitos_sonoros\\mj3\\mj3_m_7.ogg", 4, 1024);
		if (!*musica) {
			return false;
		}
		break;
	case 7:
		*musica = al_load_audio_stream("efeitos_sonoros\\mj3\\mj3_m_8.ogg", 4, 1024);
		if (!*musica) {
			return false;
		}
		break;
	case 8:
		*musica = al_load_audio_stream("efeitos_sonoros\\mj3\\mj3_m_9.ogg", 4, 1024);
		if (!*musica) {
			return false;
		}
		break;
	case 9:
		*musica = al_load_audio_stream("efeitos_sonoros\\mj3\\mj3_m_10.ogg", 4, 1024);
		if (!*musica) {
			return false;
		}
		break;
	case 10:
		*musica = al_load_audio_stream("efeitos_sonoros\\mj3\\mj3_m_11.ogg", 4, 1024);
		if (!*musica) {
			return false;
		}
		break;
	case 11:
		*musica = al_load_audio_stream("efeitos_sonoros\\mj3\\mj3_m_12.ogg", 4, 1024);
		if (!*musica) {
			return false;
		}
		break;

	}
	al_attach_audio_stream_to_mixer(*musica, al_get_default_mixer());
	al_set_audio_stream_playmode(*musica, ALLEGRO_PLAYMODE_LOOP);
	return true;
}

bool carregar_efeitos_sonoros_mj3(ALLEGRO_SAMPLE *efeitos[]) {

	efeitos[0] = al_load_sample("efeitos_sonoros\\mj3\\mj3_errou.ogg");
	if (!efeitos[0]) {
		return false;
	}
	efeitos[1] = al_load_sample("efeitos_sonoros\\mj3\\mj3_acerto.ogg");
	if (!efeitos[1]) {
		return false;
	}

	return true;
}

void inicializar_vetor(int *vet, int tam) {
	for (int i = 0; i < tam; i++) {
		vet[i] = 0;
	}
}

int numero_aletorio_nao_repetido(int *vet, int tam) {
	int i, j;
	int n = numeroAleatorio(0, tam);
	bool flag = true;

	while (flag == true) {
		flag = false;
		for (int i = 0; i < tam; i++) {
			if (vet[i] == n) {
				flag = true;
				n = numeroAleatorio(0, tam);
			}
			else if (vet[i] == 0 && flag == false) {
				vet[i] = n;
				break;
			}
		}

	}
	return n;
}