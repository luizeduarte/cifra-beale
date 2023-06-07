#include "manipula_conteudo.h"
#include "manipula_dir.h"

void insere(char* nome_arquivo, FILE* arq_novo, FILE* archive, struct diretorio* v_diretorio[]);

void extrai(FILE* archive, char* nome_arquivo, struct diretorio* v_diretorio[]);

void exclui(FILE* archive, struct diretorio* v_diretorio[], int arquivo);