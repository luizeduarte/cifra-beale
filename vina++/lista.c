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

void lista(FILE* archive){
	int num_arquivos;
	long long int tamanho;
	fread(&num_arquivos, sizeof(int), 1, archive);		//le o numero de arquivos
	fread(&tamanho, sizeof(long long int), 1, archive);		//le o tamanho dos conteudos
	fseek(archive, (tamanho + sizeof(long long int) + sizeof(int)), SEEK_SET);	//vai para o diretorio

	long long int tam_dir;

	for (int i = 0; i < num_arquivos; i++){
		long long int tam_nome;
		fread(&tam_nome, sizeof(long long int), 1, archive);	//le o tamanho do nome do arquivo
		tam_dir = sizeof(char)*tam_nome + sizeof(long long int) + sizeof(uid_t) + sizeof(mode_t) + sizeof(time_t);
		fseek(archive, tam_dir, SEEK_CUR);
	}
}