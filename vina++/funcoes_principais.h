#ifndef FUNCOES_PRINCIPAIS_H
#define FUNCOES_PRINCIPAIS_H
#include "manipula_conteudo.h"
#include "manipula_dir.h"

int verifica_sub(time_t ultima_modificacao, char* nome_arquivo);

void substitui(struct diretorio* v_diretorio[], char* nome_arquivo, FILE* novo_arq, FILE* archive, int id_arq);

void substitui_arg(FILE* archive, struct diretorio* v_diretorio[], int argc, char** argv);

void insere(char* nome_arquivo, FILE* arq_novo, FILE* archive, struct diretorio* v_diretorio[]);

void insere_arg(FILE* archive, struct diretorio* v_diretorio[], int num_arq, char** argv, int optind);

void extrai(FILE* archive, char* nome_arquivo, struct diretorio* v_diretorio[]);

void extrai_arg(int argc, char* argv[], FILE* archive, struct diretorio* v_diretorio[]);

void exclui(FILE* archive, struct diretorio* v_diretorio[], int id_arq);

int exclui_arg(FILE* archive, struct diretorio* v_diretorio[], int num_arq, char** argv, int optind);

void move(FILE* archive, char* nome_target, char* nome_arquivo, struct diretorio* v_diretorio[]);

#endif