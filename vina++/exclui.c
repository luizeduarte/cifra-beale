#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

struct diretorio{
	char* nome;
	long long int tamanho;
	long long int posicao;
	uid_t uid;
	mode_t permissoes;
	time_t ultima_modificacao;
};

struct diretorio** le_diretorio(FILE* archive){
	int num_arquivos, tam_nome, tamanho;
	struct diretorio* *v_diretorio;

	fread(&num_arquivos, sizeof(int), 1, archive);		//le o numero de arquivos
	v_diretorio = malloc(num_arquivos * sizeof(struct diretorio*));
	fread(&tamanho, sizeof(long long int), 1, archive);		//le o tamanho dos conteudos
	fseek(archive, (tamanho + sizeof(long long int) + sizeof(int)), SEEK_SET);	//vai para o diretorio

	for (int i = 0; i < num_arquivos; i++){
		v_diretorio[i] = malloc(sizeof(struct diretorio));
		fread(&tam_nome, sizeof(int), 1, archive);	//le o tamanho do nome do arquivo
		v_diretorio[i]->nome = malloc(tam_nome * sizeof(char));
		fread(v_diretorio[i]->nome, sizeof(char), tam_nome, archive);
		fread(&v_diretorio[i]->tamanho, sizeof(long long int), 1, archive);	//le o tamanho do arquivo
		fread(&v_diretorio[i]->posicao, sizeof(long long int), 1, archive);	//le a posicao do arquivo
		fread(&v_diretorio[i]->uid, sizeof(uid_t), 1, archive);	//le o uid do arquivo
		fread(&v_diretorio[i]->permissoes, sizeof(mode_t), 1, archive);	//le as permissoes do arquivo
		fread(&v_diretorio[i]->ultima_modificacao, sizeof(time_t), 1, archive);	//le a ultima modificacao do arquivo
	}

	return v_diretorio;
}

int info_arquivo(FILE* archive, char* nome_arquivo, struct diretorio* v_diretorio[]){
	//recebe o nome de um arquivo e retorna o indice dele no vetor diretorio

	int achou = 0, num_arquivos;

	fseek(archive, 0, SEEK_SET);
	fread(&num_arquivos, sizeof(int), 1, archive);		//le o numero de arquivos
	while ((achou == 0) && (num_arquivos > 0)){
		if (strcmp(v_diretorio[num_arquivos - 1]->nome, nome_arquivo) == 0){
			achou = 1;
		}

		num_arquivos--;
	}

	if (achou == 0)
		return -1;

	return num_arquivos;
}

void exclui(FILE* archive, char* nome_arquivo, struct diretorio* v_diretorio[]){
	int arquivo, num_arquivos;
	long long int diretorio_pos, tam_conteudo;
	char* buffer = malloc(sizeof(char) * 1024), tam_nome;
	arquivo = info_arquivo(archive, nome_arquivo, v_diretorio);

	//guarda onde começa o diretório
	fseek(archive, 0, SEEK_SET);
	if (fread(&num_arquivos, sizeof(int), 1, archive) == 0){
		fprintf(stderr, "Archive vazio\n");
		return;
	}
	fread(&tam_conteudo, sizeof(long long int), 1, archive);
	diretorio_pos = tam_conteudo + sizeof(long long int) + sizeof(int);
	fseek(archive, 0, SEEK_SET);

	//atualiza o numero de arquivos
	num_arquivos--;
	fwrite(&num_arquivos, sizeof(int), 1, archive);
	tam_conteudo -= v_diretorio[arquivo]->tamanho;
	fwrite(&tam_conteudo, sizeof(long long int), 1, archive);

	if (arquivo < 0){
		fprintf(stderr, "Arquivo nao encontrado\n");
		return;
	}

	long long int nova_pos = v_diretorio[arquivo]->posicao;
	long long int antiga_pos = nova_pos + v_diretorio[arquivo]->tamanho;

	//calcula o carregamento em blocos 
	long long int num_blocos = v_diretorio[arquivo]->tamanho / 1024;
	long long int resto = v_diretorio[arquivo]->tamanho % 1024;

	//copia os blocos para a nova posicao no archive 
	for (int i = 0; i < num_blocos; i++){
		fseek(archive, antiga_pos, SEEK_SET);
		fread(&buffer, sizeof(char), 1024, archive);
		fseek(archive, nova_pos, SEEK_SET);
		fwrite(&buffer, sizeof(char), 1024, archive);
		nova_pos += 1024;
		antiga_pos += 1024;
	}

	//copia o resto para a nova posicao no archive
	fseek(archive, antiga_pos, SEEK_SET);
	fread(&buffer, sizeof(char), resto, archive);
	fseek(archive, nova_pos, SEEK_SET);
	fwrite(&buffer, sizeof(char), resto, archive);

	//atualiza o diretorio
	for (int i = 0; i < num_arquivos; i++){
		if (i != arquivo){
		tam_nome = strlen(v_diretorio[i]->nome);
		fwrite(&tam_nome, sizeof(int), 1, archive);
		fwrite(v_diretorio[i]->nome, sizeof(char), tam_nome , archive);
		fwrite(&v_diretorio[i]->tamanho, sizeof(long long int), 1, archive);
		fwrite(&v_diretorio[i]->posicao, sizeof(long long int), 1, archive);
		fwrite(&v_diretorio[i]->uid, sizeof(uid_t), 1, archive);
		fwrite(&v_diretorio[i]->permissoes, sizeof(mode_t), 1, archive);
		fwrite(&v_diretorio[i]->ultima_modificacao, sizeof(time_t), 1, archive);
		}
	}
}

int main(){
	//abre o archive para ler e escrever os binarios
	FILE* archive = fopen("archive.bin", "r+b");
	struct diretorio* *v_diretorio = le_diretorio(archive);
	exclui(archive, "teste.txt", v_diretorio);
	return 0;
}