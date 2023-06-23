#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "manipula_conteudo.h"
#include "manipula_dir.h"
#include "funcoes_principais.h"
#include "abre_arquivos.h"

int main(int argc, char **argv){
	int cont = 0, inserir = 0, substituir = 0, extrair = 0, excluir = 0, mover = 0, listar = 0;
	char *opcoes = "iam:xrch", *nome_archive, *target, opt;
	//processa os argumentos da linha de comando
	while((opt = getopt(argc, argv, opcoes)) != -1){
		cont++;
		if (cont > 1){
			fprintf(stderr, "Apenas uma opcao pode ser passada\n");
			return 1;
		}

		switch (opt){
			case 'i':
				inserir = 1;
				break;
			case 'a':
				substituir = 1;
				break;
			case 'm':
				mover = 1;
				target = optarg;
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
	}

	if (optind >= argc){ 	//nao foi passado o nome do arquivo
		fprintf(stderr, "Faltou o nome do arquivo\n");
		return 1;
	}
	nome_archive = argv[optind];

	if (inserir){
		FILE *archive = cria_arquivo(nome_archive);
		struct diretorio* *v_diretorio = le_diretorio(archive);

		insere_arg(archive, v_diretorio, argc, argv, optind + 1);
	} else if (substituir){
		FILE *archive = cria_arquivo(nome_archive);
		struct diretorio* *v_diretorio = le_diretorio(archive);

		substitui_arg(archive, v_diretorio, argc, argv);
	} else if (mover){
		FILE* archive = abre_arquivo(nome_archive);
		struct diretorio* *v_diretorio = le_diretorio(archive);

		move(archive, argv[2], argv[4], v_diretorio);
	} else if (extrair){
		FILE* archive = abre_arquivo(nome_archive);
		struct diretorio* *v_diretorio = le_diretorio(archive);

		extrai_arg(argc, argv, archive, v_diretorio);
	} else if (excluir){
		FILE* archive = abre_arquivo(nome_archive);
		struct diretorio* *v_diretorio = le_diretorio(archive);

		if (!exclui_arg(archive, v_diretorio, argc, argv, optind + 1)){
			remove(nome_archive);
		}
	} else if (listar){

	}
}