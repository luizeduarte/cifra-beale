#include <stdio.h>
#include <stdlib.h>
#include "abre_arquivos.h"

FILE* abre_leitura(char* arquivo){
	if (!arquivo){
		fprintf(stderr, "argumentos insuficientes!\n");
		exit(1);
	}
	
	FILE* f_arquivo = fopen(arquivo, "r");
	if (!f_arquivo){
		fprintf(stderr, "erro ao abrir os arquivos\n");
		exit(1);
	} 
		
	return f_arquivo;	
}

FILE* abre_escrita(char* arquivo){
	if (!arquivo){
		fprintf(stderr, "argumentos insuficientes!\n");
		exit(1);
	}
	
	FILE* f_arquivo = fopen(arquivo, "w");
	if (!f_arquivo){
		fprintf(stderr, "erro ao abrir os arquivos\n");
		exit(1);
	} 
		
	return f_arquivo;	
}