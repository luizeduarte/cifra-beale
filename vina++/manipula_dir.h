#ifndef MANIPULA_DIR_H
#define MANIPULA_DIR_H
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "manipula_conteudo.h"
#define MAX_STRING 1024

//add ordem no arquivo
struct diretorio{
	int tam_nome;
	char* nome;
	long long int tamanho;
	long long int posicao;
	uid_t uid;
	mode_t permissoes;
	time_t ultima_modificacao;
};

struct diretorio** le_diretorio(FILE* archive);

struct diretorio** adiciona_diretorio(struct diretorio* v_diretorio[], struct conteudo* info_conteudo, struct stat info_arquivo, char* nome_arquivo);

void imprime_diretorio(FILE* archive, struct diretorio* v_diretorio[], int num_arq);

int id_arquivo(FILE *archive, char *nome_arquivo, struct diretorio *v_diretorio[]);

void desaloca_diretorio(FILE* archive, struct diretorio* v_diretorio[]);
#endif