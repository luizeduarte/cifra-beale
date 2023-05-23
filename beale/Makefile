CC = gcc
CFLAGS = -std=c99 -Wall

all: beale

beale: beale.o abre_arquivos.o codifica.o decodifica.o manipula_lista.o cria_arq_chaves.o
	$(CC) beale.o abre_arquivos.o codifica.o decodifica.o manipula_lista.o cria_arq_chaves.o -o beale

beale.o: beale.c
	$(CC) -c $(CFLAGS) beale.c -o beale.o

abre_arquivos.o: abre_arquivos.c
	$(CC) -c $(CFLAGS) abre_arquivos.c -o abre_arquivos.o

codifica.o: codifica.c
	$(CC) -c $(CFLAGS) codifica.c -o codifica.o

decodifica.o: decodifica.c
	$(CC) -c $(CFLAGS) decodifica.c -o decodifica.o

manipula_lista.o: manipula_lista.c
	$(CC) -c $(CFLAGS) manipula_lista.c -o manipula_lista.o

cria_arq_chaves.o: cria_arq_chaves.c
	$(CC) -c $(CFLAGS) cria_arq_chaves.c -o cria_arq_chaves.o

clean:
	rm -f *.o

purge:
	rm -f *.o beale