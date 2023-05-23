#ifndef DECODIFICA_H_
#define DECODIFICA_H_
#include "manipula_lista.h"
#include <stdio.h>
#define ESPACO -1
#define QUEBRA_LINHA -2
#define CARACTERE_FALTANDO -3

void decodifica(struct nodo_caractere* chars_head, FILE* f_mensagem_codificada, FILE* f_mensagem_decodificada);
#endif