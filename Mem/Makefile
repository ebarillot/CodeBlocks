
INCLUDE= -I. -I../IndexedSeq

CC=gcc -g

.c.o:
	$(CC) $(INCLUDE) -c -o $*.o $*.c

Mem.o: Mem.h Mem.c


Mem: main.o Mem.o
	$(CC) -o Mem main.o Mem.o

Debug: Mem
