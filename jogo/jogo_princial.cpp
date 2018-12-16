#include "types.h"
#include"animacao_principal.h"
#include"save_game.h"
#include"controle_progresso.h"
#include"calc.h"
#include"Queue.h"
#include"mini_jogos.h"

const float FPS = 60.0;
const int Largura_t = 1024;
const int Altura_t = 768;
//JANELA DO JOGO
ALLEGRO_DISPLAY *janela = NULL;

//EVENTOS
ALLEGRO_EVENT evento;

//FILA DE EVENTOS
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

//BACKGROUNDS
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *background_vetor[20];
ALLEGRO_BITMAP *tela_inicio = NULL;

ALLEGRO_BITMAP *tela_aviso[2];

//SPRITE PERSONAGENS
ALLEGRO_BITMAP *personagem[8];

//SPRITE ALIMENTOS
ALLEGRO_BITMAP *sprite_alimento = NULL;

//IMAGENS OBJETOS
ALLEGRO_BITMAP *objetos_cenario[3];
ALLEGRO_BITMAP *bolhas_banheiro = NULL;
ALLEGRO_BITMAP *sujeira_personagem = NULL;
ALLEGRO_BITMAP *acessorio = NULL;

//MUSICA E EFEITOS
ALLEGRO_AUDIO_STREAM *som_chuveiro = NULL;
ALLEGRO_SAMPLE *efeitos[3];

//FONTE
ALLEGRO_FONT *font_status = NULL;

//TEMPORIZADOR
ALLEGRO_TIMER *temporizador = NULL;

//SAVE GAME
ALLEGRO_FILE *save_personagem = criar_arquivo("save_personagem.data");
ALLEGRO_FILE *save_alimento = criar_arquivo("save_alimento.data");
ALLEGRO_FILE *save_cenario = criar_arquivo("save_background.data");
ALLEGRO_FILE *save_data = criar_arquivo("save_data.data");
ALLEGRO_FILE *save_aparencia = criar_arquivo("save_aparencia.data");

//FUNÇÕES E PROCEDIMENTOS
bool inicializar();
Personagem inicializar_struct_personagem();
Alimento inicializar_struct_alimento();
Cenario inicializar_struct_cenario();
TData inicializar_struct_data();
void inicializar_vetor_sprite_personagem();
bool carregar_sprite_personagem();

void finalizar_sprite_personagem();
bool carregar_imagens();
bool carregar_loja();
bool carregar_geladeira();
void error_msg(char *text);
void finalizar();
void finalizar_loja();
void finalizar_geladeira();
void trocar_background_principal(int cenario);
float calcular_raio(float x2, float y2, float x1, float y1);
void alterar_sprite_tamanho(Nome_sprite *nome_sprite, Personagem p);

void crescer_personagem(Personagem p);
void trocar_status_personagem(Personagem *p, TData ultima_data, Cenario c);

bool carregar_bolhas(Personagem p);
void finalizar_bolhas();

bool carregar_sujeira(Personagem p);
void finalizar_sujeira();

void comprar_comida(Personagem *p, Cenario c, int x, int y);
bool validar_compra(int dinheiro, int valor_item);
void comer(Personagem *p, INTERAGIR_COZINHA *interagir_alimento, Alimento *vet_alimento, int id_alimento);
int buscar_alimento(Alimento vet_alimento[], int id);

Mini_jogo selecionar_mini_jogo(int x, int y, ALLEGRO_EVENT *evento);

bool carregar_guarda_roupa();
void comprar_aparencia(Personagem *p, Cenario c, Aparencia *aparencia_vet, int x, int y);
bool buscar_aparencia(Aparencia *aparencia_vet, int id_aparencia);
void finalizar_guarda_roupa();

void trocar_cor_personagem(Personagem p, int cor);
void selecionar_cor_personagem(Personagem p, Cenario c, Aparencia *aparencia_vet, int x, int y);
void selecionar_acessorio_personagem(Personagem p, Cenario c, Aparencia *aparencia_vet, bool *des_acessorio, int *id_acessorio, int x, int y);
bool carregar_acessorio(Personagem p, int id_acessorio);

/*-----------------------------------------variaveis de abertura---------------------*/
bool loja_aberta = false;
bool status_aberto = false;
bool minijogos_aberto = false;

