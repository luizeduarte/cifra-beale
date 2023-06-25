#ifndef FUNCOES_PRINCIPAIS_H
#define FUNCOES_PRINCIPAIS_H
#include "manipula_conteudo.h"
#include "manipula_dir.h"

int verifica_sub(time_t ultima_modificacao, char* nome_arquivo);

struct diretorio** substitui_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int argc, char** argv);

struct diretorio** insere(char* nome_arquivo, FILE* arq_novo, FILE* archive, struct diretorio* v_diretorio[], struct conteudo* info_conteudo);

struct diretorio** insere_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int num_arq, char** argv, int optind);

void extrai(FILE* archive, char* nome_arquivo, struct diretorio* arquivo);

void extrai_arg(int argc, char* argv[], FILE* archive, struct diretorio* v_diretorio[]);

void exclui(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int id_arq, struct conteudo* info_conteudo);

void exclui_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int num_arq, char** argv, int optind);

void move(FILE* archive, char* nome_target, char* nome_arquivo, struct diretorio* v_diretorio[]);

void lista_arq(FILE* archive, struct diretorio* v_diretorio[]);

#endif
