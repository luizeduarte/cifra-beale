CC = gcc
CFLAGS = -std=c99 -Wall

all: vina++

vina++: main.o funcoes_principais.o manipula_dir.o manipula_conteudo.o
	$(CC) main.o funcoes_principais.o manipula_dir.o manipula_conteudo.o -o vina++

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o main.o

funcoes_principais.o: funcoes_principais.c
	$(CC) -c $(CFLAGS) funcoes_principais.c -o funcoes_principais.o

manipula_dir.o: manipula_dir.c
	$(CC) -c $(CFLAGS) manipula_dir.c -o manipula_dir.o

manipula_conteudo.o: manipula_conteudo.c
	$(CC) -c $(CFLAGS) manipula_conteudo.c -o manipula_conteudo.o

clean:
	rm -f *.o

purge:
	rm -f *.o vina++