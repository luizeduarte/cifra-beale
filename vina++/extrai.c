#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING 1024
//lê muitas vezes o numero de arquivos e o tamanho do conteudo, 
//verificar quando fizer o main se continua mesmo repetitivo, 
//talvez criar uma função, mas ela faria mts coisas, talvez uma struct
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

struct diretorio** le_diretorio(FILE* archive){
	int num_arq, tam_nome, tamanho;
	struct diretorio* *v_diretorio;

	fread(&num_arq, sizeof(int), 1, archive);		//le o numero de arquivos
	v_diretorio = malloc(num_arq * sizeof(struct diretorio*));
	fread(&tamanho, sizeof(long long int), 1, archive);		//le o tamanho dos conteudos
	fseek(archive, (tamanho + sizeof(long long int) + sizeof(int)), SEEK_SET);	//vai para o diretorio

	for (int i = 0; i < num_arq; i++){
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

int id_arquivo(FILE *archive, char *nome_arquivo, struct diretorio *v_diretorio[])
{
	//recebe o archive e o nome de um arquivo, retornando o indice dele no vetor diretorio

	int achou = 0, num_arq;

	fseek(archive, 0, SEEK_SET);
	fread(&num_arq, sizeof(int), 1, archive);		//le o numero de arquivos
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

void copia_texto(FILE* archive, struct diretorio* v_diretorio[], int arquivo, FILE* arq_novo){
	char* buffer = malloc(sizeof(char) * MAX_STRING);
	long long int tam_texto = v_diretorio[arquivo]->tamanho;
	long long int pos_texto = v_diretorio[arquivo]->posicao;
	FILE* arq_le = archive;
	FILE* arq_escreve = arq_novo;
	
	//calcula o numeros de blocos a serem lidos
	long long int num_blocos = tam_texto / MAX_STRING;
	int resto = tam_texto % MAX_STRING;

	fseek(arq_le, pos_texto, SEEK_SET);	//vai ate o comeco do texto a ser copiado
	for (int i = 0; i < num_blocos; i++){
		fread(buffer, sizeof(char), MAX_STRING, arq_le);
		fwrite(buffer, sizeof(char), MAX_STRING, arq_escreve);
	}
	fread(buffer, sizeof(char), resto, arq_le);
	fwrite(buffer, sizeof(char), resto, arq_escreve);
}

void extrai(FILE* archive, char* nome_arquivo, struct diretorio* v_diretorio[]){	//verificar permissoes
	int arquivo;

	FILE* arq_novo = fopen(nome_arquivo, "w");
	v_diretorio = le_diretorio(archive);

	arquivo = id_arquivo(archive, nome_arquivo, v_diretorio);
	if (arquivo < 0){
		fprintf(stderr, "Arquivo nao encontrado no archive\n");
		return;
	}



	fclose(arq_novo);
}

int main(){
	FILE* archive = fopen("archive.bin", "rb");
	struct diretorio* *v_diretorio = NULL;
	extrai(archive, "teste.txt", v_diretorio);
	return 0;
}