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

void exclui(FILE* archive, char* nome_arquivo, struct diretorio* v_diretorio[]){
	struct diretorio* arquivo;
	long long int diretorio_pos;
	char* buffer = malloc(sizeof(char) * 1024);

	//guarda onde começa o diretório
	fseek(archive, 4, SEEK_SET);
	fread(&diretorio_pos, sizeof(long long int), 1, archive);
	diretorio_pos = diretorio_pos + sizeof(long long int) + sizeof(int);
	fseek(archive, 0, SEEK_SET);

	arquivo = info_arquivo(archive, nome_arquivo, v_diretorio);
	if (!arquivo){
		fprintf(stderr, "Arquivo nao encontrado\n");
		return;
	}

	long long int nova_pos = arquivo->posicao;
	long long int antiga_pos = nova_pos + arquivo->tamanho + 1;

	/* while (antiga_pos < diretorio_pos){
		fseek(archive, antiga_pos, SEEK_SET);
		fread(&buffer, sizeof(char), 1024, archive);
		fseek(archive, nova_pos, SEEK_SET);
		fwrite(&buffer, sizeof(char), 1024, archive);
		nova_pos += 1024;
		antiga_pos += 1024;
	}

	int i = arquivo->tamanho / 1024;
	int j = arquivo->tamanho % 1024;

	for (int k = 0; k < i; k++){
		fseek(archive, antiga_pos, SEEK_SET);
		fread(&buffer, sizeof(char), 1024, archive);
		fseek(archive, nova_pos, SEEK_SET);
		fwrite(&buffer, sizeof(char), 1024, archive);
		nova_pos += 1024;
		antiga_pos += 1024;
	} */

}