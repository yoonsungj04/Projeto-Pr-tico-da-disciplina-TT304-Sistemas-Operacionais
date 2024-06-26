all: threadManipularMatriz.o threadOperacoesMatriz.o
	gcc -o main main.c threadManipularMatriz.o threadOperacoesMatriz.o -lpthread

threadManipularMatriz.o: threadManipularMatriz.h 
	gcc -c threadManipularMatriz.c

threadOperacoesMatriz.o: threadOperacoesMatriz.h
	gcc -c threadOperacoesMatriz.c

clean:
	rm -rf *.o