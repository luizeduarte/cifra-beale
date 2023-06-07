#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "manipula_conteudo.h"
#include "manipula_dir.h"
#include "funcoes_principais.h"

int main(int argc, char **argv){
	int insere = 0, substitui = 0, extrai = 0, exclui = 0, move = 0, lista = 0;
	char *opcoes = "i:a:m:x:r:c:h:", *nome_arq	;
	int opt;

	//processa os argumentos da linha de comando
	while ((opt = getopt(argc, argv, opcoes)) != -1){
		switch (opt){
			case 'i':
				insere = 1;
				break;
			case 'a':
				substitui = 1;
				break;
			case 'm':
				move = 1;
				break;
			case 'x':
				extrai = 1;
				break;
			case 'r':
				exclui = 1;
				break;
			case 'c':
				lista = 1;
				break;
			case 'h':
				printf("O programa deve ser executado da seguinte forma:\n./vina++ <opção> <archive> [membro1 membro2 ...]");
				break;
			//avisa se passou a especificacao errada
			default:
				fprintf(stderr, "opcao invalida: %c\n", optopt);
				return 1;
		}
	}

	if (insere){
		int id_arq;
		FILE* archive = fopen(argv[2], "wb");
		struct diretorio* *v_diretorio = le_diretorio(archive);

		for (int i = 2; i < argc; i++){
			nome_arq = argv[i];

			FILE* novo_arq = fopen(nome_arq, "r");
			if (!novo_arq){
				fprintf(stderr, "erro ao abrir o arquivo %s", nome_arq);
			}

			id_arq = id_arquivo(archive, nome_arq, v_diretorio);
			if (id_arq > -1){	//se o arquivo ja esta no archive, deve ser substituido
				exclui(archive, v_diretorio, id_arq);
			}
			insere(nome_arq, novo_arq, archive, v_diretorio);

			fclose(novo_arq);
		}
	} else if (substitui){

	} else if (move){
		char* target = argv[2];
		char* archive = argv[3];
		char* arquivo = argv[4];
		struct diretorio* *v_diretorio = le_diretorio(archive);

		move(archive, target, arquivo, v_diretorio);
	} else if (extrai){
		FILE* archive = fopen(argv[2], "wb");
		struct diretorio* *v_diretorio = le_diretorio(archive);

		if (argc == 3){
			//extrai todos 
		} else {
			for (int i = 2; i < argc; i++){
				nome_arq = argv[i];

				FILE* novo_arq = fopen(nome_arq, "r");
				if (!novo_arq){
					fprintf(stderr, "erro ao abrir o arquivo %s", nome_arq);
				}
				extrai(nome_arq, novo_arq, archive, v_diretorio);
				fclose(novo_arq);
			}
		}
	} else if (exclui){
		FILE* archive = fopen(argv[2], "wb");
		struct diretorio* *v_diretorio = le_diretorio(archive);
		int id_arq;
		for (int i = 2; i < argc; i++){
			nome_arq = argv[i];

			id_arq = id_arquivo(archive, nome_arq, v_diretorio);
			if (id_arq < 0){	
				fprintf(stderr, "arquivo %s nao encontrado", nome_arq);
			} else {
				exclui(archive, v_diretorio, id_arq);
			}
		}
	} else if (lista){

	}
}