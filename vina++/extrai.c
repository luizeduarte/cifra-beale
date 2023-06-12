#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
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

void extrai(FILE* archive, char* nome_arquivo, struct diretorio* v_diretorio[]){	//verificar permissoes
	int arquivo;
	char* buffer = malloc(sizeof(char) * 1024);

	FILE* novo_arquivo = fopen(nome_arquivo, "w");

	arquivo = info_arquivo(archive, nome_arquivo, v_diretorio);
	if (arquivo < 0){
		fprintf(stderr, "Arquivo nao encontrado\n");
		return;
	}

	//calcula o numeros de blocos a serem lidos
	long long int num_blocos = v_diretorio[arquivo]->tamanho / 1024;
	int resto = v_diretorio[arquivo]->tamanho % 1024;

	fseek(archive, v_diretorio[arquivo]->posicao, SEEK_SET);
	//copia do archive para o novo arquivo
	for (int i = 0; i < num_blocos; i++){
		fread(buffer, sizeof(char), 1024, archive);
		fwrite(buffer, sizeof(char), 1024, novo_arquivo);
	}
	fread(buffer, sizeof(char), resto, archive);
	fwrite(buffer, sizeof(char), resto, novo_arquivo);

	fclose(novo_arquivo);
}

int main(){
	FILE* archive = fopen("archive.bin", "rb");
	struct diretorio* *v_diretorio = le_diretorio(archive);
	extrai(archive, "teste.txt", v_diretorio);
	return 0;
}