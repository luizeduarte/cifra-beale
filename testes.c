#include  <stdio.h>
#include <stdlib.h>
#include  <string.h>
#include <stdarg.h>

//struct que compoe a fila de chaves
struct chave{
	int num_chave;
	struct chave *prox;
};

//struct que compoe a fila de caracteres presentes no livro cifra
struct caractere{
	char letra;	//guarda o caractere que possui a lista de chaves
	int tamanho; 	//guarda o tamanho da lista de chaves do caractere
	struct chave *lista_chaves;	//ponteiro para a lista de chaves do caractere
	struct caractere *prox;
};

struct chave* aloca_chave(int* num_chave){
	struct chave* novo = malloc(sizeof(struct chave));
	novo->num_chave = *num_chave;
	novo->prox = NULL;
	return novo;
}

struct caractere* aloca_caracter(char letra){
	struct caractere* novo = malloc(sizeof(struct caractere));
	novo->letra = letra;
	novo->tamanho = 0;
	novo->lista_chaves = NULL;
	novo->prox = NULL;
	return novo;
}

void adiciona_chave(int* num_chave, struct caractere* aux){
	struct chave* novo = aloca_chave(num_chave);
	(*num_chave)++;
	aux->tamanho++;

	novo->prox = aux->lista_chaves;		//adiciona a chave no comeco
	aux->lista_chaves = novo;

}

struct caractere* insere_lista(struct caractere* lista_chars, int* num_chave, char letra){
	struct caractere* aux = lista_chars;

	if (lista_chars == NULL){	//lista vazia, o novo elemento sera o primeiro
		lista_chars = aloca_caracter(letra);
		adiciona_chave(num_chave, lista_chars);

	} else if (lista_chars->letra >= letra){	//lista com um elemento, verifica se o novo deve ir antes

		if (lista_chars->letra == letra)	//o caractere eh o unico na lista, basta adicionar a chave
			adiciona_chave(num_chave, lista_chars);

		else {		//o caractere deve ser inserido como primeiro da lista
				struct caractere* novo = aloca_caracter(letra);

				novo->prox = lista_chars;
				lista_chars = novo;
				adiciona_chave(num_chave, lista_chars);
		}
	} else {
		while ((aux->prox != NULL) && (aux->prox->letra < letra))	//procura onde o novo elemento deve ser inseridoreturn;
			aux = aux->prox;

		if ((aux->prox != NULL) && (aux->prox->letra == letra))		//o caractere ja esta na lista, basta adicionar a chave
			adiciona_chave(num_chave, aux->prox);
		else {		//o caractere deve ser adicionado
			struct caractere* novo = aloca_caracter(letra);

			novo->prox = aux->prox;
			aux->prox = novo;
			adiciona_chave(num_chave, novo);
		}

	}
	return lista_chars;
}

void imprime_lista(struct caractere* lista_chars){
	struct caractere* aux = lista_chars;
	struct chave* aux_chave;

	while (aux != NULL){
		printf("%c: ", aux->letra);
		aux_chave = aux->lista_chaves;
		while (aux_chave != NULL){
			printf("%d ", aux_chave->num_chave);
			aux_chave = aux_chave->prox;
		}
		printf("\n");
		aux = aux->prox;
	}
}

int main(){
	struct caractere* lista_chars = NULL;
	int num_chave = 0;

	lista_chars = insere_lista(lista_chars, &num_chave, 'a');
	lista_chars = insere_lista(lista_chars, &num_chave, 'b');
	lista_chars = insere_lista(lista_chars, &num_chave, 'c');
	lista_chars = insere_lista(lista_chars, &num_chave, 'a');
	lista_chars = insere_lista(lista_chars, &num_chave, 'b');

	imprime_lista(lista_chars);
}
