#include "stdio.h"
#include "stdlib.h"
#include "chessEngine.h"

int main() {
	allInit();
	int index = 0;
	U64 bitBoard = 0ULL;
	//
	//
	// SETBIT(bitBoard, 61);
	// PrintBitBoard(bitBoard);
	//
	//
	// CLRBIT(bitBoard, 61);
	// PrintBitBoard(bitBoard);

	bitBoard |= (1ULL << B64(D2));
	bitBoard |= (1ULL << B64(D3));
	bitBoard |= (1ULL << B64(D4));

	printf("\n");
	printBitBoard(bitBoard);

	int count = CNT(bitBoard);
	printf("Bit count: %d\n", count);

	int i = POP(&bitBoard);
	printf("Popped Index: %d\n", i);

	printBitBoard(bitBoard);

	count = CNT(bitBoard);
	printf("Bit count: %d\n", count);

	return 0;
}
