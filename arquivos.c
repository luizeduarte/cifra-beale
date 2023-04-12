#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
	int opt, flag_codificar = 0, flag_decodificar = 0, flag_arq_chaves = 0;
	char* livro_cifra = NULL, mensagem_original = NULL, mensagem_codificada = NULL, arquivo_chaves = NULL, mensagem_decodificada = NULL, 
	opcoes = "e:d:b:m:o:c:i:";
	struct caractere* chars_head = NULL;

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
				flag_arq_chaves = 1;
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
		if ((livro_cifra == NULL) || (mensagem_original == NULL) || (mensagem_codificada == NULL)) {
			printf("tenha certeza de inserir todas as opcoes.\n");
			return 1;
		}

		//abre os arquivos necessarios
		FILE* f_livro = fopen(livro_cifra, "r");
		FILE* f_mensagem_original = fopen(mensagem_original, "r");
		FILE* f_mensagem_codificada = fopen(mensagem_codificada, "w");	//modo de escrita apaga o que o arquivo possuia

		//verifica se nao houve erro ao abrir os arquivos
		if ((f_livro == NULL) || (f_mensagem_original == NULL) || (f_mensagem_codificada == NULL)){
			printf("erro ao abrir os arquivos");
			return 1;
		}

		chars_head = gera_lista(chars_head, f_livro);	//gera a lista de chaves a partir do livro cifra
		codifica(chars_head, f_mensagem_original, f_mensagem_codificada);
		
		//fecha os arquivos abertos
		fclose(f_mensagem_codificada);
		fclose(f_livro);
		fclose(f_mensagem_original);
		
		if (flag_arq_chaves == 1){	//se quiser criar um arquivo de chaves
			FILE* f_chaves = fopen(arquivo_chaves, "a+");
			if (f_chaves == NULL)	//verifica se nao houve erro
				printf("erro ao abrir o arquivo de chaves");
			
			cria_arq_chaves(chars_head, f_chaves);
			
			fclose(f_chaves);
		}
		
		return 0;
	}

	//DECODIFICAR
	if (flag_decodificar == 1) {
		//checa se passou todos os argumentos necessarios
		if ((mensagem_codificada == NULL) || (mensagem_decodificada == NULL) || (~((livro_cifra == NULL) ^ (arquivo_chaves == NULL)))) {
			printf("tenha certeza de inserir todas as opcoes.\n");
			return 1;
		}

		//abre os arquivos
		FILE* f_mensagem_codificada = fopen(mensagem_codificada, "r");
		FILE* f_mensagem_decodificada = fopen(mensagem_decodificada, "w");
		//verifica se houve algum erro
		if ((f_mensagem_codificada == NULL) || (f_mensagem_decodificada == NULL))
			printf("erro ao abrir os arquivos");

		//verifica qual tipo de decodificacao sera feita, usando o livro cifra ou o arquivo de chaves
		if (livro_cifra != NULL){

			//abre o arquivo do livro cifra para leitura
			FILE* f_livro = fopen(livro_cifra, "r");
			if ((f_livro == NULL))
				printf("erro ao abrir o livro de cifra");

			gera_lista(chars_head, f_livro);
			decodifica(chars_head, f_mensagem_codificada, f_mensagem_decodificada);

			fclose(f_livro);

		} else {

			//abre o arquivo de chaves para leitura
			FILE* f_chaves = fopen(arquivo_chaves, "r");
			if (f_chaves == NULL)
				printf("erro ao abrir o arquivo de chaves");

			//gera_lista_chave pelo arquivo de chaves
			decodifica(chars_head, f_mensagem_codificada, f_mensagem_decodificada);

			fclose(f_chaves);
		}	
	
		fclose(f_mensagem_decodificada);
		fclose(f_mensagem_codificada);
	}

}