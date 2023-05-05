#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "manipula_lista.h"

//FUNCOES QUE MANIPULAM A LISTA
struct nodo_chave* aloca_chave(int num_chave){
	struct nodo_chave* novo = malloc(sizeof(struct nodo_chave));
	novo->num_chave = num_chave;
	novo->prox = NULL;
	return novo;
}

struct nodo_caractere* aloca_caractere(char caractere){
	struct nodo_caractere* novo = malloc(sizeof(struct nodo_caractere));
	novo->caractere = caractere;
	novo->tamanho = 0;
	novo->chaves_head = NULL;
	novo->prox = NULL;
	return novo;
}

void adiciona_chave(int num_chave, struct nodo_caractere* aux){
	struct nodo_chave* novo = aloca_chave(num_chave);
	aux->tamanho++;		//cada elemento da lista de chars guarda o tamanho da lista de chaves ligada a ela

	if (!aux->chaves_head)	//lista vazia, o novo elemento sera o primeiro
		aux->chaves_head = novo;

	else if (aux->chaves_head->num_chave < num_chave){	//verifica se o novo deve ir como primeiro
		novo->prox = aux->chaves_head;
		aux->chaves_head = novo;

	} else {	//novo deve ir no fim
		struct nodo_chave* aux2 = aux->chaves_head;

		while (aux2->prox)	//procura o ultimo elemento da lista
			aux2 = aux2->prox;

		aux2->prox = novo;	//adiciona a chave no final
	}
}

struct nodo_caractere* insere_lista(struct nodo_caractere* chars_head, int num_chave, char caractere){
	//verifica se o caractere ja esta na lista e adiciona a chave 

	if (!chars_head){	//lista vazia, o novo elemento sera o primeiro
		chars_head = aloca_caractere(caractere);
		adiciona_chave(num_chave, chars_head);

	} else if (chars_head->caractere >= caractere){	//lista com um elemento, verifica se o novo deve ir antes

		if (chars_head->caractere == caractere)	//o caractere eh o unico na lista, basta adicionar a chave
			adiciona_chave(num_chave, chars_head);

		else {		//o caractere deve ser inserido como primeiro da lista
			struct nodo_caractere* novo = aloca_caractere(caractere);
			novo->prox = chars_head;
			chars_head = novo;

			adiciona_chave(num_chave, chars_head);
		}
	} else {
		struct nodo_caractere* aux = chars_head;

		while ((aux->prox) && (aux->prox->caractere < caractere))	//procura onde o novo elemento deve ser inserido
			aux = aux->prox;

		if ((aux->prox) && (aux->prox->caractere == caractere))		//o caractere ja esta na lista, basta adicionar a chave
			adiciona_chave(num_chave, aux->prox);
		else {		//o caractere deve ser adicionado
			struct nodo_caractere* novo = aloca_caractere(caractere);
			novo->prox = aux->prox;
			aux->prox = novo;

			adiciona_chave(num_chave, aux->prox);
		}

	}
	return chars_head;
}

int seleciona_chaves(struct nodo_caractere* chars_head, char caractere){
	//seleciona uma chave que corresponde a caractere do texto
	struct nodo_caractere* aux1 = chars_head;
	struct nodo_chave* aux2;

	while (aux1){
		if (aux1->caractere == caractere){	//encontra o caractere
			aux2 = aux1->chaves_head;
			int pos = rand() % aux1->tamanho;	//aleatoriza a posicao da chave

			for (int i = 0; i < pos; i++)
				aux2 = aux2->prox;	//anda ate a posicao aleatoria

			return aux2->num_chave;		//retorna a chave
		}
		aux1 = aux1->prox;
	}
	//se nao encontrou o caractere, codifica como -3
	return CARACTERE_FALTANDO;
}

char seleciona_caractere(struct nodo_caractere* chars_head, int chave){
	//seleciona a caractere que corresponde a chave
	struct nodo_caractere* aux1 = chars_head;
	struct nodo_chave* aux2;

	while ((aux1)){		//procura na lista de caracteres
		aux2 = aux1->chaves_head;

		while ((aux2)){		//procura na lista de chaves
			if (aux2->num_chave == chave){
				return aux1->caractere;
			}

			aux2 = aux2->prox;
		}

		aux1 = aux1->prox;
	}
	return '?';
}

void desaloca_lista(struct nodo_caractere* chars_head){
	struct nodo_caractere* aux = chars_head;

	while (aux){
		while (aux->chaves_head){
			struct nodo_chave* temp = aux->chaves_head->prox;
			free(aux->chaves_head);
			aux->chaves_head = temp;
		}

		struct nodo_caractere* temp = aux->prox;
		free(aux);
		aux = temp;
	}
}

//FUNCOES DE TRANSFORMAR ARQUIVOS EM LISTAS
struct nodo_caractere* gera_lista_livro(FILE* f_livro){
	//gera a lista de chaves a partir do livro cifra
	struct nodo_caractere* chars_head = NULL;
	int num_chave = 0;
	char caractere, palavra[MAX_STRING];

	while (fscanf(f_livro, "%s", palavra) != EOF){
		caractere = tolower(palavra[0]);

		if ((caractere >= 33) && (caractere <= 126)){	//verifica se eh um caractere valido
			chars_head = insere_lista(chars_head, num_chave, caractere);
			num_chave++;
		}
	}
	return chars_head;
}

struct nodo_caractere* gera_lista_arqchaves(FILE* f_chaves){
	//gera listas a partir do arq de chaves
	char palavra[MAX_STRING];
	struct nodo_caractere* chars_head = NULL;
	struct nodo_caractere* aux1 = NULL;

	if (fscanf(f_chaves, "%s", palavra) != EOF){	//insere o primeiro 
		chars_head = aloca_caractere(palavra[0]);
		aux1 = chars_head;
	}

	while (fscanf(f_chaves, "%s", palavra) != EOF){
		if (palavra[1] == ':'){		//leu um char
			aux1->prox = aloca_caractere(palavra[0]);
			aux1 = aux1->prox;
		}else	//leu uma chave
			adiciona_chave(atoi(palavra), aux1);
	}
	return chars_head;
}