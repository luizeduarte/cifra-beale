#include "abre_arquivos.h"

FILE* cria_arquivo(char* arquivo){
	/*a funcao abre o arquivo passado como argumento para 
	leitura e escrita se ja existir, ou para escrita caso contrario*/
	FILE* p_arquivo = fopen(arquivo, "r+b");

	if (!p_arquivo)
		p_arquivo = fopen(arquivo, "wb");

	return p_arquivo;
}

FILE* abre_arquivo(char* arquivo){
	/*a funcao abre o arquivo passado como argumento para 
	leitura e escrita se ja existir, se nao, avisa e fecha o programa*/
	FILE* p_arquivo = fopen(arquivo, "r+b");

	if (!p_arquivo)
		fprintf(stderr, "Arquivo %s nao existe\n", arquivo);

	return p_arquivo;
}