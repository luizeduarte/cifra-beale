#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING 1024

struct diretorio{
	int tam_nome;
	char* nome;
	long long int tamanho;
	long long int posicao;
	uid_t uid;
	mode_t permissoes;
	time_t ultima_modificacao;
};

struct conteudo{
	unsigned int num_arq;
	long long int tam_conteudo, diretorio_pos;
};

struct diretorio** le_diretorio(FILE* archive);

struct diretorio* att_diretorio(struct diretorio* v_diretorio[], struct conteudo info_conteudo, struct stat info_arquivo, char* nome_arquivo);

void imprime_diretorio(FILE* archive, struct diretorio* v_diretorio[], struct conteudo info_conteudo);

int id_arquivo(FILE *archive, char *nome_arquivo, struct diretorio *v_diretorio[]);