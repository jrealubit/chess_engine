CC = gcc

chess: main.o
	$(CC) -o chess main.o
	
main.o: main.c
	$(CC) -c main.c
	
clean:
	rm -rf core *.o chess
