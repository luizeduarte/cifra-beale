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

struct conteudo{
	unsigned int num_arq;
	long long int tam_conteudo, diretorio_pos;
};

struct diretorio** le_diretorio(FILE* archive){
	struct conteudo info_conteudo;
	struct diretorio* *v_diretorio;

	fread(&info_conteudo.num_arq, sizeof(int), 1, archive);		//le o numero de arquivos
	v_diretorio = malloc(info_conteudo.num_arq * sizeof(struct diretorio*));
	fread(&info_conteudo.tam_conteudo, sizeof(long long int), 1, archive);		//le o tamanho dos conteudos
	info_conteudo.diretorio_pos = info_conteudo.tam_conteudo + sizeof(long long int) + sizeof(int);	//guarda a posicao do diretorio
	
	fseek(archive, info_conteudo.diretorio_pos, SEEK_SET);	//vai para o diretorio

	for (int i = 0; i < info_conteudo.num_arq; i++){
		v_diretorio[i] = malloc(sizeof(struct diretorio));
		fread(&v_diretorio[i]->tam_nome, sizeof(int), 1, archive);	//le o tamanho do nome do arquivo
		v_diretorio[i]->nome = malloc(v_diretorio[i]->tam_nome * sizeof(char));
		fread(v_diretorio[i]->nome, sizeof(char), v_diretorio[i]->tam_nome, archive);
		fread(&v_diretorio[i]->tamanho, sizeof(long long int), 1, archive);	//le o tamanho do arquivo
		fread(&v_diretorio[i]->posicao, sizeof(long long int), 1, archive);	//le a posicao do arquivo
		fread(&v_diretorio[i]->uid, sizeof(uid_t), 1, archive);	//le o uid do arquivo
		fread(&v_diretorio[i]->permissoes, sizeof(mode_t), 1, archive);	//le as permissoes do arquivo
		fread(&v_diretorio[i]->ultima_modificacao, sizeof(time_t), 1, archive);	//le a ultima modificacao do arquivo
	}

	return v_diretorio;
}

void lista(FILE* archive){
	struct conteudo info_conteudo;

	if (fread(&info_conteudo.num_arquivos, sizeof(int), 1, archive) == 0){
		fprintf(stderr, "Archive vazio\n");
		return;
	}
	
	struct diretorio* *v_diretorio = le_diretorio(archive);
	//imprime formatado
}
