#include "funcoes_principais.h"
#include <unistd.h>

int verifica_sub(time_t ultima_modificacao, char* nome_arquivo){
	/*a funcao verifica se o arquivo deve ser substituido no archive,
	dependendo de sua data de modificacao. Para isso ele recebe a data
	do arquivo ja presente no archive e seu nome, retornando 1 caso deve e 0 caso nao*/

	//pega a data de modificacao do arquivo 
	struct stat info_arquivo;
	stat(nome_arquivo, &info_arquivo);

	//se for maior que a do arquivo no archive, substitui
	if (info_arquivo.st_mtime > ultima_modificacao)
		return 1;
	
	return 0;
}

void substitui(struct diretorio* v_diretorio[], char* nome_arquivo, FILE* novo_arq, FILE* archive, int id_arq){
	/*a funcao verifica se é necessario realizar a substituicao de arquivos no archive, para isso ela recebe 
	um vetor de ponteiros para structs diretorio, o nome do arquivo que talvez seja substituido, ponteiros 
	para o mesmo e o archive, e, por fim, o index do arquivo no vetor citado*/
	if (verifica_sub(v_diretorio[id_arq]->ultima_modificacao, nome_arquivo)){
		//se deve ser substituido, exclui o arquivo antigo e insere o novo
		exclui(archive, v_diretorio, id_arq);
		insere(nome_arquivo, novo_arq, archive, v_diretorio);
	}
}

void substitui_arg(FILE* archive, struct diretorio* v_diretorio[], int argc, char** argv){
	FILE *novo_arq;
	int id_arq;

	//insere todos os arquivos passados como argumento
	for (int i = 3; i < argc; i++){

		novo_arq = fopen(argv[i], "r");
		if (!novo_arq){
			fprintf(stderr, "erro ao abrir o arquivo %s", argv[i]);
		}

		id_arq = id_arquivo(archive, argv[i], v_diretorio);
		substitui(v_diretorio, argv[i], novo_arq, archive, id_arq);

		fclose(novo_arq);
	}
}

void insere(char* nome_arquivo, FILE* arq_novo, FILE* archive, struct diretorio* v_diretorio[]){
	/*a funcao atualiza o archive inserindo no fim dos conteudos e no diretorio as informacoes 
	do arquivo novo, para isso ela recebe o nome de tal arquivo, um ponteiro para ele e o archive,
	alem de um vetor de ponteiros para structs diretorio*/
	struct conteudo* info_conteudo;
	struct stat info_arquivo;

	stat(nome_arquivo, &info_arquivo);	//pega as informacoes do arquivo novo

	//atualiza o numero de arquivos e o tamanho total dos conteudos no comeco do archive
	info_conteudo = add_info_conteudo(archive, info_arquivo.st_size);

	//posiciona o ponteiro no fim dos conteudos
	fseek(archive, info_conteudo->diretorio_pos, SEEK_SET);

	//copia o texto do arquivo novo para o archive
	copia_texto(arq_novo, archive, info_arquivo.st_size);

	//atualiza o diretorio com o novo arquivo
	v_diretorio = att_diretorio(v_diretorio, info_conteudo, info_arquivo, nome_arquivo);

	//imprime o diretório no fim do archive
	imprime_diretorio(archive, v_diretorio, info_conteudo->num_arq);

}

void insere_arg(FILE* archive, struct diretorio* v_diretorio[], int argc, char** argv){
	/*a funcao insere no archive todos os arquivos passados como argumentos, 
	substituindo os ja existentes pela outra versao*/
	FILE* novo_arq;
	int id_arq;

	for (int i = 3; i < argc; i++){
		//exclui se ele ja estiver no archive
		id_arq = id_arquivo(archive, argv[i], v_diretorio);
		if(id_arq != -1){
			exclui(archive, v_diretorio, id_arq);
		}

		novo_arq = fopen(argv[i], "r");
		if (!novo_arq){
			fprintf(stderr, "erro ao abrir o arquivo %s", argv[i]);
		}

		insere(argv[i], novo_arq, archive, v_diretorio);
		fclose(novo_arq);
	}
}
//onde vai as informações do arquivo que estavam no diretorio? chmod
//https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c

