#ifndef MANIPULA_CONTEUDO_H
#define MANIPULA_CONTEUDO_H
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING 1024
#include "manipula_dir.h"

struct conteudo{
	unsigned int num_arq;
	long long int tam_conteudo, diretorio_pos;
};

struct conteudo* conteudo(FILE* archive);

void copia_texto(FILE* arq_le, FILE* arq_escreve, long long int tam_texto);

struct conteudo* add_info_conteudo(FILE* archive, long long int tam_arquivo);

struct conteudo* sub_info_conteudos(FILE* archive, long long int tam_arquivo);

void move_conteudo(FILE* archive, struct diretorio* diretorio, long long int diretorio_pos);

#endif