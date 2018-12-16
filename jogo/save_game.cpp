#include"types.h"
#include"save_game.h"
#include"controle_progresso.h"

ALLEGRO_FILE *criar_arquivo(char nome_arq[]) { 
	ALLEGRO_FILE *arq;
	Personagem personagem;
	Alimento alimento;
	Cenario cenario;
	TData data;
	Aparencia aparencia;
	

	arq = al_fopen(nome_arq, "r+b");
	if (arq == NULL) {
		arq = al_fopen(nome_arq, "w+b");
		if(strcmp(nome_arq, "save_personagem.data") == 0) {
			personagem.file_flag = true;
			save_file_personagem(personagem, arq);
		}
		else if(strcmp(nome_arq, "save_background.data") == 0) {
			cenario.file_flag = true;
			save_file_cenario(cenario, arq);
		}
		else if (strcmp(nome_arq, "save_data.data") == 0) {
			data.file_flag = true;
			save_file_data(data, arq);
		}
		else if (strcmp(nome_arq, "save_alimento.data") == 0) {
		    alimento.file_flag = true;
			save_file_alimento(alimento, arq);
		}
		else if (strcmp(nome_arq, "save_aparencia.data") == 0) {
			aparencia.file_flag = true;
			save_file_aparencia(aparencia, arq); 
		}
	}
	return arq;
}


Personagem load_file_personagem(ALLEGRO_FILE *arq) {
	Personagem personagem;
	
	al_fseek(arq, 0, 0);
	al_fread(arq, &personagem, sizeof(Personagem));
	if (al_ferror(arq) == true) {//validar
		cout << "error de leitura" << endl;
	}
	return personagem;
}

void save_file_personagem(Personagem personagem, ALLEGRO_FILE *arq) {

	al_fseek(arq, 0, 0);
	al_fwrite(arq, &personagem, sizeof(Personagem));
	if (al_ferror(arq) == true) {
		cout << "error de gravacao" << endl;
	}
	else {
		al_fflush(arq);
		printf("salvo com sucesso");
	}
}

Cenario load_file_cenario(ALLEGRO_FILE *arq) {
	Cenario cenario;

	al_fseek(arq, 0, 0);
	al_fread(arq, &cenario, sizeof(Cenario));
	if (al_ferror(arq) == true) {//validar
		cout << "error de leitura" << endl;
	}
	return cenario;
}

void save_file_cenario(Cenario cenario, ALLEGRO_FILE *arq) {

	al_fseek(arq, 0, 0);
	al_fwrite(arq, &cenario, sizeof(Cenario));
	if (al_ferror(arq) == true) {
		cout << "error de gravacao" << endl;
	}
	else {
		al_fflush(arq);
		printf("salvo com sucesso");
	}
}


TData load_file_data(ALLEGRO_FILE *arq) {
	TData ultima_data;

	al_fseek(arq, 0, 0);
	al_fread(arq, &ultima_data, sizeof(TData));
	if (al_ferror(arq) == true) {//validar
		cout << "error de leitura" << endl;
	}
	return ultima_data;
}

void save_file_data(TData ultima_data, ALLEGRO_FILE *arq) {

	al_fseek(arq, 0, 0);
	al_fwrite(arq, &ultima_data, sizeof(TData));
	if (al_ferror(arq) == true) {
		cout << "error de gravacao" << endl;
	}
	else {
		al_fflush(arq);
		printf("salvo com sucesso");
	}
}


void load_file_alimento_vet(ALLEGRO_FILE *arq, Alimento alimento[]) {
	int i = 1;

	al_fseek(arq, 0, 0);
	while (1) {
		if (!al_feof(arq)) {
			al_fread(arq, &alimento[i], sizeof(Alimento));
			if (al_ferror(arq) == true) {//validar
				cout << "error de leitura" << endl;
			}
			i++;
		}
		else {
			break;
		}
	}
	/*cout << "--"<<alimento[0].id_alimento << endl;
	cout << "--" << alimento[1].id_alimento << endl;
	cout << "--" << alimento[2].id_alimento << endl;
	cout << "--" << alimento[3].id_alimento << endl;
	cout << "--" << alimento[3].id_alimento << endl;
	cout << "--" << alimento[5].id_alimento << endl;*/
	
	//system("pause");
	//return alimento[1];
}

Alimento load_file_alimento(ALLEGRO_FILE *arq) {
	Alimento alimento;

	al_fseek(arq, 0, 0);
	while (1) {
		if (!al_feof(arq)) {
			al_fread(arq, &alimento, sizeof(Alimento));
			if (al_ferror(arq) == true) {//validar
				cout << "error de leitura" << endl;
			}
		}
		else {
			break;
		}
	}
	return alimento;
}

