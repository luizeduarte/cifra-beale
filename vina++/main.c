#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "manipula_conteudo.h"
#include "manipula_dir.h"
#include "funcoes_principais.h"

int main(int argc, char **argv){
	int inserir = 0, substituir = 0, extrair = 0, excluir = 0, mover = 0, listar = 0;
	char *opcoes = "iam:xrch", *nome_arq;
	int opt, num_arg;

	//processa os argumentos da linha de comando
	opt = getopt(argc, argv, opcoes);
	switch (opt){
		case 'i':
			num_arg = argc;
			inserir = 1;
			break;
		case 'a':
			substituir = 1;
			break;
		case 'm':
			mover = 1;
			break;
		case 'x':
			extrair = 1;
			break;
		case 'r':
			excluir = 1;
			break;
		case 'c':
			listar = 1;
			break;
		case 'h':
			printf("O programa deve ser executado da seguinte forma:\n./vina++ <opção> <archive> [membro1 membro2 ...]\n");
			break;
		//avisa se passou a especificacao errada
		default:
			fprintf(stderr, "opcao invalida: %c\n", optopt);
			return 1;
	}

	if (inserir){
		FILE *novo_arq, *archive = fopen(argv[2], "r+b");

		if(!archive){
			archive = fopen(argv[2], "wb");
		}
	
		struct diretorio* *v_diretorio = le_diretorio(archive);

		for (int i = 3; i < num_arg; i++){
			nome_arq = argv[i];

			exclui(archive, v_diretorio, nome_arq, argv[2]);

			novo_arq = fopen(nome_arq, "r");
			if (!novo_arq){
				fprintf(stderr, "erro ao abrir o arquivo %s", nome_arq);
			}

			insere(nome_arq, novo_arq, archive, v_diretorio);
			fclose(novo_arq);
		}
	} else if (substituir){

	} else if (mover){
		char* target = argv[2];
		char* arquivo = argv[4];

		FILE* archive = fopen(argv[3], "r+b");

		struct diretorio* *v_diretorio = le_diretorio(archive);

		move(archive, target, arquivo, v_diretorio);
	} else if (extrair){
		int num_arquivos;
		FILE* archive = fopen(argv[2], "r+b");
		struct diretorio* *v_diretorio = le_diretorio(archive);

		if (argc == 3){
			if (fread(&num_arquivos, sizeof(int), 1, archive) != 0){
				for (int i = 0; i < num_arquivos; i++){
					extrai(archive, v_diretorio[i]->nome, v_diretorio);
				}
			} else {
				fprintf(stderr, "Archive vazio\n");
				return 1;
			}
		} else {
			for (int i = 3; i < argc; i++){
				extrai(archive, argv[i], v_diretorio);
			}
		}
	} else if (excluir){
		FILE* archive = fopen(argv[2], "r+b");
		struct diretorio* *v_diretorio = le_diretorio(archive);

		if (!v_diretorio){
			fprintf(stderr, "Archive vazio\n");
			return 1;
		}

		for (int i = 3; i < argc; i++){
			exclui(archive, v_diretorio, argv[i], argv[2]);
		}
	} else if (listar){

	}
}
