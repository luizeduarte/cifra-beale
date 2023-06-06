#include <stdio.h>
#include <sys/stat.h>
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

/*struct diretorio** le_diretorio(FILE* archive){
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
}*/

void insere_archive(char* nome_arquivo, FILE* arq_novo, FILE* archive, struct diretorio* v_diretorio[]){
	unsigned int num_arquivos;
	long long int tam_conteudo;
	char* buffer = malloc(sizeof(char) * 1024);
	struct stat info_arquivo;

	stat(nome_arquivo, &info_arquivo);	//pega as informacoes do arquivo novo

	//atualiza as variaveis do numero de arquivos e o tamanho dos conteudos, localizados no comeco do archive
	if (fread(&num_arquivos, sizeof(int), 1, archive) == 0){ 	//archive vazio
		num_arquivos = 1;
		tam_conteudo = info_arquivo.st_size;
	} else {
		num_arquivos++;
		fread(&tam_conteudo, sizeof(long long int), 1, archive);
		tam_conteudo += info_arquivo.st_size;
	}

	//escreve no archive os valores atualizados
	fseek(archive, 0, SEEK_SET);
	fwrite(&num_arquivos, sizeof(int), 1, archive);
	fwrite(&tam_conteudo, sizeof(long long int), 1, archive);

	//posiciona o ponteiro no fim dos conteudos
	fseek(archive, (tam_conteudo + sizeof(long long int) + sizeof(int) - info_arquivo.st_size), SEEK_SET);
	//calcula o numeros de blocos a serem lidos
	long long int num_blocos = info_arquivo.st_size/ 1024;
	long long int resto = info_arquivo.st_size % 1024;
 
	//copia do novo arquivo para o archive
	for (long long int i = 0; i < num_blocos; i++){
		fread(buffer, sizeof(char), 1024, arq_novo);
		fwrite(buffer, sizeof(char), 1024, archive);
	}
	fread(buffer, sizeof(char), resto, arq_novo);
	fwrite(buffer, sizeof(char), resto, archive);

	//atualiza o diretorio
	v_diretorio = malloc(num_arquivos*sizeof(struct diretorio*));
	v_diretorio[num_arquivos - 1] = malloc(sizeof(struct diretorio));

	v_diretorio[num_arquivos - 1]->nome = nome_arquivo;
	v_diretorio[num_arquivos - 1]->tamanho = info_arquivo.st_size;
	v_diretorio[num_arquivos - 1]->posicao = tam_conteudo + sizeof(long long int) + sizeof(int) - info_arquivo.st_size;
	v_diretorio[num_arquivos - 1]->uid = info_arquivo.st_uid;
	v_diretorio[num_arquivos - 1]->permissoes = info_arquivo.st_mode;
	v_diretorio[num_arquivos - 1]->ultima_modificacao = info_arquivo.st_mtime;

	int tam_nome; 
	//agora, é preciso imprimir o diretório no fim do arquivo
	for (int i = 0; i < num_arquivos; i++){
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

int main(){
	FILE* archive = fopen("archive.bin", "wb");
	FILE* arq_novo = fopen("teste.txt", "r");
	struct diretorio* *v_diretorio = NULL;

	insere_archive("teste.txt", arq_novo, archive, v_diretorio);

	return 0;
}