#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

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

struct conteudo add_info_conteudo(FILE* archive, struct stat info_arquivo){
	//atualiza as variaveis do numero de arquivos e o tamanho dos conteudos, localizados no comeco do archive
	struct conteudo info_conteudo;

	fseek(archive, 0, SEEK_SET);
	if (fread(&info_conteudo.num_arq, sizeof(int), 1, archive) == 0){ 	//archive vazio
		info_conteudo.num_arq = 1;
		info_conteudo.tam_conteudo = info_arquivo.st_size;
	} else {
		info_conteudo.num_arq++;
		fread(&info_conteudo.tam_conteudo, sizeof(long long int), 1, archive);
		info_conteudo.tam_conteudo += info_arquivo.st_size;
	}

	info_conteudo.diretorio_pos = info_conteudo.tam_conteudo + sizeof(long long int) + sizeof(int);

	//escreve no archive os valores atualizados
	fseek(archive, 0, SEEK_SET);
	fwrite(&info_conteudo.num_arq, sizeof(int), 1, archive);
	fwrite(&info_conteudo.tam_conteudo, sizeof(long long int), 1, archive);

	return info_conteudo;
}

struct diretorio* att_diretorio(FILE* archive, struct diretorio* v_diretorio, struct stat info_arquivo){
	v_diretorio = realloc(info_conteudo.num_arq*sizeof(struct diretorio*));
	v_diretorio[info_conteudo.num_arq - 1] = malloc(sizeof(struct diretorio));

	int tam_nome = strlen(nome_arquivo);
	v_diretorio[info_conteudo.num_arq - 1]->tam_nome = tam_nome;
	v_diretorio[info_conteudo.num_arq - 1]->nome = malloc(sizeof(char) * tam_nome);
	v_diretorio[info_conteudo.num_arq - 1]->nome = nome_arquivo;
	v_diretorio[info_conteudo.num_arq - 1]->tamanho = info_arquivo.st_size;
	v_diretorio[info_conteudo.num_arq - 1]->posicao = info_conteudo.tam_conteudo + sizeof(long long int) + sizeof(int) - info_arquivo.st_size;
	v_diretorio[info_conteudo.num_arq - 1]->uid = info_arquivo.st_uid;
	v_diretorio[info_conteudo.num_arq - 1]->permissoes = info_arquivo.st_mode;
	v_diretorio[info_conteudo.num_arq - 1]->ultima_modificacao = info_arquivo.st_mtime;

	return v_diretorio;
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

void insere_archive(char* nome_arquivo, FILE* arq_novo, FILE* archive, struct diretorio* v_diretorio[]){
	struct conteudo info_conteudo;
	struct stat info_arquivo;

	stat(nome_arquivo, &info_arquivo);	//pega as informacoes do arquivo novo

	//atualiza o numero de arquivos e o tamanho total dos conteudos no comeco do archive
	info_conteudo = add_info_conteudo(archive, info_arquivo);

	//posiciona o ponteiro no fim dos conteudos
	fseek(archive, (info_conteudo.diretorio_pos - info_arquivo.st_size), SEEK_SET);

	//copia o texto do arquivo novo para o archive
	copia_texto(arq_novo, archive, info_arquivo.st_size);

	//atualiza o diretorio com o novo arquivo
	v_diretorio = att_diretorio(archive, v_diretorio);

	//imprime o diretório no fim do archive
	imprime_diretorio(archive, v_diretorio);
}

int main(){
	FILE* archive = fopen("archive.bin", "wb");
	FILE* arq_novo = fopen("teste.txt", "r");
	struct diretorio* *v_diretorio = NULL;

	insere_archive("teste.txt", arq_novo, archive, v_diretorio);

	return 0;
}
