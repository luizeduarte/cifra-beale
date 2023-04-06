#include  <stdio.h>
#include  <string.h>
#include <stdarg.h>


//struct que compoe a fila de chaves
struct chave{
	int* num_chave;
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
	novo->num_chave = num_chave;
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

void insere_lista(struct caractere* lista_chars, int* num_chave, char letra){
	struct caractere* aux = lista_chars;

	if (lista_chars == NULL){	//lista vazia, sera o primeiro elemento
		lista_chars = aloca_caracter(letra);
		adiciona_chave(num_chave, lista_chars);

	} else if (lista_chars->prox == NULL){	//lista com um elemento

		if (lista_chars->letra == letra)	//o caractere eh o unico na lista, basta adicionar a chave
			adiciona_chave(num_chave, lista_chars);

		else if (lista_chars->letra < letra) {		//o caractere deve ser inserido como primeiro da lista
				struct caractere* novo = aloca_caracter(letra);

				novo->prox = lista_chars;
				lista_chars = novo;
				adiciona_chave(num_chave, lista_chars);

		} else {	//se nao, adiciona ele no fim

			lista_chars->prox = aloca_caracter(letra);
			adiciona_chave(num_chave, lista_chars);
		}

	} else {	//lista com mais de um elemento
			while (aux->prox != NULL){	//enquanto nao chegar no penultimo elemento da lista
		if (aux->prox->letra <= letra){		//checa se o prox caractere eh menor ou igual ao que estamos buscando
			if (aux->prox->letra == letra)		//o caractere ja esta na lista, basta adicionar a chave
				adiciona_chave(num_chave, aux);
			else {					//o caractere nao esta na lista, entao o adiciona
				adiciona_caracter(letra, lista_chars);
				adiciona_chave(num_chave, aux);
			}

			return;
		}
		aux = aux->prox;
	}

	}

	while (aux->prox != NULL){	//enquanto nao chegar no penultipo elemento da lista
		if (aux->prox->letra <= letra){		//checa se o prox caractere eh menor ou igual ao que estamos buscando
			if (aux->prox->letra == letra)		//o caractere ja esta na lista, basta adicionar a chave
				adiciona_chave(num_chave, aux);
			else {					//o caractere nao esta na lista, entao o adiciona
				struct caractere* novo = aloca_caracter(letra);]
				novo->prox = aux->prox;
				aux->prox = novo;
				
				adiciona_chave(num_chave, aux);
			}
		}
		aux = aux->prox;
	}

	//se chegou aqui, eh pq o caractere eh o ultimo da lista
	struct caractere* novo = aloca_caracter(letra);]
	novo->prox = aux->prox;
	aux->prox = novo;
	
	adiciona_chave(num_chave, aux);
}

int main(){
	struct caractere* lista_chars = NULL;
	int num_chave;

	insere_lista(lista_chars, &num_chave, 'a');
}