int main() {

	srand((unsigned)time(NULL));
	char texto[20];
	int con = 0;
	Personagem p;//personagem
	Alimento al;//alimento
	/*------------------inicializar vetor de alimento--------------------*/
	Alimento vet_al[40];
	int id_comida = 1;
	load_file_alimento_vet(save_alimento, vet_al);
	/*----------------------------------------------------------*/
	/*--------------------inicializar vetor de aparencia------------------*/
	Aparencia vet_ap[30];
	for (int i = 0; i < 29; i++) {
		vet_ap[i].id = 0;
	}
	load_file_aparencia(save_aparencia, vet_ap);
	/*----------------------------------------------------------*/
	Cenario c;//cenario
	TData d;//data

	float raio;
	int x1 = 528, y1 = 478;
	int limite_clic_x = 0, limite_clic_y = 0;
	INTERAGIR_COZINHA interagir_cozinha;
	interagir_cozinha.movimentar_alimento = false;
	interagir_cozinha.comer = false;
	interagir_cozinha.mastigar = false;
	interagir_cozinha.mastigando = 0;
	interagir_cozinha.abrir_geladeira = false;
	interagir_cozinha.alimento_mesa = false;

	INTERAGIR_BANHEIRO interagir_banheiro;
	interagir_banheiro.movimentar_sabonete = false;
	interagir_banheiro.ensaboar = false;
	interagir_banheiro.ligar_chuveiro = false;
	interagir_banheiro.qtd_bolhas = 0;
	interagir_banheiro.cont_frames = 0;

	ALIMENTO_DESENHO a;
	a.regiao_x_folha = 0;
	a.regiao_y_folha = 0;
	a.largura_sprite = 82;
	a.altura_sprite = 77;
	a.pos_sprite_x = 512;
	a.pos_sprite_y = 590;

	bool quarto_interruptor = false;
	bool exibirTexto = false;
	bool status_mouse = false;

	int cont_frames = 0;
	int cont_status = 0;

	bool fim = false;

	float x = 0, y = 0;
	float pos_ant_per_x, pos_ant_per_y;

	Mini_jogo resul_mini_jogo;
	if (inicializar()) {
		p = inicializar_struct_personagem();
		p.dinheiro = 1000;
		al = inicializar_struct_alimento();
		c = inicializar_struct_cenario();
		d = inicializar_struct_data();
		//verificar o status atual do personagem
		trocar_status_personagem(&p, d, c);
		//para exibir o primeiro cenário
		if (c.cenario_atual > LOJA_MENU) {
			c.cenario_atual = c.ultimo_cenario;
		}
		trocar_background_principal(c.cenario_atual);

		if (p.acessorio.desenhar_acessorio == true) {
			carregar_acessorio(p, p.acessorio.id);
		}
		while (!fim) {
			al_wait_for_event(fila_eventos, &evento);//fila de eventos

			if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {//fechar a tela no botão
				fim = true;
			}

			/*----------------EVENTOS A PARTIR DO MOVIMENTO DO MOUSE----------------*/
			if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
				//cout << "x:" << evento.mouse.x << "y:" << evento.mouse.y << endl;
				x = evento.mouse.x; y = evento.mouse.y;
				limite_clic_x = evento.mouse.x; limite_clic_y = evento.mouse.y;
				
			}
			//se a comida estiver na boca do personagem, ele abrirá a boca
			if (p.tamanho == 1 && limite_clic_x >= 460 && limite_clic_x <= 563 && limite_clic_y >= 520 && limite_clic_y <= 564 && interagir_cozinha.movimentar_alimento == true && interagir_cozinha.comer == false) {
				cont_frames = 4;
				p.status = COMENDO;
			}
			else if (p.tamanho == 2 && limite_clic_x >= 460 && limite_clic_x <= 563 && limite_clic_y >= 520 && limite_clic_y <= 564 && interagir_cozinha.movimentar_alimento == true && interagir_cozinha.comer == false) {
				cont_frames = 4;
				p.status = COMENDO;
			}
			else if (p.tamanho == 3 && limite_clic_x >= 460 && limite_clic_x <= 563 && limite_clic_y >= 520 && limite_clic_y <= 564 && interagir_cozinha.movimentar_alimento == true && interagir_cozinha.comer == false) {
				cont_frames = 4;
				p.status = COMENDO;
			}
			else if (p.tamanho == 4 && limite_clic_x >= 416 && limite_clic_x <= 590 && limite_clic_y >= 478 && limite_clic_y <= 520  && interagir_cozinha.movimentar_alimento == true && interagir_cozinha.comer == false) {
				cont_frames = 4;
				p.status = COMENDO;
			}
			//se o sabonete estiver em cima do personagem
			else if (sqrt(pow(abs(x1 - evento.mouse.x), 2) + pow(abs(y1 - evento.mouse.y), 2)) <= 180 && interagir_banheiro.movimentar_sabonete == true) {
				cont_frames = 201;
				p.status = PARADO;
				//if (interagir_banheiro.cont_frames > 400) {
				interagir_banheiro.cont_frames++;
				interagir_banheiro.ensaboar = true;
				//}

			}
			//fim dos eventos a partir do movimento do mouse

			//al_draw_bitmap(mouse,evento.mouse.x,evento.mouse.y,0);
			/*---------------EVENTOS A PARTIR DO TECLADO---------------*/
			if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
				if (c.cenario_atual == SALA || c.cenario_atual == COZINHA || c.cenario_atual == BANHEIRO || c.cenario_atual == QUARTO_ACESO || c.cenario_atual == SALA_DE_JOGOS) {
					switch (evento.keyboard.keycode) {
					case ALLEGRO_KEY_RIGHT:
						c.cenario_atual++;
						if (c.cenario_atual == QUARTO_APAGADO) {
							c.cenario_atual = SALA_DE_JOGOS;
						}
						else if (c.cenario_atual > SALA_DE_JOGOS) {
							c.cenario_atual = SALA;
						}
						break;
					case ALLEGRO_KEY_LEFT:
						c.cenario_atual--;
						if (c.cenario_atual == QUARTO_APAGADO) {
							c.cenario_atual = QUARTO_ACESO;
						}
						else if (c.cenario_atual < SALA) {
							c.cenario_atual = SALA_DE_JOGOS;
						}
						break;
					}
					cont_frames = 0;
					trocar_background_principal(c.cenario_atual);
				}
				else if (c.cenario_atual == LOJA_COMIDA_1 || c.cenario_atual == LOJA_COMIDA_2) {
					switch (evento.keyboard.keycode) {
					case ALLEGRO_KEY_RIGHT:
						c.cenario_atual++;
						if (c.cenario_atual > LOJA_COMIDA_2) {
							c.cenario_atual = LOJA_COMIDA_1;
						}
						break;
					case ALLEGRO_KEY_LEFT:
						c.cenario_atual--;
						if (c.cenario_atual < LOJA_COMIDA_1) {
							c.cenario_atual = LOJA_COMIDA_2;
						}
						break;
					}
					cont_frames = 0;
					trocar_background_principal(c.cenario_atual);
				}
				else if (c.cenario_atual >= LOJA_PERSONAGEM_1 && c.cenario_atual <= LOJA_PERSONAGEM_3) {
					switch (evento.keyboard.keycode) {
					case ALLEGRO_KEY_RIGHT:
						c.cenario_atual++;
						if (c.cenario_atual > LOJA_PERSONAGEM_3) {
							c.cenario_atual = LOJA_PERSONAGEM_1;
						}
						break;
					case ALLEGRO_KEY_LEFT:
						c.cenario_atual--;
						if (c.cenario_atual < LOJA_PERSONAGEM_1) {
							c.cenario_atual = LOJA_PERSONAGEM_3;
						}
						break;
					}
					cont_frames = 0;
					trocar_background_principal(c.cenario_atual);
				}
				else if (c.cenario_atual >= GUARDA_ROUPA_1 && c.cenario_atual <= GUARDA_ROUPA_2) {
					switch (evento.keyboard.keycode) {
					case ALLEGRO_KEY_RIGHT:
						c.cenario_atual++;
						if (c.cenario_atual > GUARDA_ROUPA_2) {
							c.cenario_atual = GUARDA_ROUPA_1;
						}
						break;
					case ALLEGRO_KEY_LEFT:
						c.cenario_atual--;
						if (c.cenario_atual < GUARDA_ROUPA_1) {
							c.cenario_atual = GUARDA_ROUPA_2;
						}
						break;
					}
					cont_frames = 0;
					trocar_background_principal(c.cenario_atual);
				}
			}
			/*-----------------EVENTOS COM O CLICK DO MOUSE DOWN-------------------*/
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

				/*------------------------abrir tela de status----------------------*/
				if (evento.mouse.button & 1 && calcular_raio(evento.mouse.x, evento.mouse.y, 978, 40) <= 30 && (c.cenario_atual < LOJA_MENU)) {
					c.ultimo_cenario = c.cenario_atual;
					c.cenario_atual = TELA_STATUS;
					trocar_background_principal(c.cenario_atual);
					//442 252
					exibirTexto = true;
					//p.ultimo_status = p.status;
					//p.status = SEM_INTERAGIR;
					cont_frames = -1;
					d = load_file_data(save_data);
					trocar_status_personagem(&p, d, c);
				}
				else if (evento.mouse.button & 1 && (c.cenario_atual == TELA_STATUS) && evento.mouse.x >= 425 && evento.mouse.x <= 597
					&& evento.mouse.y >= 646 && evento.mouse.y <= 698) {
					c.cenario_atual = c.ultimo_cenario;
					trocar_background_principal(c.cenario_atual);
					exibirTexto = false;
					p.status = p.ultimo_status;
					cont_frames = -1;
				}

				/*-------------------------interagir com o personagem na sala--------------------*/
				else if (evento.mouse.button & 1 && calcular_raio(evento.mouse.x, evento.mouse.y, x1, y1) <= 160 && (c.cenario_atual == SALA || c.cenario_atual == SALA_DE_JOGOS)) {
					p.interagir = true;
					cont_frames = 0;
				}

				/*-------------pegar o alimento com o mouse-----------------------------*/
				else if (evento.mouse.button & 1 && c.cenario_atual == COZINHA && evento.mouse.x >= 440 && evento.mouse.x <= 600
					&& evento.mouse.y >= 592 && evento.mouse.y <= 620) {
					p.interagir = false;//???
					interagir_cozinha.movimentar_alimento = true;
				}

				/*-----------------------ligar chuveiro-----------------------*/
				else if (evento.mouse.button & 1 && c.cenario_atual == BANHEIRO && evento.mouse.x >= 495 && evento.mouse.x <= 518
					&& evento.mouse.y >= 158 && evento.mouse.y <= 190) {
					interagir_banheiro.ligar_chuveiro = !interagir_banheiro.ligar_chuveiro;
					interagir_banheiro.cont_frames++;
					if (interagir_banheiro.ligar_chuveiro == true) {
						som_chuveiro = al_load_audio_stream("efeitos_sonoros\\principal\\chuveiro.ogg", 4, 1024);
						al_attach_audio_stream_to_mixer(som_chuveiro, al_get_default_mixer());
						al_set_audio_stream_playmode(som_chuveiro, ALLEGRO_PLAYMODE_LOOP);
					}
					else {
						if (som_chuveiro != NULL) {
							al_drain_audio_stream(som_chuveiro);
							al_destroy_audio_stream(som_chuveiro);
						}
					}
				}

				/*-----------------------pegar sabonete com o mouse--------------------*/
				else if (evento.mouse.button & 1 && c.cenario_atual == BANHEIRO && evento.mouse.x >= 713 && evento.mouse.x <= 772
					&& evento.mouse.y >= 200 && evento.mouse.y <= 230) {
					carregar_bolhas(p);
					interagir_banheiro.movimentar_sabonete = true;
				}

				/*------------------------interagir com o interruptor do quarto-------------------*/
				else if (evento.mouse.button & 1 && (c.cenario_atual == QUARTO_ACESO || c.cenario_atual == QUARTO_APAGADO) && evento.mouse.x >= 110 && evento.mouse.x <= 122
					&& evento.mouse.y >= 272 && evento.mouse.y <= 292) {
					quarto_interruptor = !quarto_interruptor;
					if (quarto_interruptor == true && c.cenario_atual == QUARTO_ACESO) {
						cont_frames = 0;
						c.cenario_atual = QUARTO_APAGADO;
						p.ultimo_status = p.status;
						p.status = DORMINDO;
					}
					else {
						cont_frames = 0;
						c.cenario_atual = QUARTO_ACESO;
						trocar_status_personagem(&p, d, c);
					}
					al_play_sample(efeitos[0], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					trocar_background_principal(c.cenario_atual);
				}

				/*-------------------------------------------ABRIR LOJA---------------------------------------------*/
				else if (evento.mouse.button & 1 && c.cenario_atual != GELADEIRA && evento.mouse.x >= 12 && evento.mouse.x <= 94
					&& evento.mouse.y >= 671 && evento.mouse.y <= 757 && loja_aberta == false) {
					if (carregar_loja()) {
						loja_aberta = true;
						c.ultimo_cenario = c.cenario_atual;
						p.ultimo_status = p.status;
						p.status = SEM_INTERAGIR;
						c.cenario_atual = LOJA_MENU;
						trocar_background_principal(c.cenario_atual);
					}
					else {//CORRIGIR
						fim = true;
					}
				}

				/*---------------------------------fechar loja / voltar para o menu loja----------------------------------*/
				else if (evento.mouse.button & 1 && (c.cenario_atual >= LOJA_MENU && c.cenario_atual <= LOJA_PERSONAGEM_2) && evento.mouse.x >= 425 && evento.mouse.x <= 597
					&& evento.mouse.y >= 646 && evento.mouse.y <= 698) {
					if (c.cenario_atual == LOJA_MENU) {
						loja_aberta = false;
						finalizar_loja();
						p.status = p.ultimo_status;
						cont_frames = -1;
						c.cenario_atual = c.ultimo_cenario;
						load_file_alimento_vet(save_alimento, vet_al);
						if (vet_al[2].quantidade > 0) {
							interagir_cozinha.comer = false;
						}
					}
					else {
						c.cenario_atual = LOJA_MENU;
					}
					trocar_background_principal(c.cenario_atual);
				}

				/*---------------------------------abrir_loja_comidas----------------------------------------*/
				else if (evento.mouse.button & 1 && c.cenario_atual == LOJA_MENU && evento.mouse.x >= 45 && evento.mouse.x <= 228
					&& evento.mouse.y >= 290 && evento.mouse.y <= 330) {
					c.cenario_atual = LOJA_COMIDA_1;
					trocar_background_principal(c.cenario_atual);
				}
				else if (evento.mouse.button & 1 && c.cenario_atual == LOJA_COMIDA_1 || c.cenario_atual == LOJA_COMIDA_2) {
					comprar_comida(&p, c, evento.mouse.x, evento.mouse.y);
					
				}
				///*sa,dlsaldslamfmspfmmfas validar abertura de sessão
				/*-------------------------------abrir_loja_personagem-----------------------------------------*/
			   else if (evento.mouse.button & 1 && c.cenario_atual == LOJA_MENU && evento.mouse.x >= 627 && evento.mouse.x <= 986
					&& evento.mouse.y >= 290 && evento.mouse.y <= 330) {
				   c.cenario_atual = LOJA_PERSONAGEM_1;
					trocar_background_principal(c.cenario_atual);
				}
			   else if (evento.mouse.button & 1 && c.cenario_atual == LOJA_PERSONAGEM_1 || c.cenario_atual == LOJA_PERSONAGEM_2 || c.cenario_atual == LOJA_PERSONAGEM_3) {
				   comprar_aparencia(&p, c, vet_ap, evento.mouse.x, evento.mouse.y);
				   load_file_aparencia(save_aparencia, vet_ap);

			   }
			   if (evento.mouse.button & 1 && (c.cenario_atual >= LOJA_PERSONAGEM_1 && c.cenario_atual <= LOJA_PERSONAGEM_3) && evento.mouse.x >= 418 && evento.mouse.x <= 600
				   && evento.mouse.y >= 707 && evento.mouse.y <= 738) {
				   c.cenario_atual = LOJA_MENU;
				   trocar_background_principal(c.cenario_atual);

			   }

			   /*-----------------------------------------ABRIR_GUARDA_ROUPA------------------------------------------*/
			   else if (evento.mouse.button & 1 && (c.cenario_atual == QUARTO_ACESO || c.cenario_atual == QUARTO_APAGADO) && evento.mouse.x >= 931 && evento.mouse.x <= 1016
				   && evento.mouse.y >= 671 && evento.mouse.y <= 757) {
				   if (carregar_guarda_roupa()) {
					   c.ultimo_cenario = c.cenario_atual;
					 //  p.ultimo_status = p.status;
					   trocar_status_personagem(&p, d, c);
					 //  p.status = SEM_INTERAGIR;
					   pos_ant_per_x = p.pos_sprite_x;pos_ant_per_y = p.pos_sprite_y;
					   p.pos_sprite_x = 650; p.pos_sprite_y = 420;
					   c.cenario_atual = GUARDA_ROUPA_1;
					   trocar_background_principal(c.cenario_atual);
				   }

			   }
			   else if (evento.mouse.button & 1 && c.cenario_atual == GUARDA_ROUPA_1) {
				   selecionar_cor_personagem(p, c, vet_ap, evento.mouse.x, evento.mouse.y);
				   p = inicializar_struct_personagem();
			   }
			   else if (evento.mouse.button & 1 && c.cenario_atual == GUARDA_ROUPA_2) {
				   selecionar_acessorio_personagem(p, c, vet_ap, &p.acessorio.desenhar_acessorio, &p.acessorio.id,evento.mouse.x, evento.mouse.y);
			   }
			   if (evento.mouse.button & 1 && (c.cenario_atual == GUARDA_ROUPA_1 || c.cenario_atual == GUARDA_ROUPA_2) && evento.mouse.x >= 418 && evento.mouse.x <= 600
				   && evento.mouse.y >= 707 && evento.mouse.y <= 738) {
				   finalizar_guarda_roupa();
				   c.cenario_atual = c.ultimo_cenario;
				   trocar_background_principal(c.cenario_atual);
				   p.pos_sprite_x = pos_ant_per_x; p.pos_sprite_y = pos_ant_per_y;
				   trocar_status_personagem(&p, d, c);
			   }
				
				/*-------------------------------abrir geladeira-------------------------------------*/
				else if (evento.mouse.button & 1 && c.cenario_atual == COZINHA && evento.mouse.x >= 693 && evento.mouse.x <= 898
					&& evento.mouse.y >= 250 && evento.mouse.y <= 571) {
					if (!carregar_geladeira()) {
						fim = true;
					}
					else {
						interagir_cozinha.abrir_geladeira = true;
						c.cenario_atual = GELADEIRA;
						trocar_background_principal(c.cenario_atual);
						p.status = SEM_INTERAGIR;
					}
				}
			}
			/*----------------------------------fechar geladeira--------------------------------------*/
			else if (evento.mouse.button & 1 && c.cenario_atual == GELADEIRA && evento.mouse.x >= 425 && evento.mouse.x <= 597
				&& evento.mouse.y >= 646 && evento.mouse.y <= 698) {
				finalizar_geladeira();
				interagir_cozinha.abrir_geladeira = false;
				c.cenario_atual = COZINHA;
				trocar_background_principal(c.cenario_atual);
				p.status = PARADO;
				cont_frames = -1;
			}//425  597  646 698

			/*-----------------------abrir menu jogos--------------------------*/
			else if (evento.mouse.button & 1 && c.cenario_atual == SALA_DE_JOGOS && evento.mouse.x >= 908 && evento.mouse.x <= 1013
				&& evento.mouse.y >= 657 && evento.mouse.y <= 757) {
				c.ultimo_cenario = c.cenario_atual;
				c.cenario_atual = MINI_JOGOS;
				trocar_background_principal(c.cenario_atual);
				p.ultimo_status = p.status;
				p.status = SEM_INTERAGIR;
				cont_frames = -1;
				
			}
			else if (evento.mouse.button & 1 && (c.cenario_atual == MINI_JOGOS) && evento.mouse.x >= 425 && evento.mouse.x <= 597
				&& evento.mouse.y >= 646 && evento.mouse.y <= 698) {
				c.cenario_atual = c.ultimo_cenario;
				trocar_background_principal(c.cenario_atual);
				p.status = p.ultimo_status;
				cont_frames = -1;
			}
			else if (evento.mouse.button & 1 && c.cenario_atual == MINI_JOGOS) {
				resul_mini_jogo = selecionar_mini_jogo(evento.mouse.x, evento.mouse.y, &evento);
				p.dinheiro += resul_mini_jogo.dinheiro_ganho;
				p.progresso.progresso_nivel += resul_mini_jogo.progresso;
				if (p.progresso.progresso_nivel >= 100) {
					cout << p.nivel << endl;
					p.nivel++;
					cout << p.nivel << endl;
					crescer_personagem(p);
					p = inicializar_struct_personagem();
				}
			}

			/*------------------------SELICIONAR COMIDA----------------------------*/
			else if (evento.mouse.button & 1 && c.cenario_atual == COZINHA && evento.mouse.x >= 324 && evento.mouse.x <= 369
				&& evento.mouse.y >= 663 && evento.mouse.y <= 710) {
				id_comida--;
				if (id_comida < 1) {
					id_comida = 1;
				}
				id_comida = buscar_alimento(vet_al, id_comida);
				cout << id_comida << endl;
			}
			else if (evento.mouse.button & 1 && c.cenario_atual == COZINHA && evento.mouse.x >= 664 && evento.mouse.x <= 713
				&& evento.mouse.y >= 663 && evento.mouse.y <= 710) {
				id_comida++;
				if (id_comida > 29) {
					id_comida = 29;
				}
				id_comida = buscar_alimento(vet_al, id_comida);
			}
			
			 /*-----------------EVENTOS COM O CLICK DO MOUSE UP-------------------*/
			if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				//se o usuario soltar o alimento na boca do personagem
				if (evento.mouse.button & 1 && c.cenario_atual == COZINHA && interagir_cozinha.movimentar_alimento == true && interagir_cozinha.comer == false && evento.mouse.x >= 440 && evento.mouse.x <= 616 && evento.mouse.y >= 520 && evento.mouse.y <= 555) {
					p.status = COMENDO;
					//interagir_cozinha.comer = true;
					comer(&p, &interagir_cozinha, vet_al, id_comida);
					load_file_alimento_vet(save_alimento, vet_al);
					if (vet_al[id_comida].quantidade > 0) {
						interagir_cozinha.comer = false;
					}
					interagir_cozinha.mastigar = true;
					cont_frames = 0;
					al_play_sample(efeitos[1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				
				}
				//se o usuario soltar o alimento
				if (interagir_cozinha.movimentar_alimento == true) {
					interagir_cozinha.movimentar_alimento = false;
					trocar_status_personagem(&p, d, c);
				}
				//se o usuario soltar o sabonete
				if (interagir_banheiro.movimentar_sabonete == true) {
					interagir_banheiro.movimentar_sabonete = false;
					
				}
			}

			/*------------------mastigando o alimento-------------------*/
			if (interagir_cozinha.mastigar == true) {
				interagir_cozinha.mastigando++;
				if (interagir_cozinha.mastigando < 15) {
					p.status = COMENDO;
					if (interagir_cozinha.mastigando == 1) {
						cont_frames = 0;
					}
				}
				else {
					interagir_cozinha.mastigar = false;
					interagir_cozinha.mastigando = 0;
				}
			}

			/*-------------------EXECUÇÃO DOS EVENTOS---------------------*/
			if (evento.type == ALLEGRO_EVENT_TIMER) {
				
				/*con++;
				if (con == 200) {
					p.nivel = 4;
					crescer_personagem(p);
					p = inicializar_struct_personagem();
				}*/
				if (cont_status > 3000) {
					d = load_file_data(save_data);
					trocar_status_personagem(&p, d, c);
					cont_status = 0;
				}
				cont_frames++;
				cont_status++;
				exibir_animacao(&cont_frames, c.cenario_atual, background, objetos_cenario, personagem, &p, &interagir_cozinha, &interagir_banheiro, sprite_alimento, &a, x, y, font_status);
				//
				if (interagir_banheiro.ligar_chuveiro == true && c.cenario_atual == BANHEIRO) {
					chuveiro();
					interagir_banheiro.ensaboar = false;
					interagir_banheiro.cont_frames = 0;
					if (som_chuveiro == NULL) {
						som_chuveiro = al_load_audio_stream("efeitos_sonoros\\principal\\chuveiro.ogg", 4, 1024);
						al_attach_audio_stream_to_mixer(som_chuveiro, al_get_default_mixer());
						al_set_audio_stream_playmode(som_chuveiro, ALLEGRO_PLAYMODE_LOOP);
					}
					if (p.progresso.porcentagem_limpeza < 50) {
						p.progresso.porcentagem_limpeza = 100;
						p.progresso.porcentagem_saude += 10;
						finalizar_sujeira();
					}
				}
				if (c.cenario_atual == COZINHA) {
					desenhar_alimento_mesa(sprite_alimento, save_alimento, &interagir_cozinha, &a, x, y, id_comida, vet_al, font_status);
				}
			    if (c.cenario_atual != BANHEIRO && interagir_banheiro.ligar_chuveiro == true) {
					if (som_chuveiro != NULL) {
						al_drain_audio_stream(som_chuveiro);
						al_destroy_audio_stream(som_chuveiro);
						som_chuveiro = NULL;
					}
					exibir_aviso(tela_aviso, AGUA);
				}
				if (p.progresso.porcentagem_limpeza < 50) {
					
					if (c.cenario_atual < LOJA_MENU || c.cenario_atual >= GUARDA_ROUPA_1) {
						if (carregar_sujeira(p)) {
							desenhar_sujeira(p, sujeira_personagem);
						}
					}
				}
				//desenhar_mouse(mouse_of, mouse_on, status_mouse, x, y);
				else if (interagir_cozinha.abrir_geladeira == true) {
					load_file_alimento_vet(save_alimento, vet_al);
				}
			  
				 if (interagir_banheiro.ensaboar == true) {
					ensaboar(p, bolhas_banheiro, c,interagir_banheiro.cont_frames++);
				}
			}
			if (p.acessorio.desenhar_acessorio == true && ( c.cenario_atual < LOJA_MENU || c.cenario_atual >= GUARDA_ROUPA_1)) {
				desenhar_acessorio_persnoagem(acessorio, p, c, p.acessorio.id);
			}
			al_flip_display();

		}
		save_file_personagem(p, save_personagem);
		save_file_cenario(c, save_cenario);
		d = data_atual();
		save_file_data(d, save_data);
		finalizar();
	}
	return 0;
}

