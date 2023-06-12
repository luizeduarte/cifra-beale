#include "funcoes_principais.h"
#include <unistd.h>

void insere(char* nome_arquivo, FILE* arq_novo, FILE* archive, struct diretorio* v_diretorio[]){
	struct conteudo* info_conteudo;
	struct stat info_arquivo;

	stat(nome_arquivo, &info_arquivo);	//pega as informacoes do arquivo novo

	int id_arq = id_arquivo(archive, nome_arquivo, v_diretorio);
	if((id_arquivo > 0) && (info_arquivo.st_mtime > v_diretorio[id_arq]->ultima_modificacao)){
		//substitui
	} else { 
		//atualiza o numero de arquivos e o tamanho total dos conteudos no comeco do archive
		info_conteudo = add_info_conteudo(archive, info_arquivo);

		//posiciona o ponteiro no fim dos conteudos
		fseek(archive, info_conteudo->diretorio_pos, SEEK_SET);

		//copia o texto do arquivo novo para o archive
		copia_texto(arq_novo, archive, info_arquivo.st_size);

		//atualiza o diretorio com o novo arquivo
		v_diretorio = att_diretorio(v_diretorio, info_conteudo, info_arquivo, nome_arquivo);

		//imprime o diretório no fim do archive
		imprime_diretorio(archive, v_diretorio, info_conteudo->num_arq);
	}
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

void exclui(FILE* archive, struct diretorio* v_diretorio[], char* nome_arquivo, char* nome_archive){

	int id_arq = id_arquivo(archive, nome_arquivo, v_diretorio);
	if (id_arq < 0){
		return;
	}

	//atualiza o numero de arquivos e o tamanho do conteudo
	struct conteudo* info_conteudo = sub_info_conteudos(archive, v_diretorio, id_arq);
	if (info_conteudo->num_arq == 0){
		remove(nome_archive);
		return;
	}

	move_conteudo(archive, v_diretorio, id_arq, info_conteudo->diretorio_pos);

	v_diretorio[id_arq] = NULL;
	imprime_diretorio(archive, v_diretorio, info_conteudo->num_arq);

	long long int tam_archive = ftell(archive) - 1;
	truncate(nome_archive, tam_archive);
}

void move(FILE* archive, char* nome_target, char* nome_arquivo, struct diretorio* v_diretorio[]){
	int target = id_arquivo(archive, nome_target, v_diretorio);
	int arquivo = id_arquivo(archive, nome_arquivo, v_diretorio);
	if ((target < 0) || (arquivo < 0)){
		fprintf(stderr, "Arquivo target nao encontrado\n");
		return;
	}

	struct diretorio* temp = v_diretorio[arquivo];

	if (arquivo > target){
		for (int i = arquivo; i < target; i--){
			v_diretorio[i] = v_diretorio[i - 1];
		}

		v_diretorio[target + 1] = temp;
	} else if (arquivo < target){
		for (int i = arquivo; i <= target; i++){
			v_diretorio[i] = v_diretorio[i + 1];
		}

		v_diretorio[target] = temp;
	}

	struct conteudo* info_conteudo = conteudo(archive);
	fseek(archive, info_conteudo->tam_conteudo + sizeof(long long int) + sizeof(int), SEEK_SET);
	imprime_diretorio(archive, v_diretorio, info_conteudo->num_arq);
}