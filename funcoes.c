#include  <stdio.h>
#include  <string.h>
#include <stdarg.h>

int verifica_arquivos(int num_arquivos, ...){
	va_list arg;	//cria a lista de argumentos
	va_start(arg, num_arquivos);	//inicia ela
	FILE* arq;

	for (int i = 0; i < num_arquivos; i++){
		arq = va_arg(arg, FILE*);	//pega o prox arquivo
		if (arq == NULL){	//verifica se abriu corretamente
			printf("erro ao abrir um dos arquivos.\n");
			va_end(arg);
			return 1;
		}
	}

	va_end(arg);
	return 0;
}

int seleciona_chaves(struct caractere* lista_chars, char letra){
	//seleciona uma chave que corresponde a letra do texto
	struct caractere* aux1 = lista_chars;
	struct chave* aux2;

	while (aux1 != NULL){
		if (aux1->letra == letra){	//encontra o caractere
			aux2 = aux1->lista_chaves;
			int pos = rand() % aux1->tamanho;	//aleatoriza a posicao da chave

			for (int i = 0; i < pos; i++)
				aux2 = aux2->prox;	//anda ate a posicao aleatoria

			return aux2->num_chave;		//retorna a chave
		}
		aux1 = aux1->prox;
	}
}

void codifica(struct caractere* lista_chars, FILE* f_mensagem_original, FILE* f_mensagem_codificada){
	char c;

	gera_chaves(lista_chars, f_mensagem_original);	//gera as chaves

	while ((c = fgetc(f_mensagem_original)) != EOF){	//enquanto o arquivo ainda nao terminou
		if (c == ' ')
			fprintf(f_mensagem_codificada, "-1 ");
		else 
			fprintf(f_mensagem_codificada, "%d ", seleciona_chaves(lista_chars, c)); 	//seleciona uma chave que corresponde a letra do texto
	}
}

void decodifica(struct caractere* lista_chars, FILE* f_mensagem_codificada, FILE* f_mensagem_decodificada){
	int numero;

	while (fscanf(f_mensagem_codificada, "%d", &numero) != EOF){
		if (numero == -1) 	//se for -1, eh um espaco
			fprintf(f_mensagem_decodificada, " ");
		else { 
			//procura a letra que corresponde a chave
			struct caractere* aux1 = lista_chars;
			struct chave* aux2;

			int achou = 0;
			while ((aux1 != NULL) && (achou != 1)){		//procura na lista de caracteres
				aux2 = aux1->lista_chaves;

				while ((aux2 != NULL) && (achou != 1)){		//procura na lista de chaves
					if (aux2->num_chave == numero);{
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
	num_chave++;
	aux->tamanho++;
	novo->prox = aux->lista_chaves;
	aux->lista_chaves = novo;

}

void adiciona_caracter(char letra, struct chave* aux){
	struct caractere* novo = aloca_caracter(letra);
	novo->prox = aux->prox;
	aux->prox = novo;
}

void insere_lista(struct caractere* lista_chars, int* num_chave, char letra){
	struct caractere* aux = lista_chars;

	while (aux->prox != NULL){	//enquanto nao chegar no penultipo elemento da lista
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

	//se chegou aqui, eh pq o caractere eh o ultimo da lista
	adiciona_caracter(letra, aux);
}

void desaloca_lista(struct caractere* lista_chars){
	struct caractere* aux = lista_chars;
	while (aux != NULL){
		while (aux->lista_chaves != NULL){
			struct chave* temp = aux->lista_chaves->prox;
			free(aux->lista_chaves);
			aux->lista_chaves = temp;
		}

		struct caractere* temp = aux->prox;
		free(aux);
		aux = temp;
	}
}

void gera_lista_chaves(struct caractere* lista_chars, FILE* f_livro){
	int* num_chave = 0;
	char letra, palavra[30];

	while (fscanf(f_livro, "%s", palavra) != EOF){
		insere_lista(lista_chars, num_chave, palavra[0]);
		num_chave++;
	}
}

void cria_arq_chaves(struct caractere* lista_chars, FILE* f_chaves){
	
	while (lista_chars != NULL){
		fprintf(f_chaves, "%c: ", lista_chars->letra);

		struct chave* aux = lista_chars->lista_chaves;
		while (aux != NULL){
			fprintf(f_chaves, "%d ", aux->num_chave);
			aux = aux->prox;
		}

		fprintf(f_chaves, "\n");
	}
}

void cria_lista(struct caractere* lista_chars, FILE* f_chaves){
	char letra;
	int num_chave;
	struct caractere* aux1 = lista_chars;
	struct chave* aux2;

	while (fscanf(f_chaves, "%c", &letra) != EOF){
		aux1->letra = letra;		//guarda o caractere
		fscanf(f_chaves, "%c", &letra);		//pula o ":"

		aux2 = aux1->lista_chaves;
		while (/*verifica se eh fim de linha*/){
			fscanf(f_chaves, "%d", &num_chave);	//guarda a chave
			adiciona_chave(num_chave, aux2);
			aux2 = aux2->prox;
		}

		aux1 = aux1->prox;
	}
}