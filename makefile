task1: task1.o
	gcc -g -m32 -Wall -o hexeditplus task1.o

task1.o: task1.c
	gcc -g -Wall -ansi -c -m32 -o task1.o task1.c

clean: 
	rm -f *.o hexeditplus
