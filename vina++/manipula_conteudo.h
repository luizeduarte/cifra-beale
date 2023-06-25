#ifndef MANIPULA_CONTEUDO_H
#define MANIPULA_CONTEUDO_H
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "manipula_dir.h"
#define MAX_STRING 1024

struct conteudo* conteudo(FILE* archive);

void copia_texto(FILE* arq_le, FILE* arq_escreve, long long int tam_texto);

void att_info_conteudo(FILE* archive, struct conteudo* info_conteudo);

void move_conteudo(FILE* archive, struct diretorio* diretorio, long long int diretorio_pos);

#endif