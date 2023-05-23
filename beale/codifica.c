#include <stdio.h>
#include "codifica.h"
#include "manipula_lista.h"

void codifica(struct nodo_caractere* chars_head, FILE* f_mensagem_original, FILE* f_mensagem_codificada){
	char c;
	while ((c = fgetc(f_mensagem_original)) != EOF){	//enquanto o arquivo nao terminou, pega cada caractere
		if (c == ' ')
			fprintf(f_mensagem_codificada, "-1 ");	//se for espaco, codifica como -1 
		else if (c == '\n')
			fprintf(f_mensagem_codificada, "-2 ");	//se for quebra de linha, codifica como -2
		else
			fprintf(f_mensagem_codificada, "%d ", seleciona_chaves(chars_head, c)); 
			//seleciona uma chave que corresponde a caractere do texto
	}
}