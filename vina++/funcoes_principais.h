#ifndef FUNCOES_PRINCIPAIS_H
#define FUNCOES_PRINCIPAIS_H
#include "manipula_conteudo.h"
#include "manipula_dir.h"

int verifica_sub(time_t ultima_modificacao, char* nome_arquivo);

void substitui(char* nome_archive, struct diretorio* v_diretorio[], char* nome_arquivo, FILE* novo_arq, FILE* archive, int id_arq);

void substitui_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int argc, char** argv);

void insere(char* nome_arquivo, FILE* arq_novo, FILE* archive, struct diretorio* v_diretorio[]);

void insere_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int num_arq, char** argv, int optind);

void atribui_permissoes(char* nome_arquivo, struct diretorio* arquivo);

void extrai(FILE* archive, char* nome_arquivo, struct diretorio* v_diretorio[]);

void extrai_arg(int argc, char* argv[], FILE* archive, struct diretorio* v_diretorio[]);

void exclui(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int id_arq);

void exclui_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int num_arq, char** argv, int optind);

void move(FILE* archive, char* nome_target, char* nome_arquivo, struct diretorio* v_diretorio[]);

#endif