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
	fseek(archive, (tam_conteudo + sizeof(long long int) + sizeof(int)), SEEK_SET);
	//calcula o numeros de blocos a serem lidos
	int num_blocos = info_arquivo.st_size/ 1024;
	int resto = info_arquivo.st_size % 1024;

	//copia do novo arquivo para o archive
	for (int i = 0; i < num_blocos; i++){
		fread(&buffer, sizeof(char), 1024, arq_novo);
		fwrite(&buffer, sizeof(char), 1024, archive);
	}
	fread(&buffer, sizeof(char), resto, arq_novo);
	fwrite(&buffer, sizeof(char), resto, archive);

	//atualiza o diretorio
	v_diretorio = realloc(v_diretorio, num_arquivos*sizeof(struct diretorio*));

	v_diretorio[num_arquivos - 1]->nome = malloc(sizeof(char) * strlen(nome_arquivo));
	v_diretorio[num_arquivos - 1]->nome = nome_arquivo;
	v_diretorio[num_arquivos - 1]->tamanho = info_arquivo.st_size;
	v_diretorio[num_arquivos - 1]->posicao = tam_conteudo + sizeof(long long int) + sizeof(int);
	v_diretorio[num_arquivos - 1]->uid = info_arquivo.st_uid;
	v_diretorio[num_arquivos - 1]->permissoes = info_arquivo.st_mode;
	v_diretorio[num_arquivos - 1]->ultima_modificacao = info_arquivo.st_mtime;

	//agora, é preciso imprimir o diretório no fim do arquivo
	for (int i = 0; i < num_arquivos; i++){
		fwrite(v_diretorio[i], sizeof(struct diretorio), 1, archive);
	}
}

int main(){
	FILE* archive = fopen("archive.bin", "wb");
	FILE* arq_novo = fopen("teste.txt", "rb");
	struct diretorio* *v_diretorio = NULL;

	insere_archive("teste.txt", arq_novo, archive, v_diretorio);

	return 0;
}