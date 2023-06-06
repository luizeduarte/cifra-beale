#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_STRING 1024

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

int id_arquivo(FILE *archive, char *nome_arquivo, struct diretorio *v_diretorio[]){
	//recebe o archive e o nome de um arquivo, retornando o indice dele no vetor diretorio

	int achou = 0, info_conteudo.num_arq;

	fseek(archive, 0, SEEK_SET);
	fread(&info_conteudo.num_arq, sizeof(int), 1, archive);		//le o numero de arquivos
	while ((achou == 0) && (info_conteudo.num_arq > 0)){
		if (strcmp(v_diretorio[info_conteudo.num_arq - 1]->nome, nome_arquivo) == 0){
			achou = 1;
		}

		info_conteudo.num_arq--;
	}

	if (achou == 0)
		return -1;

	return info_conteudo.num_arq;
}

struct conteudo sub_info_conteudos(FILE* archive, struct diretorio* v_diretorio[]){
	struct conteudo info_conteudo;

	fseek(archive, 0, SEEK_SET);
	if (fread(&info_conteudo.num_arquivos, sizeof(int), 1, archive) == 0){
		fprintf(stderr, "Archive vazio\n");
		return;
	}
	fread(&info_conteudo.tam_conteudo, sizeof(long long int), 1, archive);

	info_conteudo.num_arquivos--;
	info_conteudo.tam_conteudo -= v_diretorio[arquivo]->tamanho;

	fseek(archive, 0, SEEK_SET);
	fwrite(&info_conteudo.num_arquivos, sizeof(int), 1, archive);	
	fwrite(&info_conteudo.tam_conteudo, sizeof(long long int), 1, archive);

	info_conteudo.diretorio_pos = sizeof(long long int) + sizeof(int) + info_conteudo.tam_conteudo;

	return info_conteudo;
}

void imprime_diretorio(FILE* archive, struct diretorio* v_diretorio[]){
	for (int i = 0; i < info_conteudo.num_arq; i++){
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

void move_conteudo(FILE* archive, struct diretorio* v_diretorio[], int arquivo, long long int diretorio_pos){
	long long int nova_pos = v_diretorio[arquivo]->posicao;
	long long int antiga_pos = nova_pos + v_diretorio[arquivo]->tamanho;
	long long int tam_mover = diretorio_pos - nova_pos - v_diretorio[arquivo]->tamanho;

	//calcula o carregamento em blocos 
	long long int num_blocos = tam_mover / MAX_STRING;
	long long int resto = tam_mover % MAX_STRING;

	char* buffer = malloc(sizeof(char) * MAX_STRING), tam_nome;
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

void exclui(FILE* archive, char* nome_arquivo, struct diretorio* v_diretorio[]){

	//atualiza o numero de arquivos e o tamanho do conteudo
	struct conteudo info_conteudo = sub_info_conteudos(archive, v_diretorio);
	if (info_conteudo.num_arquivos == 0){
		ftruncate(fileno(archive), 0);
		return;
	}

	int arquivo = info_arquivo(archive, nome_arquivo, v_diretorio);
	if (arquivo < 0){
		fprintf(stderr, "Arquivo nao encontrado\n");
		return;
	}

	move_conteudo(archive, v_diretorio, arquivo, info_conteudo.diretorio_pos);

	v_diretorio[arquivo] = NULL;
	imprime_diretorio(archive, v_diretorio);

	long long int tam_archive = ftell(archive);
	//cortar archive
	ftruncate(fileno(archive), tam_archive);
}

int main(){
	//abre o archive para ler e escrever os binarios
	FILE* archive = fopen("archive.bin", "r+b");
	struct diretorio* *v_diretorio = le_diretorio(archive);
	exclui(archive, "teste.txt", v_diretorio);
	return 0;
}