Personagem inicializar_struct_personagem() {
	Personagem p;

	p = load_file_personagem(save_personagem);

	if (p.file_flag == true) {// file_flag indica se é a primeira vez que o jogo está sendo inciado ou se o jogo foi resetado
		al_draw_bitmap(tela_inicio, 0, 0, 0);
		al_flip_display();
		al_rest(5);
		p.file_flag = false;
		p.interagir = false;
		p.acessorio.desenhar_acessorio = false;
		p.acessorio.id = 0;
		p.nivel = 1;
		p.cor = BRANCO;
		p.tamanho =  1;
		p.dinheiro = 50;
		p.status = PARADO;
		p.ultimo_status = p.status;
		p.altura_sprite = 200;
		p.largura_sprite = 200;
		p.regiao_x_folha = 0;
		p.regiao_y_folha = 0;
		p.pos_sprite_x = 412;
		p.pos_sprite_y = 420;
		p.progresso.porcentagem_energia = 100;
		p.progresso.porcentagem_fome = 100;
		p.progresso.porcentagem_saude = 100;
		p.progresso.porcentagem_limpeza = 100;
		p.progresso.porcentgem_peso = 0;
		p.progresso.progresso_nivel = 0;
		save_file_personagem(p, save_personagem);
	}
	return p;
}

Cenario inicializar_struct_cenario() {
	Cenario c;

	c = load_file_cenario(save_cenario);
	if (c.file_flag == true) {
		c.file_flag = false;
		c.cenario_atual = SALA;
		c.ultimo_cenario = SALA;
		save_file_cenario(c, save_cenario);
	}
	return c;
}

Alimento inicializar_struct_alimento() {
	Alimento a;
	a = load_file_alimento(save_alimento);
	if (a.file_flag == true) {
			a.id_alimento = 1;
			a.quantidade = 1;
			a.preco = 0;
			a.file_flag = false;
			save_file_alimento(a, save_alimento);
	}
	return a;
}

TData inicializar_struct_data() {
	TData d;

	d = load_file_data(save_data);
	if (d.file_flag == true) {
		d = data_atual();
		d.file_flag = false;
		save_file_data(d, save_data);
	}
	return d;
}

