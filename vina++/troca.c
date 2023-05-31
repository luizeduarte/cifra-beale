#include <stdio.h>
#include <sys/types.h>

struct diretorio{
	char* nome;
	long long int tamanho;
	long long int posicao;
	uid_t uid;
	mode_t permissoes;
	time_t ultima_modificacao;
};


void troca(struct diretorio* v_diretorio[], char* nome_arq1, char* nome_arq2, FILE* archive){
	int arquivo1, arquivo2;

	arquivo1 = info_arquivo(archive, nome_arq1, v_diretorio);
	arquivo2 = info_arquivo(archive, nome_arq2, v_diretorio);

	if ((arquivo1 < 0) || (arquivo2 < 0)){
		fprintf(stderr, "Arquivo nao encontrado\n");
		return;
	}

	struct diretorio* aux = v_diretorio[arquivo1];
	v_diretorio[arquivo1] = v_diretorio[arquivo2];
	v_diretorio[arquivo2] = aux;
}