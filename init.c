// init.c
// initialize file
#include "chessEngine.h"

// Global variables
int B120ToB64[BOARD_NUM];
int B64ToB120[64];

U64 setMask[64];
U64 clearMask[64];

void initBitMasks() {
	int i = 0;

	for (i = 0; i < 64; ++i) {
		setMask[i] = 0ULL;
		clearMask[i] = 0ULL;
	}

	for (i = 0; i < 64; ++i) {
		setMask[i] |= 1ULL << i;
		clearMask[i] |= ~setMask[i];
	}	
}

void initB120ToB64() {
	int i = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int b = A1;
	int b64 = 0;

	// initalize both board representation arrays
	for (i = 0; i < BOARD_NUM; i++) {
		B120ToB64[i] = 65;
	}

	for (i = 0; i < 64; ++i) {
		B64ToB120[i] = 120;
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
	initBitMasks();
	return;
}
