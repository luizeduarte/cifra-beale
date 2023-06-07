#include "manipula_conteudo.h"

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

struct conteudo sub_info_conteudos(FILE* archive, struct diretorio* v_diretorio[], int arquivo){
	struct conteudo info_conteudo;

	fseek(archive, 0, SEEK_SET);
	if (fread(&info_conteudo.num_arq, sizeof(int), 1, archive) == 0){
		fprintf(stderr, "Archive vazio\n");
		return;
	}
	fread(&info_conteudo.tam_conteudo, sizeof(long long int), 1, archive);

	info_conteudo.num_arq--;
	info_conteudo.tam_conteudo -= v_diretorio[arquivo]->tamanho;

	fseek(archive, 0, SEEK_SET);
	fwrite(&info_conteudo.num_arq, sizeof(int), 1, archive);	
	fwrite(&info_conteudo.tam_conteudo, sizeof(long long int), 1, archive);

	info_conteudo.diretorio_pos = sizeof(long long int) + sizeof(int) + info_conteudo.tam_conteudo;

	return info_conteudo;
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
