#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include "codifica.h"
#include "decodifica.h"
#include "manipula_lista.h"
#include "abre_arquivos.h"
#include "cria_arq_chaves.h"

int main(int argc, char **argv){
	int opt, flag_codificar = 0, flag_decodificar = 0, flag_arq_chaves = 0;
	char* livro_cifra = NULL, *mensagem_original = NULL, *mensagem_codificada = NULL, *arquivo_chaves = NULL, *mensagem_decodificada = NULL;
	char *opcoes = "edb:m:o:c:i:";
	struct nodo_caractere* chars_head = NULL;

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
				if (flag_decodificar)
					mensagem_decodificada = optarg;
				else
					mensagem_codificada = optarg;
				break;
			case 'c':
				arquivo_chaves = optarg;
				flag_arq_chaves = 1;
				break;
			case 'i':
				mensagem_codificada = optarg;
				break;
			//avisa se passou a especificacao errada
			default:
				fprintf(stderr, "opcao invalida: %c\n", optopt);
				return 1;
		}
	}

	if (!((flag_codificar) ^ (flag_decodificar))) {
		fprintf(stderr, "especificar -e ou -d\n");
		return 1;
	}
	if ((flag_decodificar) && !((livro_cifra != NULL) ^ (arquivo_chaves != NULL))) {
		fprintf(stderr, "especificar -b ou -c\n");
		return 1;
	}
	
	//CODIFICAR
	if (flag_codificar) {
		FILE* f_livro = abre_leitura(livro_cifra);	//abre o arquivo, verificando possiveis erros 
		chars_head = gera_lista_livro(f_livro);		//gera a lista de chaves a partir do livro cifra
		fclose(f_livro);

		if (chars_head == NULL){		//se nao conseguiu gerar a lista
			fprintf(stderr, "erro ao gerar lista de chaves.\n");
			return 1;
		}

		//abre arquivos de mensagens
		FILE* f_mensagem_original = abre_leitura(mensagem_original);
		FILE* f_mensagem_codificada = abre_escrita(mensagem_codificada);	//modo de escrita apaga o que o arquivo possuia

		codifica(chars_head, f_mensagem_original, f_mensagem_codificada);

		//fecha os arquivos abertos
		fclose(f_mensagem_codificada);
		fclose(f_mensagem_original);
		

		if (flag_arq_chaves){	//se quiser criar um arquivo de chaves
			FILE* f_chaves = abre_escrita(arquivo_chaves);
			
			cria_arq_chaves(chars_head, f_chaves);
			
			fclose(f_chaves);
		}
		
		return 0;
	}

	//DECODIFICAR
	if (flag_decodificar) {

		//abre os arquivos, verificando possiveis erros
		FILE* f_mensagem_codificada = abre_leitura(mensagem_codificada);
		FILE* f_mensagem_decodificada = abre_escrita(mensagem_decodificada);

		//verifica qual tipo de decodificacao sera feita, usando o livro cifra ou o arquivo de chaves
		if (livro_cifra){

			//abre o arquivo do livro cifra para leitura
			FILE* f_livro = abre_leitura(livro_cifra);

			chars_head = gera_lista_livro(f_livro);	//gera as listas a partir do livro cifra

			if (!chars_head){		//se nao conseguiu gerar a lista
				fprintf(stderr, "erro ao gerar lista de chaves.\n");
				return 1;
			}

			decodifica(chars_head, f_mensagem_codificada, f_mensagem_decodificada);

			fclose(f_livro);

		} else {

			//abre o arquivo de chaves para leitura
			FILE* f_chaves = abre_leitura(arquivo_chaves);

			chars_head = gera_lista_arqchaves(f_chaves);	//cria a lista a partir do arquivo de chaves

			if (!chars_head){		//se nao conseguiu gerar a lista
				fprintf(stderr, "erro ao gerar lista de chaves.\n");
				return 1;
			}

			decodifica(chars_head, f_mensagem_codificada, f_mensagem_decodificada);

			fclose(f_chaves);
		}	
	
		fclose(f_mensagem_decodificada);
		fclose(f_mensagem_codificada);
	}
	desaloca_lista(chars_head);
}