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

void substitui_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int argc, char** argv){
	FILE *novo_arq;
	int id_arq, total_arq;

	//insere todos os arquivos passados como argumento
	for (int i = 3; i < argc; i++){
		novo_arq = fopen(argv[i], "r");
		if (!novo_arq){
			fprintf(stderr, "erro ao abrir o arquivo %s", argv[i]);
		}

		id_arq = id_arquivo(archive, argv[i], v_diretorio);
		if (id_arq < 0){
			insere(argv[i], novo_arq, archive, v_diretorio);
		} else{ 
			if (verifica_sub(v_diretorio[id_arq]->ultima_modificacao, argv[i])){
				//se deve ser substituido, exclui o arquivo antigo e insere o novo
				exclui(nome_archive, archive, v_diretorio, id_arq);
				insere(argv[i], novo_arq, archive, v_diretorio);
			}
		}
		fclose(novo_arq);
	}

	fseek(archive, 0, SEEK_SET);
	fread(&total_arq, sizeof(int), 1, archive);
	imprime_diretorio(archive, v_diretorio, total_arq);
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
	adiciona_diretorio(v_diretorio, info_conteudo, info_arquivo, nome_arquivo);
}

void insere_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int num_arq, char** argv, int optind){
	/*a funcao insere no archive todos os arquivos passados como argumentos, 
	substituindo os ja existentes pela outra versao*/
	FILE* novo_arq;
	int id_arq, total_arq;

	for (int i = optind; i < num_arq; i++){
		//exclui se ele ja estiver no archive
		id_arq = id_arquivo(archive, argv[i], v_diretorio);
		if(id_arq != -1){
			exclui(nome_archive, archive, v_diretorio, id_arq);
		}

		novo_arq = fopen(argv[i], "r");
		if (!novo_arq)
			fprintf(stderr, "erro ao abrir o arquivo %s, ele nao sera inserido\n", argv[i]);
		else 
			insere(argv[i], novo_arq, archive, v_diretorio);
		fclose(novo_arq);
	}

	fseek(archive, 0, SEEK_SET);
	fread(&total_arq, sizeof(int), 1, archive);
	imprime_diretorio(archive, v_diretorio, total_arq);
}
//https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c

void extrai(FILE* archive, char* nome_arquivo, struct diretorio* arquivo){

	FILE* arq_novo = fopen(nome_arquivo, "w");
	fseek(archive, arquivo->posicao, SEEK_SET);	//vai ate o comeco do texto a ser copiado
	copia_texto(archive, arq_novo, arquivo->tamanho);
	chmod(nome_arquivo, arquivo->permissoes);	//atribui as permissoes ao arquivo criado
	fclose(arq_novo);
}

void extrai_arg(int argc, char* argv[], FILE* archive, struct diretorio* v_diretorio[]){
	int num_arquivos, id_arq;

	fseek(archive, 0, SEEK_END);
	if (ftell(archive) == 0){
		fprintf(stderr, "archive vazio\n");
		return;
	}
	fseek(archive, 0, SEEK_SET);

	if (argc == 3){		//extrai todos os arquivos
		fread(&num_arquivos, sizeof(int), 1, archive);
		for (int i = 0; i < num_arquivos; i++){
			id_arq = id_arquivo(archive, argv[i], v_diretorio);
			if (id_arq < 0)
				fprintf(stderr, "arquivo nao encontrado no archive\n");
			else 
				extrai(archive, v_diretorio[i]->nome, v_diretorio[id_arq]);
		}

	} else {
		for (int i = 3; i < argc; i++){
			id_arq = id_arquivo(archive, argv[i], v_diretorio);
			if (id_arq < 0)
				fprintf(stderr, "arquivo nao encontrado no archive\n");
			else 
				extrai(archive, v_diretorio[i]->nome, v_diretorio[id_arq]);
		}
	}
}

void exclui(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int id_arq){
	//atualiza o numero de arquivos e o tamanho do conteudo
	struct conteudo* info_conteudo = sub_info_conteudos(archive, v_diretorio[id_arq]->tamanho);

	//se nao houver mais arquivos no archive, basta corta-lo no comeco
	if (info_conteudo->num_arq == 0)
		remove(nome_archive);

	//move o restante dos conteudos para o lugar do excluido
	move_conteudo(archive, v_diretorio[id_arq], info_conteudo->diretorio_pos);

	//retira o arquivo do diretorio e atualiza a posicao do restante
	for (int i = id_arq + 1; i < info_conteudo->num_arq; i++){
		v_diretorio[i]->posicao = - v_diretorio[id_arq]->tamanho;
	}
	for (int i = id_arq; i < info_conteudo->num_arq; i++){
		v_diretorio[i] = v_diretorio[i + 1];
	}
	
	//corta o archive no final dos conteudos
	ftruncate(fileno(archive), ftell(archive) - 1);
}

void exclui_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int num_arq, char** argv, int optind){
	int id_arq, total_arq;

	fseek(archive, 0, SEEK_END);
	if (ftell(archive) == 0){
		fprintf(stderr, "archive vazio\n");
		return;
	}
	fseek(archive, 0, SEEK_SET);

	for (int i = optind; i < num_arq; i++){
		id_arq = id_arquivo(archive, argv[i], v_diretorio);
		if (id_arq == -1){
			fprintf(stderr, "arquivo %s nao existe no archive\n", argv[i]);
			return;
		}
		
		exclui(nome_archive, archive, v_diretorio, id_arq);
	}

	fseek(archive, 0, SEEK_SET);
	fread(&total_arq, sizeof(int), 1, archive);
	imprime_diretorio(archive, v_diretorio, total_arq);
}

void move(FILE* archive, char* nome_target, char* nome_arquivo, struct diretorio* v_diretorio[]){
	int target = id_arquivo(archive, nome_target, v_diretorio);
	int arquivo = id_arquivo(archive, nome_arquivo, v_diretorio);
	if ((target < 0) || (arquivo < 0)){
		fprintf(stderr, "arquivo nao encontrado\n");
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