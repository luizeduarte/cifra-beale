#include <stdio.h>

struct elemento{
	void* elem;
	void(*imprime_elem)(void*);
	struct elemento* prox;
};

struct fila{
	struct elemento* inicio;
	struct elemento* fim;
	int tam;
	int limite_fila;
};

struct fila* cria_fila(unsigned int limite_fila){
	struct fila* f = malloc(sizeof(struct fila));
	f->inicio = NULL;
	f->fim = NULL;
	f->tam = 0;
	f->limite_fila = limite_fila;
	return f;
}

int fila_vazia(struct fila* f){
	return (f->inicio == NULL);
}

int fila_cheia(struct fila* f){
	return (f->tam == f->limite_fila);
}

void insere(struct elemento *o, struct fila* f){
	if (fila_vazia(f)){
		f->inicio = o;
	} else {
		f->fim->prox = o;
	}

	f->fim = o;
	f->tam++;
	return;
}

void retira(struct elemento *o, struct fila* f){
	//procura o elemento 
	if (fila_vazia(f)){
		return;
	}

	o = f->inicio;
	f->inicio = f->inicio->prox;

	if (f->inicio == NULL){
		f->fim = NULL;
	}
	f->tam--;
	return;
}

void imprime(struct fila* f, void(*imprime_elem)(void*)){
	struct elemento* aux = f->inicio;

	while (aux){
		aux->imprime_elem(aux->elem);
		aux = aux->prox;
	}
	return;
}

void imprime_elem(void* elem){
	int *i = (int*) elem;
}