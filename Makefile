wish: main.o builtIn.o
	gcc -o wish main.o builtIn.o

main.o: main.c
	gcc -c main.c

builtIn.o: builtIn.c
	gcc -c builtIn.c
