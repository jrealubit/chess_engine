CC = gcc

build: main.o init.o bitboards.o
	$(CC) -o chess main.o init.o bitboards.o

bitboards.o: bitboards.c
	$(CC) -c bitboards.c

init.o: init.c
	$(CC) -c init.c

main.o: main.c
	$(CC) -c main.c
	
clean:
	rm -rf core *.o chess
