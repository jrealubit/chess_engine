/*
* This file hosts all of the definitions for the chess engine.
* All .c code files in connect to this directory. I think naming
* the program Sinanju is pretty cool.
* */

#ifndef CHESS_H
#define CHESS_H

#include "stdlib.h"

#ifndef DEBUG
#define DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - Failed",#n); \
printf("On %s ",__DATE__); \
printf("At %s ",__TIME__); \
printf("In File %s ",__FILE__); \
printf("At Line %d\n",__LINE__); \
exit(1);
}
#endif

typedef unsigned long long U64; // 64-bti data type

#define NAME "Shinanju 1.0"
#define BRD_POS_NUM 120
#define MAXGAMEMOVES 2048
#define MAXPOSITIONMOVES 256

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

enum {FALSE, TRUE};
enum {WHITE, BLACK, BOTH}; // chess colors
enum {WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8}; // castling permissions
enum {EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK}; // pieces
enum {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE};
enum {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE};

// chess board enumeration
enum {
  A1 = 21, B1, C1, D1, E1, F1, G1, H1,
  A2 = 31, B2, C2, D2, E2, F2, G2, H2,
  A3 = 41, B3, C3, D3, E3, F3, G3, H3,
  A4 = 51, B4, C4, D4, E4, F4, G4, H4,
  A5 = 61, B5, C5, D5, E5, F5, G5, H5,
  A6 = 71, B6, C6, D6, E6, F6, G6, H6,
  A7 = 81, B7, C7, D7, E7, F7, G7, H7,
  A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
};

// Data structures
typedef struct {
	int move;
	int score;
} MoveStruct;

typedef struct {
	MoveStruct moves[MAXPOSITIONMOVES];
	int count;
} MoveListStruct;

typedef struct {
  U64 posKey;
  int move;
} PVEntryStruct;

typedef struct {
  PVEntryStruct* pTable;
  int numEntries;
} PVTableStruct;

typedef struct {
  U64 posKey;
	int move;
	int castlePermission;
	int enPas;
	int fiftyMove;
} UndoStruct;

// board struct: evaluation is done in half moves
typedef struct {
  U64 posKey;
  U64 pawns[3];

	int pieces[BRD_POS_NUM];
	int KingSq[2];
	int side;
	int enPas;
	int fiftyMove;
	int ply;
	int hisPly;
	int castlePermission;
	int pieceNum[13];
	int bigPiece[2]; // non-pawn pieces
	int majorPiece[2]; // rooks and queens
	int minorPiece[2]; // bishops and knights
	int material[2];
  int pieceList[13][10];

  // board history for retraction
	UndoStruct history[MAXGAMEMOVES];

  // principal variation table
  PVTableStruct pvTable[1];
} BoardStruct;

// General Macros
#define FR2SQ(f, r) ((21+f)+(r*10))
#define B64(b120) b120Tob64[b120]
#define B120(b64) b64Tob120[b64]
#define POP(b) popBit(b)
#define CNT(b) countBits(b)
#define CLRBIT(bb, pos) (bb &= clearMask[pos])
#define SETBIT(bb, pos) (bb |= setMask[pos])
#define IsBishopQueen(p) (pieceBishopQueen[p])
#define IsRookQueen(p) (pieceRookQueen[p])
#define IsKnight(p) (pieceKnight[p])
#define IsKing(p) (pieceKing[p])

// Game Move Macros
/*
* 28-bit move information structure:
* 0000 0000 0000 0000 0000 0111 1111 : from 0x7F
* 0000 0000 0000 0011 1111 1000 0000 : to >> 7, 0x7F
* 0000 0000 0011 1100 0000 0000 0000 : captured >> 14, 0xF
* 0000 0000 0100 0000 0000 0000 0000 : en passant t0x40000
* 0000 0000 1000 0000 0000 0000 0000 : pawn start? 0x80000
* 0000 1111 0000 0000 0000 0000 0000 : piece promotion >> 20, 0xF
* 0001 0000 0000 0000 0000 0000 0000 : castling permissions 0x1000000
*/

#define FromBPos(m) (m & 0x7F)
#define ToBPos(m) ((m>>7) & 0x7F)
#define Captured(m) ((m>>14) & 0xF)
#define Promoted(m) ((m>>20) & 0xF)

#define MFLAGEP 0x40000 // en passant flag
#define MFLAGPS 0x80000 // pawn start flag
#define MFLAGCP 0x1000000 // castling permissions
#define MFLAGCAP 0x7C000 // capture flag
#define MFLAGPROM 0xF00000 // promotion flag


// Global Variables
extern int b120Tob64[BRD_POS_NUM];
extern int b64Tob120[64];
extern U64 setMask[64];
extern U64 clearMask[64];
extern U64 pieceKeys[13][120];
extern U64 sideKey;
extern U64 castleKeys[16];
extern char pieceChar[];
extern char sideChar[];
extern char fileChar[];
extern char rankChar[];

extern int pieceBig[13];
extern int pieceMajor[13];
extern int pieceMinor[13];
extern int pieceValue[13];
extern int pieceColor[13];
extern int piecePawn[13];

extern int FilesBoard[BRD_POS_NUM];
extern int RanksBoard[BRD_POS_NUM];

extern int pieceKnight[13];
extern int pieceKing[13];
extern int pieceRookQueen[13];
extern int pieceBishopQueen[13];
extern int pieceSlides[13];

// Functions
// init.c
extern void allInit();

// bitboards.c
extern int countBits(U64 b);
extern int popBit(U64 *bb);
extern void printBitBoard(U64 bb);

// hashkeys.c
extern U64 generatePosKey(const BoardStruct *b);

// board.c
extern void resetBoard(BoardStruct *b);
extern int parseFenStr(char *fen_str, BoardStruct *b);
extern void printBoard(const BoardStruct *b);
extern void updateMaterialLists(BoardStruct *b);
extern int checkBoard(const BoardStruct *b);

// attack.c
extern int posAttacked(const int pos, const int side, const BoardStruct *b);

// io.c
extern char* printSq(const int pos);
extern char* printMove(const int move);
extern void printMoveList(const MoveListStruct *list);

// movegen.c
extern void generateAllMoves(const BoardStruct *b, MoveListStruct *list);

//validate.c
extern int posOnBoard(const int pos);
extern int sideValid(const int side);
extern int fileRankValid(const int fr);
extern int pieceValidEmpty(const int piece);
extern int pieceValid(const int piece);

// makemove.c
extern int makeMove(BoardStruct *b, int move);
extern void takeMove(BoardStruct *b);

// perft.c
extern void perftTest(int depth, BoardStruct *b);

// pvtable.c
extern void initTable(PVTableStruct* table);
extern void clearTable(PVTableStruct* table);

#endif
