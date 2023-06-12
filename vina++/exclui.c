#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_STRING 1024

struct diretorio{
	int tam_nome;
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


int id_arquivo(FILE *archive, char *nome_arquivo, struct diretorio *v_diretorio[]){
	//recebe o archive e o nome de um arquivo, retornando o indice dele no vetor diretorio

	int achou = 0, num_arq;

	fseek(archive, 0, SEEK_SET);
	if (fread(&num_arq, sizeof(int), 1, archive) == 0)		//le o numero de arquivos
		return -1;	//dir vazio

	while ((achou == 0) && (num_arq > 0)){
		if (strcmp(v_diretorio[num_arq - 1]->nome, nome_arquivo) == 0){
			achou = 1;
		}

		num_arq--;
	}

	if (achou == 0)
		return -1;

	return num_arq;
}


struct conteudo sub_info_conteudos(FILE* archive, struct diretorio* v_diretorio[], int arquivo){
	struct conteudo info_conteudo;

	//mudar para ponteiros 

	fseek(archive, 0, SEEK_SET);
	if (fread(&info_conteudo->num_arq, sizeof(int), 1, archive) == 0){
		fprintf(stderr, "Archive vazio\n");
		return info_conteudo;
	}
	fread(&info_conteudo->tam_conteudo, sizeof(long long int), 1, archive);

	info_conteudo->num_arq--;
	info_conteudo->tam_conteudo -= v_diretorio[arquivo]->tamanho;

	fseek(archive, 0, SEEK_SET);
	fwrite(&info_conteudo->num_arq, sizeof(int), 1, archive);	
	fwrite(&info_conteudo->tam_conteudo, sizeof(long long int), 1, archive);

	info_conteudo->diretorio_pos = sizeof(long long int) + sizeof(int) + info_conteudo->tam_conteudo;

	return info_conteudo;
}


void move_conteudo(FILE* archive, struct diretorio* v_diretorio[], int arquivo, long long int diretorio_pos){
	long long int nova_pos = v_diretorio[arquivo]->posicao;
	long long int antiga_pos = nova_pos + v_diretorio[arquivo]->tamanho;
	long long int tam_mover = diretorio_pos - nova_pos - v_diretorio[arquivo]->tamanho;

	//calcula o carregamento em blocos 
	long long int num_blocos = tam_mover / MAX_STRING;
	long long int resto = tam_mover % MAX_STRING;

	char* buffer = malloc(sizeof(char) * MAX_STRING);
	//copia os blocos para a nova posicao no archive 
	for (int i = 0; i < num_blocos; i++){
		fseek(archive, antiga_pos, SEEK_SET);
		fread(&buffer, sizeof(char), MAX_STRING, archive);
		fseek(archive, nova_pos, SEEK_SET);
		fwrite(&buffer, sizeof(char), MAX_STRING, archive);
		nova_pos += MAX_STRING;
		antiga_pos += MAX_STRING;
	}

	//copia o resto para a nova posicao no archive
	fseek(archive, antiga_pos, SEEK_SET);
	fread(&buffer, sizeof(char), resto, archive);
	fseek(archive, nova_pos, SEEK_SET);
	fwrite(&buffer, sizeof(char), resto, archive);
	free(buffer);
}

void imprime_diretorio(FILE* archive, struct diretorio* v_diretorio[], struct conteudo info_conteudo){
	for (int i = 0; i < info_conteudo->num_arq; i++){
		if (v_diretorio[i]){
			fwrite(&v_diretorio[i]->tam_nome, sizeof(int), 1, archive);
			fwrite(v_diretorio[i]->nome, sizeof(char), v_diretorio[i]->tam_nome , archive);
			fwrite(&v_diretorio[i]->tamanho, sizeof(long long int), 1, archive);
			fwrite(&v_diretorio[i]->posicao, sizeof(long long int), 1, archive);
			fwrite(&v_diretorio[i]->uid, sizeof(uid_t), 1, archive);
			fwrite(&v_diretorio[i]->permissoes, sizeof(mode_t), 1, archive);
			fwrite(&v_diretorio[i]->ultima_modificacao, sizeof(time_t), 1, archive);
		}
	}
}


struct diretorio** le_diretorio(FILE* archive){
	struct conteudo info_conteudo;
	struct diretorio* *v_diretorio;

	if (fread(&info_conteudo->num_arq, sizeof(int), 1, archive) == 0)	//le o numero de arquivos
		return NULL;
	v_diretorio = malloc(info_conteudo->num_arq * sizeof(struct diretorio*));
	fread(&info_conteudo->tam_conteudo, sizeof(long long int), 1, archive);		//le o tamanho dos conteudos
	info_conteudo->diretorio_pos = info_conteudo->tam_conteudo + sizeof(long long int) + sizeof(int);	//guarda a posicao do diretorio
	
	fseek(archive, info_conteudo->diretorio_pos, SEEK_SET);	//vai para o diretorio

	for (int i = 0; i < info_conteudo->num_arq; i++){
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


void exclui(FILE* archive, struct diretorio* v_diretorio[], int arquivo, char* nome_archive){

	//atualiza o numero de arquivos e o tamanho do conteudo
	struct conteudo* info_conteudo = sub_info_conteudos(archive, v_diretorio, arquivo);
	if (info_conteudo->num_arq == 0){
		truncate(nome_archive, 0);
		return;
	}

	move_conteudo(archive, v_diretorio, arquivo, info_conteudo->diretorio_pos);

	v_diretorio[arquivo] = NULL;
	imprime_diretorio(archive, v_diretorio, info_conteudo);

	long long int tam_archive = ftell(archive);
	truncate(nome_archive, tam_archive);
}



int main(){
	FILE* archive = fopen("archive.bin", "r+b");
	struct diretorio* *v_diretorio = le_diretorio(archive);
	int id_arq = id_arquivo(archive, "teste.txt", v_diretorio);

	exclui(archive, v_diretorio, id_arq, "archive.bin");
}