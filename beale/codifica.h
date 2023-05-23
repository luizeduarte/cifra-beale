#ifndef CODIFICA_H_
#define CODIFICA_H_
#include <stdio.h>
#include "manipula_lista.h"

void codifica(struct nodo_caractere* chars_head, FILE* f_mensagem_original, FILE* f_mensagem_codificada);
#endif