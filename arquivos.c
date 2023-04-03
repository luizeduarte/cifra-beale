#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
	int opt;
	int flag_codificar = 0;
	int flag_decodificar = 0;
	char *livro_cifra = NULL;
	char *mensagem_original = NULL;
	char *mensagem_codificada = NULL;
	char *arquivo_chaves = NULL;
	char *mensagem_decodificada = NULL;
	char *opcoes = "e:d:b:m:o:c:i:";
	struct caractere* lista_chars = NULL;
	int num_chave = 0;

	//processa os argumentos da linha de comando
	while ((opt = getopt(argc, argv, opcoes)) != -1) {
		switch (opt) {
			case 'e':
				flag_codificar = 1;
				break;
			case 'd':
				flag_decodificar = 1;
				break;
			case 'b':
				livro_cifra = optarg;
				break;
			case 'm':
				mensagem_original = optarg;
				break;
			case 'o':
				mensagem_codificada = optarg;
				break;
			case 'c':
				arquivo_chaves = optarg;
				break;
			case 'i':
				mensagem_decodificada = optarg;
				break;
			//avisa se passou a especificacao errada
			default:
				printf("opcao invalida: %c\n", optopt);
				return 1;
		}
	}

	//CODIFICAR
	if (flag_codificar == 1) {
		//checa se passou todos os argumentos necessarios
		if ((livro_cifra == NULL) || (mensagem_original == NULL) || (mensagem_codificada == NULL) || (arquivo_chaves == NULL)) {
			printf("tenha certeza de inserir todas as opcoes.\n");
			return 1;
		}

		//abre o arquivo do livro cifra para leitura
		FILE* f_livro = fopen(livro_cifra, "r");		
		//abre o arquivo da mensagem original para leitura
		FILE* f_mensagem_original = fopen(mensagem_original, "r");
		//abre o arquivo da mensagem codificada para escrita
		FILE* f_mensagem_codificada = fopen(mensagem_codificada, "w");	//modo de escrita apaga o que o arquivo possuia
		//abre o arquivo de chaves para escrita
		FILE* f_chaves = fopen(arquivo_chaves, "w");	//modo de escrita apaga o que o arquivo possuia

		//verifica se nao houve erro ao abrir os arquivos
		verifica_arquivos(4, f_livro, f_mensagem_original, f_mensagem_codificada, f_chaves);

		codifica(lista_chars, f_mensagem_original, f_mensagem_codificada);
		cria_arq_chaves(lista_chars, f_chaves);
		
		fclose(f_chaves);
		fclose(f_mensagem_codificada);
		fclose(f_livro);
		fclose(f_mensagem_original);
	}

	//DECODIFICAR
	if (flag_decodificar == 1) {
		//checa se passou todos os argumentos necessarios
		if ((mensagem_codificada == NULL) || (mensagem_decodificada == NULL) || (~((livro_cifra == NULL) ^ (arquivo_chaves == NULL)))) {
			printf("tenha certeza de inserir todas as opcoes.\n");
			return 1;
		}

		//abre o arquivo da mensagem codificada para leitura
		FILE* f_mensagem_codificada = fopen(mensagem_codificada, "r");
		//abre o arquivo da mensagem decodificada para escrita
		FILE* f_mensagem_decodificada = fopen(mensagem_decodificada, "w");

		//verifica qual tipo de decodificacao sera feita, usando o livro cifra ou o arquivo de chaves
		if (livro_cifra != NULL){

			//abre o arquivo do livro cifra para leitura
			FILE* f_livro = fopen(livro_cifra, "r");


			verifica_arquivos(3, f_livro, f_mensagem_codificada, f_mensagem_decodificada);

			gera_lista_chaves(lista_chars, f_livro);
			decodifica(lista_chars, f_mensagem_codificada, f_mensagem_decodificada);

			fclose(f_livro);

		} else {

			//abre o arquivo de chaves para leitura
			FILE* f_chaves = fopen(arquivo_chaves, "r");

			verifica_arquivos(3, f_chaves, f_mensagem_codificada, f_mensagem_decodificada);

			decodifica(lista_chars, f_mensagem_codificada, f_mensagem_decodificada);

			cria_lista(lista_chars, f_chaves);
			fclose(f_chaves);
		}	
	
		fclose(f_mensagem_decodificada);
		fclose(f_mensagem_codificada);
	}

}