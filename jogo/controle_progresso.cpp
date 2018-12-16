
#include"types.h"
#include"save_game.h"
#include"controle_progresso.h"

TData data_atual() {
	TData dataAtual;
	time_t timer;
	struct tm *horarioLocal;

	time(&timer); // Obtem informações de data e hora
	horarioLocal = localtime(&timer); // Converte a hora atual para a hora local
	dataAtual.dia = horarioLocal->tm_mday;
	dataAtual.mes = horarioLocal->tm_mon + 1;
	dataAtual.ano = horarioLocal->tm_year + 1900;
	dataAtual.hora = horarioLocal->tm_hour;
	dataAtual.minuto = horarioLocal->tm_min;
	dataAtual.segundo = horarioLocal->tm_sec;
	return dataAtual;
}

void progresso_energia(Personagem *p, TData ultima_data) {
	TData dataAtual;
	int progresso = 0;
	bool flag = true;
	float last = ultima_data.minuto;
	dataAtual = data_atual();

	while (progresso < 120 && flag == true) {
		flag = false;
		if (ultima_data.ano < dataAtual.ano) {
			dataAtual.mes = dataAtual.mes + 12;
			ultima_data.ano++;
			flag = true;
		}
		if (ultima_data.mes < dataAtual.mes) {
			dataAtual.dia = dataAtual.dia + 30;
			ultima_data.mes++;
			flag = true;
		}
		if (ultima_data.dia < dataAtual.dia) {
			dataAtual.hora = dataAtual.hora + 24;
			ultima_data.dia++;
			flag = true;
		}
		if (ultima_data.hora < dataAtual.hora) {
			dataAtual.minuto = dataAtual.minuto + 60;
			ultima_data.hora++;
			flag = true;
		}
		if (ultima_data.minuto <= dataAtual.minuto) {
			ultima_data.minuto++;
			flag = true;
		}
		progresso = ultima_data.minuto - last;
	}

	progresso = progresso * 0.84;
	if (p->status == DORMINDO) {
		p->progresso.porcentagem_energia += progresso;
	}
	else {
		p->progresso.porcentagem_energia -= progresso;
	}
	if (p->progresso.porcentagem_energia < 0) {
		p->progresso.porcentagem_energia = 0;
	}
	if (p->progresso.porcentagem_energia > 100) {
		p->progresso.porcentagem_energia = 100;
	}
}

void progresso_fome(Personagem *p, TData ultima_data) {
	TData dataAtual;
	int progresso = 0;
	bool flag = true;
	float last = ultima_data.minuto;

	dataAtual = data_atual();
	while (progresso < 60 && flag == true) {
		flag = false;
		if (ultima_data.ano < dataAtual.ano) {
			dataAtual.mes = dataAtual.mes + 12;
			ultima_data.ano++;
			flag = true;
		}
		if (ultima_data.mes < dataAtual.mes) {
			dataAtual.dia = dataAtual.dia + 30;
			ultima_data.mes++;
			flag = true;
		}
		if (ultima_data.dia < dataAtual.dia) {
			dataAtual.hora = dataAtual.hora + 24;
			ultima_data.dia++;
			flag = true;
		}
		if (ultima_data.hora < dataAtual.hora) {
			dataAtual.minuto = dataAtual.minuto + 60;
			ultima_data.hora++;
			flag = true;
		}
		if (ultima_data.minuto <= dataAtual.minuto) {
			ultima_data.minuto++;
			flag = true;
		}
		progresso = ultima_data.minuto - last;
	}
	progresso = progresso * 1.67;
	p->progresso.porcentagem_fome -= progresso;
	if (p->progresso.porcentagem_fome < 0) {
		p->progresso.porcentagem_fome = 0;
	}
	
}

void progresso_limpeza(Personagem *p, TData ultima_data) {
	TData dataAtual;
	int progresso = 0;
	bool flag = true;
	float last = ultima_data.minuto;
	dataAtual = data_atual();

	while (progresso < 120 && flag == true) {
		flag = false;
		if (ultima_data.ano < dataAtual.ano) {
			dataAtual.mes = dataAtual.mes + 12;
			ultima_data.ano++;
			flag = true;
		}
		if (ultima_data.mes < dataAtual.mes) {
			dataAtual.dia = dataAtual.dia + 30;
			ultima_data.mes++;
			flag = true;
		}
		if (ultima_data.dia < dataAtual.dia) {
			dataAtual.hora = dataAtual.hora + 24;
			ultima_data.dia++;
			flag = true;
		}
		if (ultima_data.hora < dataAtual.hora) {
			dataAtual.minuto = dataAtual.minuto + 60;
			ultima_data.hora++;
			flag = true;
		}
		if (ultima_data.minuto <= dataAtual.minuto) {
			ultima_data.minuto++;
			flag = true;
		}
		progresso = ultima_data.minuto - last;
	}
	progresso = progresso * 0.84;
	p->progresso.porcentagem_limpeza -= progresso;
	if (p->progresso.porcentagem_limpeza < 0) {
		p->progresso.porcentagem_limpeza = 0;
	}
}

void progresso_saude(Personagem *p, TData ultima_data) {
	TData dataAtual;
	float progresso = 0;
	bool flag = true;
	float last = ultima_data.minuto;
	dataAtual = data_atual();
	
	while (progresso < 1440 && flag == true) {
		flag = false;
		if (ultima_data.ano < dataAtual.ano) {
			dataAtual.mes = dataAtual.mes + 12;
			ultima_data.ano++;
			flag = true;
		}
		if (ultima_data.mes < dataAtual.mes) {
			dataAtual.dia = dataAtual.dia + 30;
			ultima_data.mes++;
			flag = true;
		}
		if (ultima_data.dia < dataAtual.dia) {
			dataAtual.hora = dataAtual.hora + 24;
			ultima_data.dia++;
			flag = true;
		}
		if (ultima_data.hora < dataAtual.hora) {
			dataAtual.minuto = dataAtual.minuto + 60;
			ultima_data.hora++;
			flag = true;
		}
		if (ultima_data.minuto <= dataAtual.minuto) {
			ultima_data.minuto++;
			flag = true;
		}
		progresso = ultima_data.minuto - last;
	}
	progresso = progresso * 0.07;
	p->progresso.porcentagem_saude -= progresso;
	if (p->progresso.porcentagem_saude < 0) {
		p->progresso.porcentagem_saude = 0;
	}
}
