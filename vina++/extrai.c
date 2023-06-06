#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING 1024

//onde vai as informações do arquivo que estavam no diretorio? chmod
//https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c

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

void copia_texto(FILE* arq_le, FILE* arq_escreve, long long int tam_texto){
	char* buffer = malloc(sizeof(char) * MAX_STRING);

	//calcula o numeros de blocos a serem lidos
	long long int num_blocos = tam_texto / MAX_STRING;
	int resto = tam_texto % MAX_STRING;

	for (int i = 0; i < num_blocos; i++){
		fread(buffer, sizeof(char), MAX_STRING, arq_le);
		fwrite(buffer, sizeof(char), MAX_STRING, arq_escreve);
	}
	fread(buffer, sizeof(char), resto, arq_le);
	fwrite(buffer, sizeof(char), resto, arq_escreve);
	free(buffer);
}

void extrai(FILE* archive, char* nome_arquivo, struct diretorio* v_diretorio[]){	//verificar permissoes
	int arquivo;

	FILE* arq_novo = fopen(nome_arquivo, "w");

	arquivo = id_arquivo(archive, nome_arquivo, v_diretorio);
	if (arquivo < 0){
		fprintf(stderr, "Arquivo nao encontrado no archive\n");
		return;
	}

	fseek(arq_novo, v_diretorio[arquivo]->posicao, SEEK_SET);	//vai ate o comeco do texto a ser copiado
	copia_texto(archive, arq_novo, v_diretorio[arquivo]->tamanho);

	fclose(arq_novo);
}

int main(){
	FILE* archive = fopen("archive.bin", "rb");
	struct diretorio* *v_diretorio = NULL;
	extrai(archive, "teste.txt", v_diretorio);
	return 0;
}
