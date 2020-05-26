// init.c
// initialize file
#include "chessEngine.h"

// Global variables
int B120ToB64[BOARD_NUM];
int B64ToB120[64];

void initB120ToB64() {
	int idx = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int b = A1;
	int b64 = 0;

	// initalize both board representation arrays
	for (idx = 0; idx < BOARD_NUM; idx++) {
		B120ToB64[idx] = 65;
	}

	for (idx = 0; idx < 64; ++idx) {
		B64ToB120[idx] = 120;
	}

	// create the board exchange representation values
	for (rank = RANK_1; rank < RANK_8+1; ++rank) {
		for (file = FILE_A; file < FILE_H+1; ++file) {
			b = ((21+file) + (rank*10));
			B64ToB120[b64] = b;
			B120ToB64[b] = b64;
			b64++;
		}
	}
}

void allInit() {
	initB120ToB64();
	return;
}
