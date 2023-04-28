#define MAX_STRING 100

//struct que compoe a fila de chaves
struct nodo_chave{
	int num_chave;
	struct nodo_chave *prox;
};

//struct que compoe a fila de caracteres presentes no livro cifra
struct nodo_caractere{
	char caractere;	//guarda o caractere que possui a lista de chaves
	int tamanho; 	//guarda o tamanho da lista de chaves do caractere
	struct nodo_chave *chaves_head;	//ponteiro para a lista de chaves do caractere
	struct nodo_caractere *prox;
};

//FUNCOES QUE MANIPULAM A LISTA
struct nodo_chave* aloca_chave(int num_chave);
struct nodo_caractere* aloca_caractere(char caractere);
void adiciona_chave(int num_chave, struct nodo_caractere* aux);
struct nodo_caractere* insere_lista(struct nodo_caractere* chars_head, int num_chave, char caractere);
int seleciona_chaves(struct nodo_caractere* chars_head, char caractere);
char seleciona_caractere(struct nodo_caractere* chars_head, int chave);
void desaloca_lista(struct nodo_caractere* chars_head);
//FUNCOES DE TRANSFORMAR ARQUIVOS EM LISTAS
struct nodo_caractere* gera_lista_livro(struct nodo_caractere* chars_head, FILE* f_livro);
struct nodo_caractere* gera_lista_arqchaves(FILE* f_chaves);
//FUNCOES QUE CRIAM ARQUIVOS
void codifica(struct nodo_caractere* chars_head, FILE* f_mensagem_original, FILE* f_mensagem_codificada);
void decodifica(struct nodo_caractere* chars_head, FILE* f_mensagem_codificada, FILE* f_mensagem_decodificada);
void cria_arq_chaves(struct nodo_caractere* chars_head, FILE* f_chaves);
//FUNCOES PARA OS ARQUIVOS
FILE* abre_leitura(char* arquivo);
FILE* abre_escrita(char* arquivo)