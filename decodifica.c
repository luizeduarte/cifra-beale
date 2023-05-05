#include <stdio.h>
#include "decodifica.h"
#include "manipula_lista.h"

void decodifica(struct nodo_caractere* chars_head, FILE* f_mensagem_codificada, FILE* f_mensagem_decodificada){
	int chave, caractere;

	while (fscanf(f_mensagem_codificada, "%d", &chave) != EOF){	//enquanto nao chegar ao fim do arquivo,le um numero
		if (chave == ESPACO)
			fprintf(f_mensagem_decodificada, " ");

		else if (chave == QUEBRA_LINHA)
			fprintf(f_mensagem_decodificada, "\n");

		else if (chave == CARACTERE_FALTANDO)
			fprintf(f_mensagem_decodificada, "?");

		else { 
			caractere = seleciona_caractere(chars_head, chave);	//seleciona o caractere que corresponde a chave
			fprintf(f_mensagem_decodificada, "%c", caractere);
		}
	}
}