void extrai(FILE* archive, char* nome_arquivo, struct diretorio* v_diretorio[]){	//verificar permissoes
	int arquivo;

	arquivo = id_arquivo(archive, nome_arquivo, v_diretorio);
	if (arquivo < 0){
		fprintf(stderr, "Arquivo nao encontrado no archive\n");
		return;
	}

	FILE* arq_novo = fopen(nome_arquivo, "w");
	fseek(arq_novo, v_diretorio[arquivo]->posicao, SEEK_SET);	//vai ate o comeco do texto a ser copiado
	copia_texto(archive, arq_novo, v_diretorio[arquivo]->tamanho);
	fclose(arq_novo);
}

void extrai_arg(int argc, char* argv[], FILE* archive, struct diretorio* v_diretorio[]){
	int num_arquivos;

	if (argc == 3){
		//verifica se o archive nao esta vazio e extrai todos os arquivos
		if (fread(&num_arquivos, sizeof(int), 1, archive) != 0){
			for (int i = 0; i < num_arquivos; i++)
				extrai(archive, v_diretorio[i]->nome, v_diretorio);
		} else {
			fprintf(stderr, "Archive vazio\n");
			return;
		}
	} else {
		for (int i = 3; i < argc; i++){
			extrai(archive, argv[i], v_diretorio);
		}
	}
}

void exclui(FILE* archive, struct diretorio* v_diretorio[], int id_arq){
	//atualiza o numero de arquivos e o tamanho do conteudo
	struct conteudo* info_conteudo = sub_info_conteudos(archive, v_diretorio[id_arq]->tamanho);

	//se nao houver mais arquivos no archive, basta corta-lo no comeco
	if (info_conteudo->num_arq == 0){
		ftruncate(fileno(archive), 0);
		return;
	}

	//move o restante dos conteudos para o lugar do excluido
	move_conteudo(archive, v_diretorio[id_arq], info_conteudo->diretorio_pos);

	//imprime o diretorio sem o arquivo excluido, ao final do archive 
	v_diretorio[id_arq] = NULL;
	imprime_diretorio(archive, v_diretorio, info_conteudo->num_arq);

	//corta o archive no final do diretorio
	long long int tam_archive = ftell(archive) - 1;
	ftruncate(fileno(archive), tam_archive);
}

void exclui_arg(FILE* archive, struct diretorio* v_diretorio[], int argc, char** argv){
	int id_arq;
	for (int i = 3; i < argc; i++){
		id_arq = id_arquivo(archive, argv[i], v_diretorio);
		if (id_arq == -1){
			fprintf(stderr, "Arquivo %s nao existe no archive\n", argv[i]);
			return;
		}
		
		exclui(archive, v_diretorio, id_arq);
	}
}

void move(FILE* archive, char* nome_target, char* nome_arquivo, struct diretorio* v_diretorio[]){
	int target = id_arquivo(archive, nome_target, v_diretorio);
	int arquivo = id_arquivo(archive, nome_arquivo, v_diretorio);
	if ((target < 0) || (arquivo < 0)){
		fprintf(stderr, "Arquivo nao encontrado\n");
		return;
	}

	struct diretorio* temp = v_diretorio[arquivo];

	if (arquivo > target){	//se o arquivo estiver a frente do target, move para tras
		for (int i = arquivo; i < target; i--){
			v_diretorio[i] = v_diretorio[i - 1];
		}
		v_diretorio[target + 1] = temp;

	} else if (arquivo < target){	//se o arquivo estiver atras do target, move para frente
		for (int i = arquivo; i <= target; i++){
			v_diretorio[i] = v_diretorio[i + 1];
		}
		v_diretorio[target] = temp;
	}

	//imprime o diretorio no fim do archive
	struct conteudo* info_conteudo = conteudo(archive);
	fseek(archive, info_conteudo->tam_conteudo + sizeof(long long int) + sizeof(int), SEEK_SET);
	imprime_diretorio(archive, v_diretorio, info_conteudo->num_arq);
}