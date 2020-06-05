/*
 * init.c is used to initialize the
 * board for the chess engine. Conencts to chessEngine.h.
 */

#include "stdio.h"
#include "stdlib.h"
#include "chessEngine.h"

// Macro to create a 64-bit psued0-random hask key
#define RAND_64 ((U64)rand() | (U64)rand() << 15 | (U64)rand() << 30 | \
								(U64)rand() << 45 | ((U64)rand() & 0xf) << 60)

int B120ToB64[BOARD_NUM];
int B64ToB120[64];

int FileOnBoard[BOARD_NUM];
int RankOnBoard[BOARD_NUM];

U64 setMask[64];
U64 clearMask[64];

U64 pieceKeys[13][120];
U64 sideKey;
U64 castleKeys[16];

void initB120To64() {
	int i = 0;
	int f = FILE_A;
	int r = RANK_1;
	int b = A1;
	int b64 = 0;

	for (i = 0; i < BOARD_NUM; ++i) {
		B120ToB64[i] = 65;
	}

	for (i = 0; i < 64; ++i) {
		B64ToB120[i] = 120;
	}

	for (r = RANK_1; r <= RANK_8; ++r) {
		for (f = FILE_A; f <= FILE_H; ++f) {
			b = FR2SQ(f,r);
			B64ToB120[b64] = b;
			B120ToB64[b] = b64;
			b64++;
		}
	}
}

void initBitMasks() {
	int i = 0;

	for (i = 0; i < 64; i++) {
		setMask[i] = 0ULL;
		clearMask[i] = 0ULL;
	}

	for (i = 0; i < 64; i++) {
		setMask[i] |= (1ULL << i);
		clearMask[i] = ~setMask[i];
	}
}

void initHashKeys() {
	int i = 0;
	int j = 0;

	// generate position keys for each slot in the array
	for (i = 0; i < 13; ++i) {
		for (j = 0; j < 120; ++j) {
			pieceKeys[i][j] = RAND_64;
		}
	}

	// generate one for the sides and the castling
	sideKey = RAND_64;
	for (i = 0; i < 16; ++i) {
		castleKeys[i] = RAND_64;
	}
}

// function to initialize file and rank on board
void initFileAndRank() {
	int i = 0;
	int f = FILE_A;
	int r = RANK_1;
	int b = A1;
	int b64 = 0;

	for (i = 0; i < BOARD_NUM; ++i) {
		FileOnBoard[i] = OFFBOARD;
		RankOnBoard[i] = OFFBOARD;
	}

	for (r = RANK_1; r <= RANK_8; ++r) {
		for (f = FILE_A; f <= FILE_H; ++f) {
			b = FR2SQ(f, r);
			FileOnBoard[b] = f;
			RankOnBoard[b] = r;
		}
	}

	// printf("Files on Board:\n");
	// for (i = 0; i < BOARD_NUM; ++i) {
	// 	if (i%10 == 0 && i != 0) {
	// 		printf("\n");
	// 	}
	// 	printf("%4d ", FileOnBoard[i]);
	// }
	//
	// printf("\nRank on Board:\n");
	// for (i = 0; i < BOARD_NUM; ++i) {
	// 	if (i%10 == 0 && i != 0) {
	// 		printf("\n");
	// 	}
	// 	printf("%4d ", RankOnBoard[i]);
	// }
}

void allInit() {
	initB120To64();
	initBitMasks();
	initHashKeys();
	initFileAndRank();
	return;
}