bool inicializar() {
	//INICIALIZAR O ALLEGRO
	if (!al_init()) {
		error_msg("Falha ao inicializar a Allegro");
		return false;
	}
	//INICIALIZAR A JANELA
	/*ALLEGRO_MONITOR_INFO info;
	int res_x_comp, res_y_comp;

	al_get_monitor_info(0, &info);

	res_x_comp = info.x2 - info.x1;
	res_y_comp = info.y2 - info.y1;


	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	janela = al_create_display(res_x_comp, res_y_comp);*/
	janela = al_create_display(Largura_t, Altura_t);
	if (!janela) {
		error_msg("Falha ao criar janela");
		return false;
	}

	/*float red_x = res_x_comp /(float) LARGURA_TELA;
	float red_y = res_y_comp / (float)ALTURA_TELA;
	ALLEGRO_TRANSFORM transformar;
	al_identity_transform(&transformar);
	al_scale_transform(&transformar, red_x, red_y);
	al_use_transform(&transformar);*/


	//INICIALIZAÇÃO DOS ADDONS
	if (!al_init_image_addon()) {
		error_msg("Falha ao inicializar o addon de imagens");
		al_destroy_display(janela);
		return false;
	}
	if (!al_init_primitives_addon()) {
		error_msg("Falha ao inicializar primitives addon");
		al_destroy_display(janela);
		return false;
	}
	if (!al_init_font_addon()) {
		error_msg("Falha ao inicializar addon font");
		al_destroy_display(janela);
		return false;
	}
	if (!al_init_ttf_addon()) {
		error_msg("Falha ao inicializar addon ttf");
		al_destroy_display(janela);
		return false;
	}
	if (!al_install_audio()) {
		error_msg("Falha ao inicializar o audio");
		return 0;
	}
	if (!al_init_acodec_addon()) {
		error_msg("Falha ao inicializar o codec de audio");
		return 0;
	}

	if (!al_reserve_samples(5)) {
		error_msg("Falha ao reservar amostrar de audio");
		return 0;
	}


	//INICIALIZAÇAO DOS DE COMPONENTES
	if (!al_install_keyboard()) {
		error_msg("Falha ao inicializar o teclado");
		al_destroy_display(janela);
		return false;
	}
	if (!al_install_mouse()) {
		error_msg("Falha ao inicializar o mouse");
		al_destroy_display(janela);
		return false;
	}
	//INICIALIZAR FONTE
	font_status = al_load_font("arial.ttf", 50, NULL);

	//INICIALIZAR TIMER
	temporizador = al_create_timer(1.0 / FPS);
	if (!temporizador) {
		error_msg("Falha ao criar temporizador");
		al_destroy_display(janela);
		return false;
	}

	//INICIALIZAR FILA DE EVENTOS
	fila_eventos = al_create_event_queue();
	if (!fila_eventos) {
		error_msg("Falha ao criar fila de eventos");
		al_destroy_timer(temporizador);
		al_destroy_display(janela);
		return false;
	}

	//CARREGAR SOM
	efeitos[0] = al_load_sample("efeitos_sonoros\\principal\\interruptor.ogg");
	if (!efeitos[0]) {
		error_msg("Falha ao carregar som");
		al_destroy_timer(temporizador);
		al_destroy_display(janela);
		al_destroy_event_queue(fila_eventos);
		return false;
	}
	efeitos[1] = al_load_sample("efeitos_sonoros\\principal\\mastigando.ogg");
	if (!efeitos[1]) {
		error_msg("Falha ao carregar som");
		al_destroy_timer(temporizador);
		al_destroy_display(janela);
		al_destroy_event_queue(fila_eventos);
		al_destroy_sample(efeitos[0]);
		return false;
	}
	efeitos[2] = al_load_sample("efeitos_sonoros\\principal\\dinheiro.ogg");
	if (!efeitos[2]) {
		error_msg("Falha ao carregar som");
		al_destroy_timer(temporizador);
		al_destroy_display(janela);
		al_destroy_event_queue(fila_eventos);
		al_destroy_sample(efeitos[0]);
		al_destroy_sample(efeitos[1]);
		return false;
	}

	//CARREGAR IMAGENS
	if (!carregar_imagens()) {
		al_destroy_timer(temporizador);
		al_destroy_display(janela);
		al_destroy_event_queue(fila_eventos);
		al_destroy_sample(efeitos[0]);
		al_destroy_sample(efeitos[1]);
		al_destroy_sample(efeitos[2]);
		return false;
	}
	//REGISTRAR EVENTOS
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());//para poder utilizar o teclado
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));//para poder fechar a janela
	al_register_event_source(fila_eventos, al_get_mouse_event_source());//para poder utilizar o mouse
	al_register_event_source(fila_eventos, al_get_timer_event_source(temporizador));

	//INICIALIZAR TEMPORIZADOR
	al_start_timer(temporizador);
	return true;

}

bool carregar_imagens() {

	tela_inicio = al_load_bitmap("backgrounds\\tela_inicio.png");
	if (!tela_inicio) {
		error_msg("Falha ao carregar background");
		return false;
	}

	/*------------------CARREGAR BACKGROUNDS-------------------------*/
	background_vetor[SALA] = al_load_bitmap("background_sala.png");
	if (!background_vetor[SALA]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(tela_inicio);
		return false;
	}
	background_vetor[COZINHA] = al_load_bitmap("background_cozinha.png");
	if (!background_vetor[COZINHA]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(tela_inicio);
		al_destroy_bitmap(background_vetor[SALA]);
		return false;
	}
	background_vetor[BANHEIRO] = al_load_bitmap("background_banheiro.png");
	if (!background_vetor[BANHEIRO]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(tela_inicio);
		al_destroy_bitmap(background_vetor[SALA]);
		al_destroy_bitmap(background_vetor[COZINHA]);
		return false;
	}
	background_vetor[QUARTO_ACESO] = al_load_bitmap("background_quarto_aceso.png");
	if (!background_vetor[QUARTO_ACESO]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(tela_inicio);
		al_destroy_bitmap(background_vetor[SALA]);
		al_destroy_bitmap(background_vetor[COZINHA]);
		al_destroy_bitmap(background_vetor[BANHEIRO]);
		return false;
	}
	background_vetor[QUARTO_APAGADO] = al_load_bitmap("background_quarto_apagado.png");
	if (!background_vetor[QUARTO_APAGADO]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(tela_inicio);
		al_destroy_bitmap(background_vetor[SALA]);
		al_destroy_bitmap(background_vetor[COZINHA]);
		al_destroy_bitmap(background_vetor[BANHEIRO]);
		al_destroy_bitmap(background_vetor[QUARTO_ACESO]);
		return false;
	}
	background_vetor[SALA_DE_JOGOS] = al_load_bitmap("background_sala_de_jogos.png");
	if (!background_vetor[SALA_DE_JOGOS]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(tela_inicio);
		al_destroy_bitmap(background_vetor[SALA]);
		al_destroy_bitmap(background_vetor[COZINHA]);
		al_destroy_bitmap(background_vetor[BANHEIRO]);
		al_destroy_bitmap(background_vetor[QUARTO_ACESO]);
		al_destroy_bitmap(background_vetor[QUARTO_APAGADO]);
		return false;
	}
	background_vetor[TELA_STATUS] = al_load_bitmap("background_status.png");
	if (!background_vetor[TELA_STATUS]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(tela_inicio);
		al_destroy_bitmap(background_vetor[SALA]);
		al_destroy_bitmap(background_vetor[COZINHA]);
		al_destroy_bitmap(background_vetor[BANHEIRO]);
		al_destroy_bitmap(background_vetor[QUARTO_ACESO]);
		al_destroy_bitmap(background_vetor[QUARTO_APAGADO]);
		al_destroy_bitmap(background_vetor[SALA_DE_JOGOS]);
		return false;
	}
	background_vetor[MINI_JOGOS] = al_load_bitmap("background_mini_jogos.png");
	if (!background_vetor[MINI_JOGOS]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(tela_inicio);
		al_destroy_bitmap(background_vetor[SALA]);
		al_destroy_bitmap(background_vetor[COZINHA]);
		al_destroy_bitmap(background_vetor[BANHEIRO]);
		al_destroy_bitmap(background_vetor[QUARTO_ACESO]);
		al_destroy_bitmap(background_vetor[QUARTO_APAGADO]);
		al_destroy_bitmap(background_vetor[SALA_DE_JOGOS]);
		al_destroy_bitmap(background_vetor[TELA_STATUS]);
		return false;
	}

	/*---------------CARREGAR TELAS DE AVISOS---------------*/
	tela_aviso[AGUA] = al_load_bitmap("tela_avisos\\tela_aviso_agua.png");
	if (!tela_aviso[AGUA]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(tela_inicio);
		al_destroy_bitmap(background_vetor[SALA]);
		al_destroy_bitmap(background_vetor[COZINHA]);
		al_destroy_bitmap(background_vetor[BANHEIRO]);
		al_destroy_bitmap(background_vetor[QUARTO_ACESO]);
		al_destroy_bitmap(background_vetor[QUARTO_APAGADO]);
		al_destroy_bitmap(background_vetor[SALA_DE_JOGOS]);
		al_destroy_bitmap(background_vetor[TELA_STATUS]);
		al_destroy_bitmap(background_vetor[MINI_JOGOS]);
		return false;
	}

	/*---------------CARREGAR IMAGENS SPRITES-------------------------*/

	if (!carregar_sprite_personagem()) {
		al_destroy_bitmap(tela_inicio);
		al_destroy_bitmap(background_vetor[SALA]);
		al_destroy_bitmap(background_vetor[COZINHA]);
		al_destroy_bitmap(background_vetor[BANHEIRO]);
		al_destroy_bitmap(background_vetor[QUARTO_ACESO]);
		al_destroy_bitmap(background_vetor[QUARTO_APAGADO]);
		al_destroy_bitmap(background_vetor[SALA_DE_JOGOS]);
		al_destroy_bitmap(background_vetor[TELA_STATUS]);
		al_destroy_bitmap(background_vetor[MINI_JOGOS]);
		al_destroy_bitmap(tela_aviso[AGUA]);
		return false;
	}


	/*-------------------CARREGAR OBJETOS----------------------------*/
	//mesa = al_load_bitmap("cozinha_mesa.png");
	objetos_cenario[MESA] = al_load_bitmap("cozinha_mesa.png");
	if (!objetos_cenario[MESA]) {
		error_msg("Falha ao carregar imagem objeto");
		al_destroy_bitmap(background_vetor[SALA]);
		al_destroy_bitmap(background_vetor[COZINHA]);
		al_destroy_bitmap(background_vetor[BANHEIRO]);
		al_destroy_bitmap(background_vetor[QUARTO_ACESO]);
		al_destroy_bitmap(background_vetor[QUARTO_APAGADO]);
		al_destroy_bitmap(background_vetor[SALA_DE_JOGOS]);
		al_destroy_bitmap(background_vetor[TELA_STATUS]);
		al_destroy_bitmap(background_vetor[MINI_JOGOS]);
		al_destroy_bitmap(tela_aviso[AGUA]);
		finalizar_sprite_personagem();
		return false;
	}

	objetos_cenario[SABONETE] = al_load_bitmap("sabonete.png");
	if (!objetos_cenario[SABONETE]) {
		error_msg("Falha ao carregar imagem objeto");
		al_destroy_bitmap(background_vetor[SALA]);
		al_destroy_bitmap(background_vetor[COZINHA]);
		al_destroy_bitmap(background_vetor[BANHEIRO]);
		al_destroy_bitmap(background_vetor[QUARTO_ACESO]);
		al_destroy_bitmap(background_vetor[QUARTO_APAGADO]);
		al_destroy_bitmap(background_vetor[SALA_DE_JOGOS]);
		al_destroy_bitmap(background_vetor[TELA_STATUS]);
		al_destroy_bitmap(background_vetor[MINI_JOGOS]);
		al_destroy_bitmap(tela_aviso[AGUA]);
		finalizar_sprite_personagem();
		al_destroy_bitmap(objetos_cenario[MESA]);
		return false;
	}

	/*-------------------CARREGAR SPRITE ALIMENTOS------------------*/
	sprite_alimento = al_load_bitmap("alimentos4.png");
	if (!sprite_alimento) {
		error_msg("Falha ao carregar sprites");
		al_destroy_bitmap(background_vetor[SALA]);
		al_destroy_bitmap(background_vetor[COZINHA]);
		al_destroy_bitmap(background_vetor[BANHEIRO]);
		al_destroy_bitmap(background_vetor[QUARTO_ACESO]);
		al_destroy_bitmap(background_vetor[QUARTO_APAGADO]);
		al_destroy_bitmap(background_vetor[SALA_DE_JOGOS]);
		al_destroy_bitmap(background_vetor[TELA_STATUS]);
		al_destroy_bitmap(background_vetor[MINI_JOGOS]);
		al_destroy_bitmap(tela_aviso[AGUA]);
		finalizar_sprite_personagem();
		al_destroy_bitmap(objetos_cenario[MESA]);
		al_destroy_bitmap(objetos_cenario[SABONETE]);
		return false;

	}
	/*-----------------ATIVAR TRANSPARÊNCIA-------------------------*/
	//al_convert_mask_to_alpha(personagem[PARADO], al_map_rgb(255, 0, 255));
	//al_convert_mask_to_alpha(personagem[COMENDO], al_map_rgb(255, 0, 255));
	//al_convert_mask_to_alpha(personagem[DORMINDO], al_map_rgb(255, 0, 255));
	al_convert_mask_to_alpha(objetos_cenario[MESA], al_map_rgb(255, 0, 255));
	al_convert_mask_to_alpha(objetos_cenario[SABONETE], al_map_rgb(255, 0, 255));
	//al_convert_mask_to_alpha(sprite_alimento, al_map_rgb(255, 0, 255));
	return true;
}

