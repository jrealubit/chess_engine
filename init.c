// init.c
#include "chessEngine.h"

int B120ToB64[BOARD_NUM];
int B64ToB120[64];

U64 setMask[64];
U64 clearMask[64];

void initBitMasks() {
	int i = 0;

	for(i = 0; i < 64; i++) {
		setMask[i] = 0ULL;
		clearMask[i] = 0ULL;
	}

	for(i = 0; i < 64; i++) {
		setMask[i] |= (1ULL << i);
		clearMask[i] = ~setMask[i];
	}
}

void initSq120To64() {
	int i = 0;
	int f = FILE_A;
	int r = RANK_1;
	int b = A1;
	int b64 = 0;

	for(i = 0; i < BOARD_NUM; ++i) {
		B120ToB64[i] = 65;
	}

	for(i = 0; i < 64; ++i) {
		B64ToB120[i] = 120;
	}

	for(r = RANK_1; r <= RANK_8; ++r) {
		for(f = FILE_A; f <= FILE_H; ++f) {
			b = FR2SQ(f,r);
			B64ToB120[b64] = b;
			B120ToB64[b] = b64;
			b64++;
		}
	}
}

void allInit() {
	initSq120To64();
	initBitMasks();
	return;
}
