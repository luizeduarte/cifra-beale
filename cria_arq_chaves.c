#include <stdio.h>
#include "cria_arq_chaves.h"

void cria_arq_chaves(struct nodo_caractere* chars_head, FILE* f_chaves){
	//cria um arquivo com as listas de chaves
	
	while (chars_head){
		fprintf(f_chaves, "%c: ", chars_head->caractere);

		struct nodo_chave* aux = chars_head->chaves_head;
		while (aux){
			fprintf(f_chaves, "%d ", aux->num_chave);
			aux = aux->prox;
		}

		fprintf(f_chaves, "\n");
		chars_head = chars_head->prox;
	}
}