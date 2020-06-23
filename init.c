/*
 * init.c is used to initialize the
 * board for the chess engine. Conencts to chessEngine.h.
 */

#include "stdio.h"
#include "stdlib.h"
#include "chessEngine.h"

// Macro to create a 64-bit pseudo-random hash key
#define RAND_64 ((U64)rand() | (U64)rand() << 15 | (U64)rand() << 30 | \
					(U64)rand() << 45 | ((U64)rand() & 0xf) << 60 )

int b120Tob64[BRD_POS_NUM];
int b64Tob120[64];

U64 setMask[64];
U64 clearMask[64];

U64 pieceKeys[13][120];
U64 sideKey;
U64 castleKeys[16];

int FilesBoard[BRD_POS_NUM];
int RanksBoard[BRD_POS_NUM];

// initialize the connection between the
// 120 board to 64 board representation
void initB120To64() {
	int i = 0;
	int f = FILE_A;
	int r = RANK_1;
	int pos = A1;
	int b64 = 0;

	for (i = 0; i < BRD_POS_NUM; ++i) {
		b120Tob64[i] = 65;
	}

	for (i = 0; i < 64; ++i) {
		b64Tob120[i] = 120;
	}

	for (r = RANK_1; r <= RANK_8; ++r) {
		for (f = FILE_A; f <= FILE_H; ++f) {
			pos = FR2SQ(f, r);
			b64Tob120[b64] = pos;
			b120Tob64[pos] = b64;
			b64++;
		}
	}
}

// initialize bit masks for each position
void initBitMasks() {
	int i = 0;
	for (i = 0; i < 64; ++i) {
		setMask[i] = 0ULL;
		clearMask[i] = 0ULL;
	}

	for (i = 0; i < 64; i++) {
		setMask[i] |= (1ULL << i);
		clearMask[i] = ~setMask[i];
	}
}

// function to hash each position on the board
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

// initialize the file and rank positions on the board
void initFileAndRank() {
	int i = 0;
	int f = FILE_A;
	int r = RANK_1;
	int pos = A1;
	int b64 = 0;

	for (i = 0; i < BRD_POS_NUM; ++i) {
		FilesBoard[i] = OFFBOARD;
		RanksBoard[i] = OFFBOARD;
	}

	for (r = RANK_1; r <= RANK_8; ++r) {
		for (f = FILE_A; f <= FILE_H; ++f) {
			pos = FR2SQ(f, r);
			FilesBoard[pos] = f;
			RanksBoard[pos] = r;
		}
	}
}

void allInit() {
	initB120To64();
	initBitMasks();
	initHashKeys();
	initFileAndRank();
	return;
}
