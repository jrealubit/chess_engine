#include <stdio.h>
#include "chessEngine.h"

int main() {
	printf("%s\n", "Let's make a chess engine!");
	allInit(); // initialization function
	int i = 0;
	U64 bitBoard = 0ULL;

	// insertion test
	bitBoard |= (1ULL << B64ToB120[D2]);
	bitBoard |= (1ULL << B64ToB120[D3]);
	bitBoard |= (1ULL << B64ToB120[D4]);
	printBitBoard(bitBoard);

	int count = countBits(bitBoard);
	printf("Count: %d\n", count);

	return 0;
}
