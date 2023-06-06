#include <stdio.h>

void move(FILE* archive, char* nome_target, char* nome_arquivo, struct diretorio* v_diretorio[]){
	int target = info_arquivo(archive, nome_target, v_diretorio);
	int arquivo = info_arquivo(archive, nome_arquivo, v_diretorio);
	if ((target == -1) || (arquivo == -1)){
		fprintf(stderr, "Arquivo target nao encontrado\n");
		return;
	}
	target++;

	struct diretorio* temp = v_diretorio[target];
	v_diretorio[target] = v_diretorio[arquivo];
	v_diretorio[arquivo] = temp;
}