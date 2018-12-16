using namespace std;
#pragma warning(disable:4996)


#ifndef ESTRUTURAS
#define ESTRUTURAS

#include <allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_primitives.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_ttf.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_memfile.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include<iostream>
#include<cstdio>
#include <cstdlib>
#include<cmath>
#include <ctime>
#include<time.h>
#include<string.h>

typedef struct noQueue {
	int info;
	struct noQueue *prox;
}TNoQueue;

typedef struct descritor {
	int qtd;
	TNoQueue *inicio, *fim;
}Tdescritor;

typedef Tdescritor *Queue;

enum PersonagemStatus {
	PARADO, COM_SONO, COM_FOME, SUJO, COMENDO, DORMINDO, SEM_INTERAGIR, DOENTE, GORDO
};

enum AparenciaPersonagem {
	BRANCO, AZUL, VERMELHO, AMARELO, VERDE, ROSA, ROXO, PRETO, LARANJA, ACESSORIO_1, ACESSORIO_2, ACESSORIO_3, ACESSORIO_4, ACESSORIO_5, ACESSORIO_6, ACESSORIO_7, ACESSORIO_8,
	ACESSORIO_9, ACESSORIO_10
};

enum TelaAvisos {
	AGUA, NIVEL
};

enum Background {
	SALA, COZINHA, BANHEIRO, QUARTO_ACESO, QUARTO_APAGADO, SALA_DE_JOGOS, LOJA_MENU, LOJA_COMIDA_1, LOJA_COMIDA_2, LOJA_PERSONAGEM_1, LOJA_PERSONAGEM_2, LOJA_PERSONAGEM_3, MINI_JOGOS, GELADEIRA, TELA_STATUS,
	GUARDA_ROUPA_1, GUARDA_ROUPA_2
};
enum Interagir {
	PERSONAGEM, COMIDA, BANHO
};

enum ObjetosCenario {
	MESA, BANANA, SABONETE
};

typedef struct Nome_sprite {
	char sprite_cor[40];
	char sprite_parado[40];
	char sprite_com_sono[40];
	char sprite_dormindo[40];
	char sprite_comendo[40];
	char sprite_sujo[40];
	char sprite_doente[40];
}Nome_sprite;

typedef struct StatusAtributos {
	float progresso_nivel;
	float porcentagem_fome;
	float porcentagem_saude;
	float porcentagem_limpeza;
	float porcentgem_peso;
	float porcentagem_energia;
}StatusAtributos;

typedef struct Aparencia {
	bool file_flag;
	bool desenhar_acessorio;
	int nivel_necessario;
	int id;
	float preco;

}Aparencia;


typedef struct Personagem {
	bool file_flag;
	bool interagir;
	char nome[50];
	int nivel;
	int cor;
	int tamanho;
	int ultimo_status;
	int status;
	float dinheiro;
	float pos_sprite_x;
	float pos_sprite_y;
	float regiao_x_folha;
	float regiao_y_folha;
	float largura_sprite;
	float altura_sprite;
	Aparencia acessorio;
	StatusAtributos progresso;
}Personagem;

typedef struct Decoracao {
	int nivel_necessario;
	int id_decoracao;
	float preco_decoracao;
}Decoracao;

typedef struct Cenario {
	bool file_flag;
	int cenario_atual;
	int ultimo_cenario;
	Decoracao decoracao;
}Cenario;

typedef struct Alimento {
	bool file_flag;
	int nivel_necessario;
	int id_alimento;
	int quantidade;
	float preco;
	float progresso_comida;
}Alimento;



typedef struct ALIMENTO_DESENHO {
	bool file_flag;
	Alimento alimento;
	float pos_sprite_x;
	float pos_sprite_y;
	float regiao_x_folha;
	float regiao_y_folha;
	float largura_sprite;
	float altura_sprite;
}ALIMENTO_DESENHO;




typedef struct INTERAGIR_COZINHA {
	bool movimentar_alimento;
	bool comer;
	bool mastigar;
	bool abrir_geladeira;
	bool alimento_mesa;
	int mastigando;
}INTERAGIR_ALIMENTO;

typedef struct INTERAGIR_BANHEIRO {
	bool movimentar_sabonete;
	bool ensaboar;
	bool ligar_chuveiro;
	int qtd_bolhas;//??
	int cont_frames;
	int x;
	int y;
}INTERAGIR_BANHEIRO;

typedef struct Data{
	bool file_flag;
	int dia;
	int mes;
	int ano;
	int hora;
	int minuto;
	int segundo;
}TData;

typedef struct Mini_jogo {
	int id;
	bool display_close;
	bool error;
	float dinheiro_ganho;
	float progresso;

}Mini_jogo;

typedef struct Mini_jogo_mj1 {
	int inicio_qtd_sequencia;
	int qtd_sequencia;
	int numero_aleatorio;
	int resp;
	bool fim_sequencia;
	bool responder;
	bool draw_resposta;
}Mini_jogo_mj1;

typedef struct Limite_clic {
	int lim_x_min;
	int lim_x_max;
	int limi_y_min;
	int limi_y_max;
}Limite_clic;


#endif

