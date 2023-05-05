1. Abrindo arquivos.
        Além das funçṍes que envolvem listas, são usadas funções para 
abrir os arquivos – localizadas no módulo “abre_arquivos” –, 
verificando se foram passados como argumentos e abriram corretamente. 

2. As listas.
        A organização das listas foi feita através de uma lista principal, a 
qual possui nodos que guardam um caractere, um ponteiro para sua lista de 
chaves e o tamanho dela:
            struct nodo_caractere{
	            char caractere;	
	            int tamanho; 	
	            struct nodo_chave *chaves_head;		
                struct nodo_caractere *prox;
            };

    Dentre as funções que manipulam tais listas – localizadas no módulo 
“manipula_listas” –, está a “insere_lista”, a qual verifica se o 
caractere já está presente na lista de ponteiro chars_head, organizada em 
ordem crescente. Se o caractere for encontrado, não é necessário 
adicioná-lo, basta chamar a função “adiciona_chave”, que tem a função 
de criar um novo nodo, reconhecendo se o número deve ser inserido no começo 
ou fim da lista de chaves. 
    Outras funções implementadas foram as de criação de listas a partir de 
arquivos.  “gera_lista_livro”  lê cada palavra do arquivo do livro cifra 
através da função fscanf, e então adiciona sua primeira letra e o numero da 
chave pela função “insere_lista” explicitada anteriormente, sempre 
atualizando o numero da chave atual. Além disso, foi implementada também 
“gera_lista_arqchaves”, a qual transformará o arquivo de chaves em listas, 
utilizando, também a funcao fscanf e, então, verificando se o que foi lido 
deve ser inserido na lista de caracteres, ou lido como um numero e adicionado 
as listas de chaves. 

3. Codificar e decodificar, criando novos arquivos.
    As principais funções do programa são a “codifica” e 
“decodifica”. A primeira lê cada caractere do arquivo de mensagem original 
usando a função fgetc, escrevendo no arquivo de mensagem codificada -1 caso 
seja um espaço, -2 se for quebra de linha, ou, então, uma chave que 
corresponde a letra do texto através da função “seleciona_chaves”, a 
qual será detalhada mais à frente. Já a função “decodifica”, lê um 
número através da função fscanf, traduzindo os números para letras e 
espaços com a função “seleciona_caractere”. As funções de 
“seleciona” tem como objetivo buscar nas listas a chave ou caractere 
correspondente aos argumentos passados. 
    Por fim, temos a função de criar o arquivo de chaves, a qual imprime de 
forma formatada as listas geradas em um arquivo. 