bool carregar_bolhas(Personagem p) {
	switch (p.tamanho) {
	case 1:bolhas_banheiro = al_load_bitmap("bolhas\\bolhas_1.png");
		if (!bolhas_banheiro) {
			error_msg("Falha ao carregar imagem");
			finalizar_bolhas();
			return false;
		}
		break;
	case 2:bolhas_banheiro = al_load_bitmap("bolhas\\bolhas_2.png");
		if (!bolhas_banheiro) {
			error_msg("Falha ao carregar imagem");
			finalizar_bolhas();
			return false;
		}
		break;
	case 3:bolhas_banheiro = al_load_bitmap("bolhas\\bolhas_3.png");
		if (!bolhas_banheiro) {
			error_msg("Falha ao carregar imagem");
			finalizar_bolhas();
			return false;
		}
		break;
	case 4:bolhas_banheiro = al_load_bitmap("bolhas\\bolhas_4.png");
		if (!bolhas_banheiro) {
			error_msg("Falha ao carregar imagem");
			finalizar_bolhas();
			return false;
		}
		break;
	}
	return true;
}

void finalizar_bolhas() {
	if (bolhas_banheiro != NULL) {
		al_destroy_bitmap(bolhas_banheiro);
		bolhas_banheiro = NULL;

	}
}

bool carregar_sujeira(Personagem p) {

	if (sujeira_personagem == NULL) {
		switch (p.tamanho) {
		case 1:sujeira_personagem = al_load_bitmap("sujeira\\sujeira_1.png");
			if (!sujeira_personagem) {
				return false;
			}
			break;
		case 2:sujeira_personagem = al_load_bitmap("sujeira\\sujeira_2.png");
			if (!sujeira_personagem) {
				return false;
			}
			break;
		case 3:sujeira_personagem = al_load_bitmap("sujeira\\sujeira_3.png");
			if (!sujeira_personagem) {
				return false;
			}
			break;
		case 4:sujeira_personagem = al_load_bitmap("sujeira\\sujeira_4.png");
			if (!sujeira_personagem) {
				return false;
			}
			break;
		}
	}
	return true;
}

void finalizar_sujeira() {
	if (sujeira_personagem != NULL) {
		al_destroy_bitmap(sujeira_personagem);
		sujeira_personagem = NULL;
	}
}

bool carregar_sprite_personagem() {
	Personagem p;
	Nome_sprite nome_sprite;
	p = inicializar_struct_personagem();
	alterar_sprite_tamanho(&nome_sprite, p);

	personagem[PARADO] = al_load_bitmap(nome_sprite.sprite_parado);
	if (!personagem[PARADO]) {
		error_msg("Falha ao carregar sprites");
		return false;
	}
	personagem[COMENDO] = al_load_bitmap(nome_sprite.sprite_comendo);
	if (!personagem[COMENDO]) {
		error_msg("Falha ao carregar sprites");
		finalizar_sprite_personagem();
		return false;
	}
	personagem[COM_SONO] = al_load_bitmap(nome_sprite.sprite_com_sono);
	if (!personagem[COM_SONO]) {
		error_msg("Falha ao carregar sprites");
		finalizar_sprite_personagem();
		return false;
	}
	personagem[DORMINDO] = al_load_bitmap(nome_sprite.sprite_dormindo);
	if (!personagem[DORMINDO]) {
		error_msg("Falha ao carregar sprites");
		finalizar_sprite_personagem();
		return false;
	}
	personagem[DOENTE] = al_load_bitmap(nome_sprite.sprite_doente);
	if (!personagem[DOENTE]) {
		error_msg("Falha ao carregar sprites");
		finalizar_sprite_personagem();
		return false;
	}
	return true;
}

void inicializar_vetor_sprite_personagem() {
	for (int i = 0; i < 6; i++) {
		personagem[i] = NULL;
	}
}

void finalizar_sprite_personagem() {
	
	for (int i = 0; i < 6; i++) {
		if (personagem[i] != NULL) {
			al_destroy_bitmap(personagem[i]);
			personagem[i] = NULL;
		}
	}
}

bool carregar_loja() {

	background_vetor[LOJA_MENU] = al_load_bitmap("loja\\loja_menu.png");
	if (!background_vetor[LOJA_MENU]) {
		error_msg("Falha ao carregar background");
		return false;
	}

	background_vetor[LOJA_COMIDA_1] = al_load_bitmap("loja\\loja_comida_1.png");
	if (!background_vetor[LOJA_COMIDA_1]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(background_vetor[LOJA_MENU]);
		return false;
	}
	background_vetor[LOJA_COMIDA_2] = al_load_bitmap("loja\\loja_comida_2.png");
	if (!background_vetor[LOJA_COMIDA_2]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(background_vetor[LOJA_MENU]);
		al_destroy_bitmap(background_vetor[LOJA_COMIDA_1]);
		return false;
	}
	background_vetor[LOJA_PERSONAGEM_1] = al_load_bitmap("loja\\loja_personagem_1.png");
	if (!background_vetor[LOJA_PERSONAGEM_1]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(background_vetor[LOJA_MENU]);
		al_destroy_bitmap(background_vetor[LOJA_COMIDA_1]);
		al_destroy_bitmap(background_vetor[LOJA_COMIDA_2]);
		return false;
	}
	background_vetor[LOJA_PERSONAGEM_2] = al_load_bitmap("loja\\loja_personagem_2.png");
	if (!background_vetor[LOJA_PERSONAGEM_2]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(background_vetor[LOJA_MENU]);
		al_destroy_bitmap(background_vetor[LOJA_COMIDA_1]);
		al_destroy_bitmap(background_vetor[LOJA_COMIDA_2]);
		al_destroy_bitmap(background_vetor[LOJA_PERSONAGEM_1]);
		return false;
	}
	background_vetor[LOJA_PERSONAGEM_3] = al_load_bitmap("loja\\loja_personagem_3.png");
	if (!background_vetor[LOJA_PERSONAGEM_3]) {
		error_msg("Falha ao carregar background");
		al_destroy_bitmap(background_vetor[LOJA_MENU]);
		al_destroy_bitmap(background_vetor[LOJA_COMIDA_1]);
		al_destroy_bitmap(background_vetor[LOJA_COMIDA_2]);
		al_destroy_bitmap(background_vetor[LOJA_PERSONAGEM_1]);
		al_destroy_bitmap(background_vetor[LOJA_PERSONAGEM_2]);
		return false;
	}
	return true;

}

bool carregar_geladeira() {
	/*------------------CARREGAR BACKGROUND-------------------------*/
	background_vetor[GELADEIRA] = al_load_bitmap("background_geladeira.png");
	if (!background_vetor[GELADEIRA]) {
		error_msg("Falha ao carregar imagem obejto");
		return false;
	}
	/*-------------------CARREGAR ALIMENTOS----------------------------*/
	objetos_cenario[BANANA] = al_load_bitmap("banana.png");
	if (!objetos_cenario[BANANA]) {
		error_msg("Falha ao carregar imagem obejto");
		al_destroy_bitmap(background_vetor[GELADEIRA]);
		return false;
	}
	/*-----------------ATIVAR TRANSPARÊNCIA-------------------------*/
	al_convert_mask_to_alpha(objetos_cenario[BANANA], al_map_rgb(255, 0, 255));
	return true;
}

bool carregar_guarda_roupa() {
	background_vetor[GUARDA_ROUPA_1] = al_load_bitmap("guarda_roupa\\guarda_roupa_1.png");
	if (!background_vetor[GUARDA_ROUPA_1]) {
		error_msg("Falha ao carregar imagem");
		return false;
	}
	background_vetor[GUARDA_ROUPA_2] = al_load_bitmap("guarda_roupa\\guarda_roupa_2.png");
	if (!background_vetor[GUARDA_ROUPA_2]) {
		error_msg("Falha ao carregar imagem");
		al_destroy_bitmap(background_vetor[GUARDA_ROUPA_1]);
		return false;
	}
	return true;
}

