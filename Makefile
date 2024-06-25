all: manipuladorMatriz.o, operacoesMatriz.o, operacoesThread.o
	gcc -o main main.c manipuladorMatriz.o operacoesMatriz.o operacoesThread.o -lpthread

manipuladorMatriz.o:	manipuladorMatriz.h
	gcc -c manipuladorMatriz.c

operacoesMatriz.o: operacoesMatriz.h
	gcc -c operacoesMatriz.c

operacoesThread.o: operacoesThread.h
	gcc -c operacoesThread.c

clean:
	rm -rf *.o

run:
	./main 4 1000 arquivos_matrizes/matrizA.dat arquivos_matrizes/matrizB.dat arquivos_matrizes/matrizC.dat arquivos_matrizes/matrizD.dat arquivos_matrizes/matrizE.dat

