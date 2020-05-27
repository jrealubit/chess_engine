/*
* This file hosts all of the definitions for the chess engine.
* */

#ifndef CHESS_H
#define CHESS_H

#include "stdlib.h"

#define DEBUG
#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - Failed ", #n); \
printf("on %s ", __DATE__); \
printf("at %s\n", __TIME__ ); \
printf("in File %s ", __FILE__); \
printf("at Line %d\n", __LINE__); \
exit(-1); \
}
#endif

typedef unsigned long long U64;
#define NAME "Chess Engine 1.0"
#define BOARD_NUM 120
#define MAXGAMEMOVES 2048

enum {FALSE, TRUE};
enum {WHITE, BLACK, BOTH}; // chess colors

// board file and rank and enumeration for
// cell positions on board- w: white, b: black
enum {EMPTY, wP, wKN, wB, wR, wQ, wK, bP, bKN, bB, bR, bQ, bK};
enum {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE};
enum {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE};

// enumeration for the board
enum {
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
  A2 = 31, B2, C2, D2, E2, F2, G2, H2,
  A3 = 41, B3, C3, D3, E3, F3, G3, H3,
  A4 = 51, B4, C4, D4, E4, F4, G4, H4,
  A5 = 61, B5, C5, D5, E5, F5, G5, H5,
  A6 = 71, B6, C6, D6, E6, F6, G6, H6,
  A7 = 81, B7, C7, D7, E7, F7, G7, H7,
  A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ
};

// castling enum permissions
enum {WKC = 1, WQC = 2, BKC = 4, BQC = 8};

typedef struct {
	U64 posKey;

	int move;
	int castlePermission;
	int enPas;
	int fiftyMove;
} UndoStruct;

// board structure
// note the evaluation sets are done by half-moves
typedef struct {
	U64 pawns[3];
	U64 posKey;

	int side;
	int enPas;
	int fiftyMove;
	int ply;
	int histPly;
	int castlePermission;

	int chessPieces[BOARD_NUM];
	int kingSq[2];
	int pieceNum[13];
	int bigPieces[3]; // non-pawn pieces
	int majorPieces[3]; // rooks and queens
	int minorPieces[3]; // bishops and knights
	int pieceList[13][10];

	// board history for retraction
	UndoStruct boardHistory[MAXGAMEMOVES];
} BoardStruct;

// Macros
#define FR2SQ(f, r) ((21+f)+(r*10))
#define B64(b120) B120ToB64[b120]
#define POP(b) popBit(b)
#define CNT(b) countBits(b)
#define SETBIT(bb, sq) (bb &= clearMask[sq])
#define CLRBIT(bb, sq) (bb |= setMask[sq])

// Global variables
extern int B120ToB64[BOARD_NUM];
extern int B64ToB120[64];
extern U64 setMask[64];
extern U64 clearMasj[64];

// Functions
extern void allInit();
extern void printBitBoard(U64 bb);
extern int countBits(U64 b);
extern int popBit(U64 *bb);

#endif
