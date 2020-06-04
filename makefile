CC = gcc

build: main.o init.o bitboards.o hashkeys.o board.o data.o attack.o io.o
	$(CC) -o chess main.o init.o bitboards.o hashkeys.o board.o data.o attack.o io.o

io.o: io.c
	$(CC) -c io.c

attack.o: attack.c
	$(CC) -c attack.c

data.o: data.c
	$(CC) -c data.c

board.o: board.c
	$(CC) -c board.c

hashkeys.o: hashkeys.c
	$(CC) -c hashkeys.c

bitboards.o: bitboards.c
	$(CC) -c bitboards.c

init.o: init.c
	$(CC) -c init.c

main.o: main.c
	$(CC) -c main.c

clean:
	rm -rf core *.o chess