void save_file_alimento(Alimento alimento, ALLEGRO_FILE *arq) {

	al_fseek(arq, 0, 2);
	al_fwrite(arq, &alimento, sizeof(Alimento));
	if (al_ferror(arq) == true) {
		cout << "error de gravacao" << endl;
	}
	else {
		al_fflush(arq);
		printf("salvo com sucesso");
	}
}

void change_file_alimento(ALLEGRO_FILE *arq, Alimento a) {
	FILE *arq1;
	Alimento alimento;
	bool flag = false;
	int d = -1;
	int teste = 0;


	arq1 = fopen("save_alimento.data", "r+b");
	if (arq1 == NULL) {
		arq1 = fopen("save_alimento.data", "w+b");
	}
	fseek(arq1, 0, 0);
	while (1) {
		teste = fread(&alimento, sizeof(Alimento), 1, arq1);
		if (teste != 1) {
			if (!feof(arq1)) {
			}
			break;
		}
		else if (feof(arq1)) {
			break;
		}
		else {
			if (alimento.id_alimento == a.id_alimento) {
				flag = true;
				alimento.quantidade += a.quantidade;
				if (alimento.quantidade < 0)
					alimento.quantidade = 0;
				fseek(arq1, d * sizeof(Alimento), 1);
				teste = fwrite(&alimento, sizeof(Alimento), 1, arq1);
				fseek(arq1, sizeof(Alimento), 1);
				if (teste != 1) {
					printf("Erro na gravacao!\n");
				}
			}
		}
	}
	fclose(arq1);
	if (flag == false) {
		save_file_alimento(a, arq);
	}









	
	//al_fclose(arq);
	/*al_fseek(arq, 0, 0);
	while (1) {
		teste = al_fread(arq, &alimento, sizeof(Alimento));
		if (al_feof(arq)) {
			cout << "llssssaf" << endl;
			break;
		}
		if (alimento.id_alimento == a.id_alimento) {
			cout <<"anterior:  "<<alimento.quantidade << endl;;
			alimento.quantidade++;
			cout <<"depois: " <<alimento.quantidade << endl;
			cout << alimento.id_alimento;
			cout << alimento.nivel_necessario;
			//alimento.status_file = false;
			//al_fungetc(arq, d* sizeof(Alimento));
			al_fseek(arq, d * sizeof(Alimento), 1);
			al_fwrite(arq, &alimento, sizeof(Alimento));
			al_fseek(arq, sizeof(Alimento), 1);
			//save_file_alimento(a, arq);
			//al_fseek(arq, d * sizeof(Alimento), 1);
		//	
			al_fflush(arq);
			
			//al_fflush(arq);
			cout << "gravou por cima";
			flag = true;
			//break;
		}
		


	}
	


	/* = 
	//al_fseek(arq, 0, 0);


	/*
	while (1){
		if (al_feof(arq)) {
			return;
		}
		cout << "ssssss" << endl;
			//teste = fread(&consulta, sizeof(TConsulta), 1, arqConsulta);
		
			al_fread(arq, &alimento, sizeof(Alimento));
			if (al_ferror(arq) == true) {//validar
				cout << "error de leitura" << endl;
			}
			if (alimento.id_alimento == a.id_alimento) {
				alimento.quantidade += a.quantidade;
				al_fseek(arq, d * sizeof(Alimento), 1);
				al_fwrite(arq, &alimento, sizeof(Alimento));
				//al_fseek(arq, 0, 2);
				flag = true;
			}
			teste++;
		}
		*/
	
	
}


void load_file_aparencia(ALLEGRO_FILE *arq, Aparencia *aparencia) {
	int i = 0;

	al_fseek(arq, 0, 0);
	while (1) {
		if (!al_feof(arq)) {
			al_fread(arq, &aparencia[i], sizeof(Aparencia));
			if (al_ferror(arq) == true) {//validar
				cout << "error de leitura" << endl;
			}
			i++;
		}
		else {
			break;
		}
	}

}

void save_file_aparencia(Aparencia aparencia, ALLEGRO_FILE *arq) {

	al_fseek(arq, 0, 2);
	al_fwrite(arq, &aparencia, sizeof(Aparencia));
	if (al_ferror(arq) == true) {
		cout << "error de gravacao" << endl;
	}
	else {
		al_fflush(arq);
		printf("salvo com sucesso");
	}
}
