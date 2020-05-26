#include <stdio.h>
#include "chessEngine.h"

int main() {
	printf("%s\n", "Let's make a chess engine!");
	allInit(); // initialization function

	int idx = 0;
	for (idx = 0; idx < BOARD_NUM; ++idx) {
		if (idx%10 == 0) {
			printf("\n");
		}
		printf("%5d", B120ToB64[idx]);
	}

	printf("\n\n");

	for (idx = 0; idx < 64; ++idx) {
		if (idx%8 == 0) {
			printf("\n");
		}
		printf("%5d", B64ToB120[idx]);
	}

	printf("\n\n");

	// printf("%ld\n", sizeof(B120ToB64)/sizeof(B120ToB64[0]));
	return 0;
}