void error_msg(char *text) {
	al_show_native_message_box(janela, "ERRO",
		"Ocorreu o seguinte erro e o programa sera finalizado:",
		text, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

void finalizar() {
	al_destroy_display(janela);
	al_destroy_event_queue(fila_eventos);
	al_destroy_font(font_status);
	al_destroy_timer(temporizador);
	al_destroy_bitmap(background_vetor[SALA]);
	al_destroy_bitmap(background_vetor[COZINHA]);
	al_destroy_bitmap(background_vetor[BANHEIRO]);
	al_destroy_bitmap(background_vetor[QUARTO_ACESO]);
	al_destroy_bitmap(background_vetor[QUARTO_APAGADO]);
	al_destroy_bitmap(background_vetor[SALA_DE_JOGOS]);
	al_destroy_bitmap(background_vetor[TELA_STATUS]);
	al_destroy_bitmap(background_vetor[MINI_JOGOS]);
	al_destroy_bitmap(personagem[PARADO]);
	al_destroy_bitmap(personagem[COMENDO]);
	al_destroy_bitmap(personagem[DORMINDO]);
	al_destroy_bitmap(objetos_cenario[MESA]);
	al_destroy_bitmap(objetos_cenario[SABONETE]);
	al_destroy_bitmap(sprite_alimento);
	finalizar_bolhas();
	al_destroy_sample(efeitos[0]);
	al_destroy_sample(efeitos[1]);
	al_destroy_sample(efeitos[2]);
	al_fclose(save_personagem);
	al_fclose(save_cenario);
	al_fclose(save_data);
	al_fclose(save_alimento);
}

void finalizar_loja() {
	al_destroy_bitmap(background_vetor[LOJA_MENU]);
	al_destroy_bitmap(background_vetor[LOJA_COMIDA_1]);
	al_destroy_bitmap(background_vetor[LOJA_COMIDA_2]);
	al_destroy_bitmap(background_vetor[LOJA_PERSONAGEM_1]);
	al_destroy_bitmap(background_vetor[LOJA_PERSONAGEM_2]);
	al_destroy_bitmap(background_vetor[LOJA_PERSONAGEM_3]);
}

void finalizar_geladeira() {
	al_destroy_bitmap(background_vetor[GELADEIRA]);
	al_destroy_bitmap(objetos_cenario[BANANA]);
}

void finalizar_guarda_roupa() {
	al_destroy_bitmap(background_vetor[GUARDA_ROUPA_1]);
	al_destroy_bitmap(background_vetor[GUARDA_ROUPA_2]);
}

void trocar_background_principal(int cenario) {

	switch (cenario) {
	case SALA: background = background_vetor[SALA];
		break;
	case COZINHA: background = background_vetor[COZINHA];
		break;
	case BANHEIRO: background = background_vetor[BANHEIRO];
		break;
	case QUARTO_ACESO: background = background_vetor[QUARTO_ACESO];
		break;
	case QUARTO_APAGADO:background = background_vetor[QUARTO_APAGADO];
		break;
	case SALA_DE_JOGOS:background = background_vetor[SALA_DE_JOGOS];
		break;
	case GELADEIRA: background = background_vetor[GELADEIRA];
		break;
	case LOJA_MENU:background = background_vetor[LOJA_MENU];
		break;
	case LOJA_COMIDA_1: background = background_vetor[LOJA_COMIDA_1];
		break;
	case LOJA_COMIDA_2: background = background_vetor[LOJA_COMIDA_2];
		break;
	case LOJA_PERSONAGEM_1: background = background_vetor[LOJA_PERSONAGEM_1];
		break;
	case LOJA_PERSONAGEM_2: background = background_vetor[LOJA_PERSONAGEM_2];
		break;
	case TELA_STATUS: background = background_vetor[TELA_STATUS];
		break;
	case MINI_JOGOS: background = background_vetor[MINI_JOGOS];
		break;
	case GUARDA_ROUPA_1: background = background_vetor[GUARDA_ROUPA_1];
		break;
	case GUARDA_ROUPA_2: background = background_vetor[GUARDA_ROUPA_2];
		break;

	}
}
//978 40 

void crescer_personagem(Personagem p) {

	if (p.nivel == 2) {
		//p.file_flag = false;
		//p.interagir = false;
		//p.cor = BRANCO;
		p.tamanho = 2;
		//p.dinheiro = 50;
		//p.status = PARADO;
		p.altura_sprite = 256;
		p.largura_sprite = 256;
		p.regiao_x_folha = 0;
		p.regiao_y_folha = 0;
		p.pos_sprite_x = 384;
		p.pos_sprite_y = 370;
		p.progresso.porcentagem_energia = 100;
		p.progresso.porcentagem_fome = 100;
		p.progresso.porcentagem_saude = 100;
		p.progresso.porcentgem_peso = 0;
		p.progresso.progresso_nivel = 0;
		save_file_personagem(p, save_personagem);
		finalizar_sprite_personagem();
		carregar_sprite_personagem();
	}

	else if (p.nivel == 3) {
		//p.file_flag = false;
		//p.interagir = false;
		//p.cor = BRANCO;
		p.tamanho = 3;
		//p.dinheiro = 50;
		//p.status = PARADO;
		p.altura_sprite = 300;
		p.largura_sprite = 300;
		p.regiao_x_folha = 0;
		p.regiao_y_folha = 0;
		p.pos_sprite_x = 362;
		p.pos_sprite_y = 330;
		p.progresso.porcentagem_energia = 100;
		p.progresso.porcentagem_fome = 100;
		p.progresso.porcentagem_saude = 100;
		p.progresso.porcentgem_peso = 0;
		p.progresso.progresso_nivel = 0;
		save_file_personagem(p, save_personagem);
		finalizar_sprite_personagem();
		carregar_sprite_personagem();
	}

	else if (p.nivel == 4) {
		//p.file_flag = false;
		//p.interagir = false;
		//p.cor = BRANCO;
		p.tamanho = 4;
		//p.dinheiro = 50;
		//p.status = PARADO;
		p.altura_sprite = 356;
		p.largura_sprite = 356;
		p.regiao_x_folha = 0;
		p.regiao_y_folha = 0;
		p.pos_sprite_x = 334;
		p.pos_sprite_y = 270;
		p.progresso.porcentagem_energia = 100;
		p.progresso.porcentagem_fome = 100;
		p.progresso.porcentagem_saude = 100;
		p.progresso.porcentgem_peso = 0;
		p.progresso.progresso_nivel = 0;
		save_file_personagem(p, save_personagem);
		finalizar_sprite_personagem();
		carregar_sprite_personagem();
	}
}

void alterar_sprite_tamanho(Nome_sprite *nome_sprite, Personagem p) {

	if (p.tamanho == 1 && p.cor == BRANCO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_1.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_1.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_1.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_1.png");
	}

	else if (p.tamanho == 2 && p.cor == BRANCO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_2.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_2.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_2.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_2.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_2.png");
	}
	else if (p.tamanho == 3 && p.cor == BRANCO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_3.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_3.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_3.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_3.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_3.png");
	}
	else if (p.tamanho == 4 && p.cor == BRANCO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_4.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_4.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_4.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_4.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_4.png");
	}

	else if (p.tamanho == 1 && p.cor == AZUL) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_1_azul.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_1_azul.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_1_azul.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_1_azul.png");
	}
	else if (p.tamanho == 2 && p.cor == AZUL) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_2_azul.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_2_azul.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_2_azul.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_2.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_2_azul.png");
	}
	else if (p.tamanho == 3 && p.cor == AZUL) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_3_azul.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_3_azul.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_3_azul.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_3.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_3_azul.png");
	}
	else if (p.tamanho == 4 && p.cor == AZUL) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_4_azul.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_4_azul.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_4_azul.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_4.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_4_azul.png");
	}
	else if (p.tamanho == 1 && p.cor == VERMELHO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_1_vermelho.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_1_vermelho.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_1_vermelho.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_1_vermelho.png");
	}
	else if (p.tamanho == 2 && p.cor == VERMELHO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_2_vermelho.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_2_vermelho.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_2_vermelho.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_2_vermelho.png");
	}
	else if (p.tamanho == 3 && p.cor == VERMELHO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_3_vermelho.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_3_vermelho.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_3_vermelho.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_3_vermelho.png");
	}
	else if (p.tamanho == 4 && p.cor == VERMELHO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_4_vermelho.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_4_vermelho.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_4_vermelho.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_4_vermelho.png");
	}
	else if (p.tamanho == 1 && p.cor == AMARELO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_1_amarelo.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_1_amarelo.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_1_amarelo.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_1_amarelo.png");
	}
	else if (p.tamanho == 2 && p.cor == AMARELO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_2_amarelo.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_2_amarelo.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_2_amarelo.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_2_amarelo.png");
	}
	else if (p.tamanho == 3 && p.cor == AMARELO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_3_amarelo.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_3_amarelo.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_3_amarelo.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_3_amarelo.png");
	}
	else if (p.tamanho == 4 && p.cor == AMARELO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_4_amarelo.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_4_amarelo.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_4_amarelo.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_4_amarelo.png");
	}
	else if (p.tamanho == 1 && p.cor == VERDE) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_1_verde.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_1_verde.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_1_verde.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_1_verde.png");
	}
	else if (p.tamanho == 2 && p.cor == VERDE) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_2_verde.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_2_verde.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_2_verde.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_2_verde.png");
	}
	else if (p.tamanho == 3 && p.cor == VERDE) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_3_verde.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_3_verde.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_3_verde.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_3_verde.png");
	}
	else if (p.tamanho == 4 && p.cor == VERDE) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_4_verde.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_4_verde.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_4_verde.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_4_verde.png");
	}
	else if (p.tamanho == 1 && p.cor == ROSA) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_1_rosa.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_1_rosa.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_1_rosa.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_1_rosa.png");
	}
	else if (p.tamanho == 2 && p.cor == ROSA) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_2_rosa.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_2_rosa.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_2_rosa.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_2_rosa.png");
	}
	else if (p.tamanho == 3 && p.cor == ROSA) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_3_rosa.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_3_rosa.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_3_rosa.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_3_rosa.png");
	}
	else if (p.tamanho == 4 && p.cor == ROSA) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_4_rosa.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_4_rosa.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_4_rosa.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_4_rosa.png");
	}
	else if (p.tamanho == 1 && p.cor == ROXO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_1_roxo.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_1_roxo.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_1_roxo.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_1_roxo.png");
	}
	else if (p.tamanho == 2 && p.cor == ROXO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_2_roxo.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_2_roxo.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_2_roxo.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_2_roxo.png");
	}
	else if (p.tamanho == 3 && p.cor == ROXO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_3_roxo.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_3_roxo.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_3_roxo.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_3_roxo.png");
	}
	else if (p.tamanho == 4 && p.cor == ROXO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_4_roxo.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_4_roxo.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_4_roxo.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_4_roxo.png");
	}
	else if (p.tamanho == 1 && p.cor == PRETO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_1_preto.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_1_preto.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_1_preto.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_1_preto.png");
	}
	else if (p.tamanho == 2 && p.cor == PRETO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_2_preto.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_2_preto.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_2_preto.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_2_preto.png");
	}
	else if (p.tamanho == 3 && p.cor == PRETO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_3_preto.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_3_preto.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_3_preto.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_3_preto.png");
	}
	else if (p.tamanho == 4 && p.cor == PRETO) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_4_preto.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_4_preto.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_4_preto.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_4_preto.png");
	}
	else if (p.tamanho == 1 && p.cor == LARANJA) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_1_laranja.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_1_laranja.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_1_laranja.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_1_laranja.png");
	}
	else if (p.tamanho == 2 && p.cor == LARANJA) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_2_laranja.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_2_laranja.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_2_laranja.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_2_laranja.png");
	}
	else if (p.tamanho == 3 && p.cor == LARANJA) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_3_laranja.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_3_laranja.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_3_laranja.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_3_laranja.png");
	}
	else if (p.tamanho == 4 && p.cor == LARANJA) {
		strcpy(nome_sprite->sprite_parado, "personagem\\sprite_parado_4_laranja.png");
		strcpy(nome_sprite->sprite_com_sono, "personagem\\sprite_sono_4_laranja.png");
		strcpy(nome_sprite->sprite_comendo, "personagem\\sprite_comendo_4_laranja.png");
		strcpy(nome_sprite->sprite_dormindo, "personagem\\sprite_dormindo_1.png");
		strcpy(nome_sprite->sprite_doente, "personagem\\sprite_doente_4_laranja.png");
	}
	
}

void trocar_cor_personagem(Personagem p, int cor) {
	    p.cor = cor;
		save_file_personagem(p, save_personagem);
		finalizar_sprite_personagem();
		carregar_sprite_personagem();
}

//se ele ficar doente, com fome e sujo
void trocar_status_personagem(Personagem *p, TData ultima_data, Cenario c) {
	TData dataAtual = data_atual();

	progresso_energia(&*p, ultima_data);
	progresso_fome(&*p, ultima_data);
	progresso_limpeza(&*p, ultima_data);
	progresso_saude(&*p, ultima_data);
	//p->ultimo_status = p->status;
	if (c.cenario_atual != QUARTO_APAGADO) {
		p->status = PARADO;
	}

	if (p->progresso.porcentagem_energia < 20) {
		if (c.cenario_atual != QUARTO_APAGADO)
		p->status = COM_SONO;
	}
	if (p->progresso.porcentagem_fome < 20) {
		//p->status = COM_FOME;
	}
	if (p->progresso.porcentagem_limpeza < 20) {
		//p->status = SUJO;
	}
	if (p->progresso.porcentagem_saude < 50) {
		if (c.cenario_atual != QUARTO_APAGADO)
		p->status = DOENTE;
	}
	if (p->progresso.porcentgem_peso > 110) {
		//p->status = GORDO;
	}


	
	p->ultimo_status = p->status;
	save_file_data(dataAtual, save_data);
	//*p = inicializar_struct_personagem();

}

