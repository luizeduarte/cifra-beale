#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
				fprintf(stderr, "opcao invalida: %c\n", optopt);
				return 1;
		}
	}

	//CODIFICAR
	if (flag_codificar == 1) {
		//checa se passou todos os argumentos necessarios
		if ((livro_cifra == NULL) || (mensagem_original == NULL) || (mensagem_codificada == NULL) || (arquivo_chaves == NULL)) {
			fprintf(stderr, "tenha certeza de inserir todas as opcoes\n");
			return 1;
		}

		//abre o arquivo do livro cifra para leitura
		FILE *f_livro = fopen(livro_cifra, "r");

		//veririca se houve algum erro ao abri-lo
		if (f_livro == NULL) {
			fprintf(stderr, "erro ao abrir o arquivo do livro cifra\n");
			return 1;
		}
		
		fclose(f_livro);

		//abre o arquivo da mensagem original para leitura
		FILE *f_mensagem_original = fopen(mensagem_original, "r");

		//veririca se houve algum erro ao abri-lo
		if (f_mensagem_original == NULL) {
			fprintf(stderr, "erro ao abrir o arquivo da mensagem original\n");
			return 1;
		}

		fclose(f_mensagem_original);
	}


	//DECODIFICAR
	if (flag_decodificar == 1) {
		//checa se passou todos os argumentos necessarios
		if ((mensagem_codificada == NULL) || (mensagem_decodificada == NULL) || ((livro_cifra == NULL) && (arquivo_chaves == NULL))) {
			fprintf(stderr, "tenha certeza de inserir todas as opcoes\n");
			return 1;
		}

	//verifica qual tipo de decodificacao sera feita, usando o livro cifra ou o arquivo de chaves
	if (livro_cifra != NULL){

		//abre o arquivo do livro cifra para leitura
		FILE *f_livro = fopen(livro_cifra, "r");

		//veririca se houve algum erro ao abri-lo
		if (f_livro == NULL) {
			fprintf(stderr, "erro ao abrir o arquivo do livro cifra\n");
			return 1;
		}
		
		fclose(f_livro);
	} else {
		
	}




	//abre o arquivo da mensagem original para leitura
	FILE *f_mensagem_original = fopen(mensagem_original, "r");
	if (f_mensagem_original == NULL) {
		fprintf(stderr, "erro ao abrir o arquivo da mensagem original\n");
		return 1;
	}

	fclose(f_mensagem_original);
	}
}