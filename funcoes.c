#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

//struct que compoe a fila de chaves
struct chave{
	int num_chave;
	struct chave *prox;
};

//struct que compoe a fila de caracteres presentes no livro cifra
struct caractere{
	char letra;	//guarda o caractere que possui a lista de chaves
	int tamanho; 	//guarda o tamanho da lista de chaves do caractere
	struct chave *chaves_head;	//ponteiro para a lista de chaves do caractere
	struct caractere *prox;
};

//FUNCOES QUE MANIPULAM A LISTA
struct chave* aloca_chave(int num_chave){
	struct chave* novo = malloc(sizeof(struct chave));
	novo->num_chave = num_chave;
	novo->prox = NULL;
	return novo;
}

struct caractere* aloca_caracter(char letra){
	struct caractere* novo = malloc(sizeof(struct caractere));
	novo->letra = letra;
	novo->tamanho = 0;
	novo->chaves_head = NULL;
	novo->prox = NULL;
	return novo;
}

void adiciona_chave(int num_chave, struct caractere* aux){
	struct chave* novo = aloca_chave(num_chave);
	aux->tamanho++;
	novo->prox = aux->chaves_head;		//adiciona a chave no comeco
	aux->chaves_head = novo;
}

struct caractere* insere_lista(struct caractere* chars_head, int num_chave, char letra){

	if (chars_head == NULL){	//lista vazia, o novo elemento sera o primeiro
		chars_head = aloca_caracter(letra);
		adiciona_chave(num_chave, chars_head);

	} else if (chars_head->letra >= letra){	//lista com um elemento, verifica se o novo deve ir antes

		if (chars_head->letra == letra)	//o caractere eh o unico na lista, basta adicionar a chave
			adiciona_chave(num_chave, chars_head);

		else {		//o caractere deve ser inserido como primeiro da lista
				struct caractere* novo = aloca_caracter(letra);

				novo->prox = chars_head;
				chars_head = novo;
				adiciona_chave(num_chave, chars_head);
		}
	} else {
		struct caractere* aux = chars_head;
		while ((aux->prox != NULL) && (aux->prox->letra < letra))	//procura onde o novo elemento deve ser inserido
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
	return chars_head;
}

void imprime_lista(struct caractere* chars_head){
	struct caractere* aux = chars_head;
	struct chave* aux_chave;

	while (aux != NULL){
		printf("%c: ", aux->letra);
		aux_chave = aux->chaves_head;
		while (aux_chave != NULL){
			printf("%d ", aux_chave->num_chave);
			aux_chave = aux_chave->prox;
		}
		printf("\n");
		aux = aux->prox;
	}
}

struct caractere* gera_lista(struct caractere* chars_head, FILE* f_livro){
	int num_chave = 0;
	char palavra[30];

	while (fscanf(f_livro, "%s", palavra) != EOF){
		chars_head = insere_lista(chars_head, num_chave, palavra[0]);
		num_chave++;
	}
	return chars_head;
}

int seleciona_chaves(struct caractere* chars_head, char letra){
	//seleciona uma chave que corresponde a letra do texto
	struct caractere* aux1 = chars_head;
	struct chave* aux2;

	while (aux1 != NULL){
		if (aux1->letra == letra){	//encontra o caractere
			aux2 = aux1->chaves_head;
			int pos = rand() % aux1->tamanho;	//aleatoriza a posicao da chave

			for (int i = 0; i < pos; i++)
				aux2 = aux2->prox;	//anda ate a posicao aleatoria

			return aux2->num_chave;		//retorna a chave
		}
		aux1 = aux1->prox;
	}

	//se nao encontrouo caractere, codifica como -2
	return -2;
}

//FUNCOES DE CODIFICAR E DECODIFICAR
void codifica(struct caractere* chars_head, FILE* f_mensagem_original, FILE* f_mensagem_codificada){
	char c;
	while ((c = fgetc(f_mensagem_original)) != EOF){	//enquanto o arquivo nao terminou
		if (c == ' ')
			fprintf(f_mensagem_codificada, "-1 ");	//se for espaco, codifica como -1 
		else
			fprintf(f_mensagem_codificada, "%d ", seleciona_chaves(chars_head, c)); 	//seleciona uma chave que corresponde a letra do texto
	}
}

void decodifica(struct caractere* chars_head, FILE* f_mensagem_codificada, FILE* f_mensagem_decodificada){
	int numero;

	while (fscanf(f_mensagem_codificada, "%d", &numero) != EOF){	//enquanto nao chegar ao fim do arquivo,le um numero
		if (numero == -1) 	//se for -1, eh um espaco
			fprintf(f_mensagem_decodificada, " ");

		else if (numero == -2)	//se for -2, eh um caractere que nao esta nas chaves
			fprintf(f_mensagem_decodificada, "?");

		else { 
			//procura a letra que corresponde a chave
			struct caractere* aux1 = chars_head;
			struct chave* aux2;

			int achou = 0;
			while ((aux1 != NULL) && (achou != 1)){		//procura na lista de caracteres
				aux2 = aux1->chaves_head;

				while ((aux2 != NULL) && (achou != 1)){		//procura na lista de chaves
					if (aux2->num_chave == numero){
						fprintf(f_mensagem_decodificada, "%c", aux1->letra);
						achou = 1;
					}
					aux2 = aux2->prox;
				}
				aux1 = aux1->prox;
			}
		}
	}
}

void cria_arq_chaves(struct caractere* chars_head, FILE* f_chaves){
	
	while (chars_head != NULL){
		fprintf(f_chaves, "%c: ", chars_head->letra);

		struct chave* aux = chars_head->chaves_head;
		while (aux != NULL){
			fprintf(f_chaves, "%d ", aux->num_chave);
			aux = aux->prox;
		}

		fprintf(f_chaves, "\n");
		chars_head = chars_head->prox;
	}
	return;
}

void desaloca_lista(struct caractere* chars_head){
	struct caractere* aux = chars_head;
	while (aux != NULL){
		while (aux->chaves_head != NULL){
			struct chave* temp = aux->chaves_head->prox;
			free(aux->chaves_head);
			aux->chaves_head = temp;
		}

		struct caractere* temp = aux->prox;
		free(aux);
		aux = temp;
	}
}

void cria_lista(struct caractere* chars_head, FILE* f_chaves){
	char letra;
	int chave;
	struct caractere* aux1 = chars_head;
	struct chave* aux2;

	while (fscanf(f_chaves, "%c", &letra) != EOF){		//enquanto nao chegou no fim do arquivo
		aux1->letra = letra;		//guarda o caractere
		fscanf(f_chaves, "%c", &letra);		//pula o ":"

		aux2 = aux1->chaves_head;
		while (){	//enquanto nao chegar no fim da linha
			fscanf(f_chaves, "%d", &chave);		//guarda a chave
			adiciona_chave(chave, aux2);
			aux2 = aux2->prox;
		}

		aux1 = aux1->prox;
	}
}