Mini_jogo selecionar_mini_jogo(int x, int y, ALLEGRO_EVENT *evento) {
	Mini_jogo jogo;
	jogo.dinheiro_ganho = 0;
	jogo.progresso = 0;
	jogo.error = false;
	jogo.id = 0;


	/*-----------------------abrir_mini_jogo--------------------------*/
	if (x >= 74 && x <= 208 && y >= 146 && y <= 274) {
		jogo = mini_jogo_repita_sequencia(fila_eventos, &*evento, font_status, janela);
		
	}
	else if (x >= 255 && x <= 390 && y >= 146 && y <= 274) {
		jogo = mini_jogo_matematica(fila_eventos, &*evento);
	}
	else if (x >= 430 && x <= 565 && y >= 146 && y <= 274) {
		jogo = mini_jogo_chuva_de_letras(fila_eventos, &*evento, &*temporizador);
	}
	
	if (jogo.error == true) {
		error_msg("Falha ao carregar mini_jogo");
		jogo.dinheiro_ganho = 0;
		jogo.progresso = 0;
	}
	return jogo;
}

void comprar_comida(Personagem *p,Cenario c , int x, int y) {
	Alimento alimento;
	alimento.id_alimento = 0;
	alimento.quantidade = 1;
	alimento.preco = 0;
	alimento.nivel_necessario = 0;
	//alimento.status_file = true;

	if (c.cenario_atual == LOJA_COMIDA_1) {
		if (x >= 84 && x <= 156 && y >= 237 && y <= 262) {
			alimento.id_alimento = 1;
			alimento.preco = 2;
			alimento.progresso_comida = 8;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 280 && x <= 352 && y >= 237 && y <= 262) {
			alimento.id_alimento = 2;
			alimento.preco = 2;
			alimento.progresso_comida = 8;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 466 && x <= 538 && y >= 237 && y <= 262) {
			alimento.id_alimento = 3;
			alimento.preco = 3;
			alimento.progresso_comida = 12;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 678 && x <= 751 && y >= 237 && y <= 262) {
			alimento.id_alimento = 4;
			alimento.preco = 2;
			alimento.progresso_comida = 8;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 866 && x <= 936 && y >= 237 && y <= 262) {
			alimento.id_alimento = 5;
			alimento.preco = 2;
			alimento.progresso_comida = 8;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 84 && x <= 156 && y >= 403 && y <= 427) {
			alimento.id_alimento = 6;
			alimento.preco = 1;
			alimento.progresso_comida = 5;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 280 && x <= 352 && y >= 403 && y <= 427) {
			alimento.id_alimento = 7;
			alimento.preco = 1;
			alimento.progresso_comida = 5;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 466 && x <= 538 && y >= 403 && y <= 427) {
			alimento.id_alimento = 8;
			alimento.preco = 5;
			alimento.progresso_comida = 18;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 678 && x <= 751 && y >= 403 && y <= 427) {
			alimento.id_alimento = 9;
			alimento.preco = 3;
			alimento.progresso_comida = 12;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 866 && x <= 936 && y >= 403 && y <= 427) {
			alimento.id_alimento = 10;
			alimento.preco = 4;
			alimento.progresso_comida = 14;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 84 && x <= 156 && y >= 565 && y <= 591) {
			alimento.id_alimento = 11;
			alimento.preco = 1;
			alimento.progresso_comida = 5;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 280 && x <= 352 && y >= 565 && y <= 591) {
			alimento.id_alimento = 12;
			alimento.preco = 1;
			alimento.progresso_comida = 5;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 466 && x <= 538 && y >= 565 && y <= 591) {
			alimento.id_alimento = 13;
			alimento.preco = 2;
			alimento.progresso_comida = 8;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 678 && x <= 751 && y >= 565 && y <= 591) {
			alimento.id_alimento = 14;
			alimento.preco = 1;
			alimento.progresso_comida = 5;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
		}
		else if (x >= 866 && x <= 936 && y >= 565 && y <= 591) {
			alimento.id_alimento = 15;
			alimento.preco = 2;
			alimento.progresso_comida = 8;
			if (validar_compra(p->dinheiro, alimento.preco) == true) {
				p->dinheiro -= alimento.preco;
				change_file_alimento(save_alimento, alimento);
				al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}

		}
	}
	else if (c.cenario_atual == LOJA_COMIDA_2) {
			if (x >= 84 && x <= 156 && y >= 237 && y <= 262) {
				alimento.id_alimento = 16;
				alimento.preco = 2;
				alimento.progresso_comida = 8;
				if (validar_compra(p->dinheiro, alimento.preco) == true) {
					p->dinheiro -= alimento.preco;
					change_file_alimento(save_alimento, alimento);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
			else if (x >= 280 && x <= 352 && y >= 237 && y <= 262) {
				alimento.id_alimento = 17;
				alimento.preco = 1;
				alimento.progresso_comida = 5;
				if (validar_compra(p->dinheiro, alimento.preco) == true) {
					p->dinheiro -= alimento.preco;
					change_file_alimento(save_alimento, alimento);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
			else if (x >= 466 && x <= 538 && y >= 237 && y <= 262) {
				alimento.id_alimento = 18;
				alimento.preco = 3;
				alimento.progresso_comida = 12;
				if (validar_compra(p->dinheiro, alimento.preco) == true) {
					p->dinheiro -= alimento.preco;
					change_file_alimento(save_alimento, alimento);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
			else if (x >= 678 && x <= 751 && y >= 237 && y <= 262) {
				alimento.id_alimento = 19;
				alimento.preco = 2;
				alimento.progresso_comida = 8;
				if (validar_compra(p->dinheiro, alimento.preco) == true) {
					p->dinheiro -= alimento.preco;
					change_file_alimento(save_alimento, alimento);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
			else if (x >= 866 && x <= 936 && y >= 237 && y <= 262) {
				alimento.id_alimento = 20;
				alimento.preco = 2;
				alimento.progresso_comida = 8;
				if (validar_compra(p->dinheiro, alimento.preco) == true) {
					p->dinheiro -= alimento.preco;
					change_file_alimento(save_alimento, alimento);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
			else if (x >= 84 && x <= 156 && y >= 403 && y <= 427) {
				alimento.id_alimento = 21;
				alimento.preco = 7;
				alimento.progresso_comida = 25;
				if (validar_compra(p->dinheiro, alimento.preco) == true) {
					p->dinheiro -= alimento.preco;
					change_file_alimento(save_alimento, alimento);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
			else if (x >= 280 && x <= 352 && y >= 403 && y <= 427) {
				alimento.id_alimento = 22;
				alimento.preco = 7;
				alimento.progresso_comida = 25;
				if (validar_compra(p->dinheiro, alimento.preco) == true) {
					p->dinheiro -= alimento.preco;
					change_file_alimento(save_alimento, alimento);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
			else if (x >= 466 && x <= 538 && y >= 403 && y <= 427) {
				alimento.id_alimento = 23;
				alimento.preco = 5;
				alimento.progresso_comida = 18;
				if (validar_compra(p->dinheiro, alimento.preco) == true) {
					p->dinheiro -= alimento.preco;
					change_file_alimento(save_alimento, alimento);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
			else if (x >= 678 && x <= 751 && y >= 403 && y <= 427) {
				alimento.id_alimento = 24;
				alimento.preco = 5;
				alimento.progresso_comida = 18;
				if (validar_compra(p->dinheiro, alimento.preco) == true) {
					p->dinheiro -= alimento.preco;
					change_file_alimento(save_alimento, alimento);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
			else if (x >= 866 && x <= 936 && y >= 403 && y <= 427) {
				alimento.id_alimento = 25;
				alimento.preco = 3;
				alimento.progresso_comida = 12;
				if (validar_compra(p->dinheiro, alimento.preco) == true) {
					p->dinheiro -= alimento.preco;
					change_file_alimento(save_alimento, alimento);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
			else if (x >= 84 && x <= 156 && y >= 565 && y <= 591) {
				alimento.id_alimento = 26;
				alimento.preco = 2;
				alimento.progresso_comida = 8;
				if (validar_compra(p->dinheiro, alimento.preco) == true) {
					p->dinheiro -= alimento.preco;
					change_file_alimento(save_alimento, alimento);
				}
			}
			else if (x >= 280 && x <= 352 && y >= 565 && y <= 591) {
				alimento.id_alimento = 27;
				alimento.preco = 4;
				alimento.progresso_comida = 14;
				if (validar_compra(p->dinheiro, alimento.preco) == true) {
					p->dinheiro -= alimento.preco;
					change_file_alimento(save_alimento, alimento);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
			else if (x >= 466 && x <= 538 && y >= 565 && y <= 591) {
				alimento.id_alimento = 28;
				alimento.preco = 1;
				alimento.progresso_comida = 5;
				if (validar_compra(p->dinheiro, alimento.preco) == true) {
					p->dinheiro -= alimento.preco;
					change_file_alimento(save_alimento, alimento);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
	}

	
	//reload vet alimento
}

bool validar_compra(int dinheiro, int valor_item) {

	if (dinheiro < valor_item) {
		return false;
	}
	return true;
}

void comer(Personagem *p, INTERAGIR_COZINHA *interagir_alimento, Alimento *vet_alimento, int id_alimento) {

	p->progresso.porcentagem_fome += vet_alimento[id_alimento].progresso_comida;
	p->progresso.porcentagem_saude += 10;
	if (p->progresso.porcentagem_saude > 100) {
		p->progresso.porcentagem_saude = 100;
	}
	vet_alimento[id_alimento].quantidade--;
	if (vet_alimento[id_alimento].quantidade == 0) {//ajei
		interagir_alimento->comer = true;
	}
	vet_alimento[id_alimento].quantidade = -1;
	change_file_alimento(save_alimento, vet_alimento[id_alimento]);
}

void comprar_aparencia(Personagem *p, Cenario c, Aparencia *aparencia_vet, int x, int y) {
	Aparencia aparencia;
	aparencia.id = 0;
	aparencia.preco = 0;

	if (c.cenario_atual == LOJA_PERSONAGEM_1) {
		if (x >= 103 && x <= 268 && y >= 340 && y <= 372) {
			if (buscar_aparencia(aparencia_vet, AZUL) == false) {
				aparencia.id = AZUL;
				aparencia.preco = 100;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
		else if (x >= 322 && x <= 482 && y >= 340 && y <= 372) {
			if (buscar_aparencia(aparencia_vet, VERMELHO) == false) {
				aparencia.id = VERMELHO;
				aparencia.preco = 100;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
		else if (x >= 538 && x <= 702 && y >= 340 && y <= 372) {
			if (buscar_aparencia(aparencia_vet, AMARELO) == false) {
				aparencia.id = AMARELO;
				aparencia.preco = 100;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
		else if (x >= 756 && x <= 918 && y >= 340 && y <= 372) {
			if (buscar_aparencia(aparencia_vet, VERDE) == false) {
				aparencia.id = VERDE;
				aparencia.preco = 100;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
		else if (x >= 103 && x <= 268 && y >= 632 && y <= 664) {
			if (buscar_aparencia(aparencia_vet, ROSA) == false) {
				aparencia.id = ROSA;
				aparencia.preco = 200;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
		else if (x >= 322 && x <= 482 && y >= 632 && y <= 664) {
			if (buscar_aparencia(aparencia_vet, ROXO) == false) {
				aparencia.id = ROXO;
				aparencia.preco = 300;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}

		else if (x >= 538 && x <= 702 && y >= 632 && y <= 664) {
			if (buscar_aparencia(aparencia_vet, PRETO) == false) {
				aparencia.id = PRETO;
				aparencia.preco = 300;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
		else if (x >= 756 && x <= 918 && y >= 632 && y <= 664) {
			if (buscar_aparencia(aparencia_vet, LARANJA) == false) {
				aparencia.id = LARANJA;
				aparencia.preco = 300;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
	}
	else if (c.cenario_atual == LOJA_PERSONAGEM_2) {
		if (x >= 103 && x <= 268 && y >= 340 && y <= 372) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_1) == false) {
				aparencia.id = ACESSORIO_1;
				aparencia.preco = 100;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
		else if (x >= 322 && x <= 482 && y >= 340 && y <= 372) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_2) == false) {
				aparencia.id = ACESSORIO_2;
				aparencia.preco = 60;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
		else if (x >= 538 && x <= 702 && y >= 340 && y <= 372) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_3) == false) {
				aparencia.id =ACESSORIO_3;
				aparencia.preco = 50;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
		else if (x >= 756 && x <= 918 && y >= 340 && y <= 372) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_4) == false) {
				aparencia.id =ACESSORIO_4;
				aparencia.preco = 50;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
		else if (x >= 103 && x <= 268 && y >= 632 && y <= 664) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_5) == false) {
				aparencia.id = ACESSORIO_5;
				aparencia.preco = 50;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
		else if (x >= 322 && x <= 482 && y >= 632 && y <= 664) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_6) == false) {
				aparencia.id = ACESSORIO_6;
				aparencia.preco = 50;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}

		else if (x >= 538 && x <= 702 && y >= 632 && y <= 664) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_7) == false) {
				aparencia.id = ACESSORIO_7;
				aparencia.preco = 90;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
		else if (x >= 756 && x <= 918 && y >= 632 && y <= 664) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_8) == false) {
				aparencia.id = ACESSORIO_8;
				aparencia.preco = 90;
				if (validar_compra(p->dinheiro, aparencia.preco) == true) {
					p->dinheiro -= aparencia.preco;
					cout << p->dinheiro << endl;
					save_file_aparencia(aparencia, save_aparencia);
					al_play_sample(efeitos[2], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
			}
		}
	}

}

bool buscar_aparencia(Aparencia *aparencia_vet, int id_aparencia) {
	
	for (int i = 0; i < 29; i++) {
		if (aparencia_vet[i].id == id_aparencia) {
			return true;
		}
	}
	return false;
}

void selecionar_cor_personagem(Personagem p, Cenario c, Aparencia *aparencia_vet, int x, int y) {

	if (c.cenario_atual == GUARDA_ROUPA_1) {
		if (x >= 410 && x <= 517 && y >= 132 && y <= 240) {
			trocar_cor_personagem(p, BRANCO);
		}
		else if (x >= 104 && x <= 216 && y >= 132 && y <= 240) {
			if (buscar_aparencia(aparencia_vet, AZUL) == true) {
				trocar_cor_personagem(p, AZUL);
			}
		}
		else if (x >= 262 && x <= 370 && y >= 132 && y <= 240) {
			if (buscar_aparencia(aparencia_vet, VERMELHO) == true) {
				trocar_cor_personagem(p, VERMELHO);
			}
		}
		else if (x >= 104 && x <= 216 && y >= 272 && y <= 377) {
			if (buscar_aparencia(aparencia_vet, AMARELO) == true) {
				trocar_cor_personagem(p, AMARELO);
			}
		}
		else if (x >= 262 && x <= 370 && y >= 272 && y <= 377) {
			if (buscar_aparencia(aparencia_vet, VERDE) == true) {
				trocar_cor_personagem(p, VERDE);
			}
		}
		else if (x >= 104 && x <= 216 && y >= 410 && y <= 516) {
			if (buscar_aparencia(aparencia_vet, ROSA) == true) {
				trocar_cor_personagem(p, ROSA);
			}
		}
		else if (x >= 262 && x <= 370 && y >= 410 && y <= 516) {
			if (buscar_aparencia(aparencia_vet, ROXO) == true) {
				trocar_cor_personagem(p, ROXO);
			}
		}
		else if (x >= 104 && x <= 216 && y >= 570 && y <= 660) {
			if (buscar_aparencia(aparencia_vet, PRETO) == true) {
				trocar_cor_personagem(p, PRETO);
			}
		}
		else if (x >= 262 && x <= 370 && y >= 570 && y <= 660) {
			if (buscar_aparencia(aparencia_vet, LARANJA) == true) {
				trocar_cor_personagem(p, LARANJA);
			}
		}
	}
}

void selecionar_acessorio_personagem(Personagem p, Cenario c, Aparencia *aparencia_vet, bool *des_acessorio, int *id_acessorio, int x, int y) {

	if (c.cenario_atual == GUARDA_ROUPA_2) {

		if (x >= 410 && x <= 517 && y >= 132 && y <= 240) {
			*des_acessorio = false;
			*id_acessorio = 0;
			carregar_acessorio(p, *id_acessorio);
		}
		
		if (x >= 104 && x <= 216 && y >= 132 && y <= 240) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_1) == true) {
				*des_acessorio = true;
				*id_acessorio = ACESSORIO_1;
				carregar_acessorio(p, *id_acessorio);
			}
		}
		else if (x >= 262 && x <= 370 && y >= 132 && y <= 240) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_2) == true) {
				*des_acessorio = true;
				*id_acessorio = ACESSORIO_2;
				carregar_acessorio(p, *id_acessorio);
			}
		}
		else if (x >= 104 && x <= 216 && y >= 272 && y <= 377) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_3) == true) {
				*des_acessorio = true;
				*id_acessorio = ACESSORIO_3;
				carregar_acessorio(p, *id_acessorio);
			}
		}
		else if (x >= 262 && x <= 370 && y >= 272 && y <= 377) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_4) == true) {
				*des_acessorio = true;
				*id_acessorio = ACESSORIO_4;
				carregar_acessorio(p, *id_acessorio);
			}
		}
		else if (x >= 104 && x <= 216 && y >= 410 && y <= 516) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_5) == true) {
				*des_acessorio = true;
				*id_acessorio = ACESSORIO_5;
				carregar_acessorio(p, *id_acessorio);
			}
		}
		else if (x >= 262 && x <= 370 && y >= 410 && y <= 516) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_6) == true) {
				*des_acessorio = true;
				*id_acessorio = ACESSORIO_6;
				carregar_acessorio(p,*id_acessorio);
			}
		}
		else if (x >= 104 && x <= 216 && y >= 570 && y <= 660) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_7) == true) {
				*des_acessorio = true;
				*id_acessorio = ACESSORIO_7;
				carregar_acessorio(p, *id_acessorio);
			}
		}
		else if (x >= 262 && x <= 370 && y >= 570 && y <= 660) {
			if (buscar_aparencia(aparencia_vet, ACESSORIO_8) == true) {
				*des_acessorio = true;
				*id_acessorio = ACESSORIO_8;
				carregar_acessorio(p, *id_acessorio);
			}
		}
	}
}

bool carregar_acessorio(Personagem p, int id_acessorio) {

	if (acessorio != NULL) {
		al_destroy_bitmap(acessorio);
		acessorio = NULL;
	}
	if (acessorio == NULL) {
		
		switch (id_acessorio) {
		case ACESSORIO_1:
			if (p.tamanho == 1) {
				acessorio = al_load_bitmap("acessorios\\acessorio_1_1.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}
			}
			else if (p.tamanho == 2) {
				acessorio = al_load_bitmap("acessorios\\acessorio_1_2.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 3) {
				acessorio = al_load_bitmap("acessorios\\acessorio_1_3.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 4) {
				acessorio = al_load_bitmap("acessorios\\acessorio_1_4.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			break;
		case ACESSORIO_2:
			if (p.tamanho == 1) {
				cout << "lloiid" << endl;
				acessorio = al_load_bitmap("acessorios\\acessorio_2_1.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}
			}
			else if (p.tamanho == 2) {
				acessorio = al_load_bitmap("acessorios\\acessorio_2_2.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 3) {
				acessorio = al_load_bitmap("acessorios\\acessorio_2_3.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 4) {
				acessorio = al_load_bitmap("acessorios\\acessorio_2_4.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			break;
		case ACESSORIO_3:
			if (p.tamanho == 1) {
				acessorio = al_load_bitmap("acessorios\\acessorio_3_1.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}
			}
			else if (p.tamanho == 2) {
				acessorio = al_load_bitmap("acessorios\\acessorio_3_2.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 3) {
				acessorio = al_load_bitmap("acessorios\\acessorio_3_3.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 4) {
				acessorio = al_load_bitmap("acessorios\\acessorio_3_4.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			break;
		case ACESSORIO_4:
			if (p.tamanho == 1) {
				acessorio = al_load_bitmap("acessorios\\acessorio_4_1.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}
			}
			else if (p.tamanho == 2) {
				acessorio = al_load_bitmap("acessorios\\acessorio_4_2.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 3) {
				acessorio = al_load_bitmap("acessorios\\acessorio_4_3.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 4) {
				acessorio = al_load_bitmap("acessorios\\acessorio_4_4.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			break;
		case ACESSORIO_5:
			if (p.tamanho == 1) {
				acessorio = al_load_bitmap("acessorios\\acessorio_5_1.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}
			}
			else if (p.tamanho == 2) {
				acessorio = al_load_bitmap("acessorios\\acessorio_5_2.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 3) {
				acessorio = al_load_bitmap("acessorios\\acessorio_5_3.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 4) {
				acessorio = al_load_bitmap("acessorios\\acessorio_5_4.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			break;
		case ACESSORIO_6:
			if (p.tamanho == 1) {
				acessorio = al_load_bitmap("acessorios\\acessorio_6_1.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}
			}
			else if (p.tamanho == 2) {
				acessorio = al_load_bitmap("acessorios\\acessorio_6_2.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 3) {
				acessorio = al_load_bitmap("acessorios\\acessorio_6_3.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 4) {
				acessorio = al_load_bitmap("acessorios\\acessorio_6_4.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			break;
		case ACESSORIO_7:
			if (p.tamanho == 1) {
				acessorio = al_load_bitmap("acessorios\\acessorio_7_1.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}
			}
			else if (p.tamanho == 2) {
				acessorio = al_load_bitmap("acessorios\\acessorio_7_2.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 3) {
				acessorio = al_load_bitmap("acessorios\\acessorio_7_3.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 4) {
				acessorio = al_load_bitmap("acessorios\\acessorio_7_4.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			break;
		case ACESSORIO_8:
			if (p.tamanho == 1) {
				acessorio = al_load_bitmap("acessorios\\acessorio_8_1.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}
			}
			else if (p.tamanho == 2) {
				acessorio = al_load_bitmap("acessorios\\acessorio_8_2.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 3) {
				acessorio = al_load_bitmap("acessorios\\acessorio_8_3.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			else if (p.tamanho == 4) {
				acessorio = al_load_bitmap("acessorios\\acessorio_8_4.png");
				if (!acessorio) {
					error_msg("Falha ao carregar imagem");
					return false;
				}

			}
			break;
		}
	}
}

int buscar_alimento(Alimento vet_alimento[], int id) {

	for (int i = id; i < 30; i++) {
		//cout << vet_alimento[i].id_alimento<<endl;
		if (vet_alimento[i].id_alimento > 0 && vet_alimento[i].id_alimento <= 29 ) {
			id = i;
			return id;
		}
	}
	return id;
}
/*
p 1 x = 412
p 2 x = 384
p 3 = 362
p 4 x = 334
*/

/*
p 1 = 420;
p 2 = 370
p 3 = 330
p 4 y = 270
*/