#include <stdio.h>

void move(FILE* archive, char* nome_target, char* nome_arquivo, struct diretorio* v_diretorio[]){
	int target = info_arquivo(archive, nome_target, v_diretorio);
	int arquivo = info_arquivo(archive, nome_arquivo, v_diretorio);
	if ((target < 0) || (arquivo < 0)){
		fprintf(stderr, "Arquivo target nao encontrado\n");
		return;
	}

	struct diretorio* temp = v_diretorio[arquivo];

	if (arquivo > target){
		for (int i = arquivo - 1; i < target; i--){
			v_diretorio[i] = v_diretorio[i+1];
		}
	} else if (arquivo < target){
		for (int i = arquivo + 1; i < target; i++){
			v_diretorio[i] = v_diretorio[i-1];
		}
	} 

	v_diretorio[target + 1] = temp;

}
