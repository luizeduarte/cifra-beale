#include  <stdio.h>
#include  <string.h>

int seleciona_chaves(struct caracteres* lista_chars, char letra){
	//seleciona uma chave que corresponde a letra do texto
	struct caracteres* aux = lista_chars;

	while (aux != NULL){
		if (aux->letra == letra)	//encontra o caractere
			return aux->lista_chaves->num_chave; //aleatorizar!!!
		aux = aux->prox;
	}
}

void codifica(struct caracteres* lista_chars){

	fgets (line, LINESIZE, );
	while (! feof ()){		//enquanto o arquivo ainda nao terminou
		if (*texto == ' ')
			printf("-1 ");	//mudar printf
		else 
			printf("%d ", seleciona_chaves(lista_chars, *texto)); //seleciona uma chave que corresponde a letra do texto
	

		fgets (line, LINESIZE, );
		texto++;	//ver se da segfault
	}
}

void decodifica(struct caracteres* lista_chars, int num_chaves){

	fgets (line, LINESIZE, );	//tenta ler a linha, depois testa se esta no final do arquivo
	while (! feof ()){
		if ( == -1) 	//se for -1, eh um espaco
			printf(" ");
		else { 
			//procura a letra que corresponde a chave
			struct caracteres* aux1 = lista_chars;
			struct chaves* aux2;

			while (aux1 != NULL){
				aux2 = aux1->lista_chaves;

				while (aux2 != NULL){
					if (aux2->num_chave == num_chaves) //fica procurando msm dps de achar flag
						printf("%c", aux1->letra);
					aux2 = aux2->prox;
				}
				aux1 = aux->prox;
			}
		}

	fgets (line, LINESIZE, );
	}
}

//struct que compoe a fila de chaves
struct chave{
	int num_chave;
	struct chave *prox;
};

//struct que compoe a fila de caracteres presentes no livro cifra
struct caracteres{
	char letra;
	struct chave *lista_chaves; //ponteiro para a lista de chaves do caractere
	struct caracteres *prox;
};

struct chave* aloca_chave(int num_chave){
	struct chave* novo = malloc(sizeof(struct chave));
	novo->num_chave = num_chave;
	novo->prox = NULL;
	return novo;
}

struct caracteres* aloca_caracter(char letra){
	struct caracteres* novo = malloc(sizeof(struct caracteres));
	novo->letra = letra;
	novo->lista_chaves = NULL;
	novo->prox = NULL;
	return novo;
}

void adiciona_chave(int num_chave, struct caracteres* aux){
	struct chave* novo = aloca_chave(num_chave);
	novo->prox = aux->lista_chaves;
	aux->lista_chaves = novo;
}

void insere_lista(struct chave* lista_chars, int num_chave, char letra){	//ordenaaaaar
	//primeiro, checa se a letra esta na fila dos caracteres
	struct caracteres* aux = lista_chars;

	while (aux != NULL){
		if (aux->letra == letra){
			//se a letra ja esta na fila, adiciona a chave na lista de chaves
			adiciona_chave(num_chave, aux);
			return;
		}
		aux = aux->prox;
	}

	//se a letra nao esta na fila, adiciona a letra na fila
	aux->prox = aloca_caracter(letra);

	//agora adiciona a chave na lista de chaves
	adiciona_chave(num_chave, aux->prox);
}

void desaloca_fila(struct chave* lista_chars){
	struct caracteres* aux = lista_chars;
	while (aux != NULL){
		while (aux->lista_chaves != NULL){
			struct chave* temp = aux->lista_chaves->prox;
			free(aux->lista_chaves);
			aux->lista_chaves = temp;
		}

		struct caracteres* temp = aux->prox;
		free(aux);
		aux = temp;
	}
}

void gera_chaves(struct chave* lista_chars){
	int num_chave = 0;
	char letra, string[1024]; 				//VER SE TEM COMO MUDAR

	fgets (line, LINESIZE, );				//tenta ler a linha, depois testa se esta no final do arquivo
	while (! feof ()){

		fscanf(/*livro cifra*/, "%s", string);		//le uma palavra
		letra = string[0];				//pega a primeira letra da palavra
		insere_lista(lista_chars, num_chave, letra);
		num_chave++;

		fgets (line, LINESIZE, );
	}
}
