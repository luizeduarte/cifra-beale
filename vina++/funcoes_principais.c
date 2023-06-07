#include "funcoes_principais.h"

void insere(char* nome_arquivo, FILE* arq_novo, FILE* archive, struct diretorio* v_diretorio[]){
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
	v_diretorio = att_diretorio(v_diretorio, info_conteudo, info_arquivo, nome_arquivo);

	//imprime o diretório no fim do archive
	imprime_diretorio(archive, v_diretorio, info_conteudo);
}

//onde vai as informações do arquivo que estavam no diretorio? chmod
//https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c

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

void exclui(FILE* archive, struct diretorio* v_diretorio[], int arquivo){

	//atualiza o numero de arquivos e o tamanho do conteudo
	struct conteudo info_conteudo = sub_info_conteudos(archive, v_diretorio, arquivo);
	if (info_conteudo.num_arq == 0){
		ftruncate(fileno(archive), 0);
		return;
	}

	move_conteudo(archive, v_diretorio, arquivo, info_conteudo.diretorio_pos);

	v_diretorio[arquivo] = NULL;
	imprime_diretorio(archive, v_diretorio, info_conteudo);

	long long int tam_archive = ftell(archive);
	//cortar archive
	ftruncate(fileno(archive), tam_archive);
}