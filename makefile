CC = gcc

chess: main.o
	$(CC) -o chess main.o init.c

init.o: init.c
	$(CC) -c init.c

main.o: main.c
	$(CC) -c main.c
	
clean:
	rm -rf core *.o chess
