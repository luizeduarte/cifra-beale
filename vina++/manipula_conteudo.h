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

void copia_texto(FILE* arq_le, FILE* arq_escreve, long long int tam_texto);

struct conteudo add_info_conteudo(FILE* archive, struct stat info_arquivo);

struct conteudo sub_info_conteudos(FILE* archive, struct diretorio* v_diretorio[], int arquivo);

void move_conteudo(FILE* archive, struct diretorio* v_diretorio[], int arquivo, long long int diretorio_pos);