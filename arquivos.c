#include <stdio.h>

int main(int argc, char *argv[]) {
	int opt;
	int flag_codificar = 0;
	int flag_decodificar = 0;
	char *livro_cifra = NULL;
	char *mensagem_original = NULL;
	char *mensagem_codificada = NULL;
	char *arquivo_chaves = NULL;
	char *mensagem_decodificada = NULL;
	char *opcoes = "e:d:b:m:o:c:i:";

	//processa os argumentos da linha de comando
	while ((opt = getopt(argc, argv, opcoes)) != -1) {
		switch (opt) {
			case 'e':
				flag_codificar = 1;
				break;
			case 'd':
				flag_decodificar = 1;
				break;
			case 'b':
				livro_cifra = optarg;
				break;
			case 'm':
				mensagem_original = optarg;
				break;
			case 'o':
				mensagem_codificada = optarg;
				break;
			case 'c':
				arquivo_chaves = optarg;
				break;
			case 'i':
				mensagem_decodificada = optarg;
				break;
			//avisa se passou a especificacao errada
			default:
				fprintf(stderr, "opcao invalida: %c\n", optopt);
				return 1;
		}
	}

	if (flag_codificar == 1) {

		//checa se passou todos os argumentos necessarios
		if (livro_cifra == NULL || mensagem_original == NULL || mensagem_codificada == NULL || arquivo_chaves == NULL) {
		fprintf(stderr, "Opções faltando\n");
		return 1;
		}

        // Abre o arquivo do livro cifra
        FILE *f_livro = fopen(livro_cifra, "r");
        if (f_livro == NULL) {
            fprintf(stderr, "Erro ao abrir o arquivo do livro cifra\n");
            return 1;
        }

        // Lê o conteúdo do livro cifra
        char livro[TAM_LIVRO];
        size_t len_livro = fread(livro, 1, TAM_LIVRO, f_livro);
        fclose(f_livro);

        // Abre o arquivo da mensagem original
        FILE *f_mensagem = fopen(mensagem_original, "r");
        if (f_mensagem == NULL) {
            fprintf(stderr, "Erro ao abrir o arquivo da mensagem original\n");
            return 1;
        }

        // Lê o conteúdo da mensagem original
        char mensagem[TAM_LIVRO];
        size_t len_mensagem = fread(mensagem, 1, TAM_LIVRO, f_mensagem);
        fclose(f_mensagem);
    }
}