/*
 * movegenerator.c is used to house the piece movement logic
 * of the board for the chess engine. Conencts to chessEngine.h.
 */

#include "stdio.h"
#include "chessEngine.h"

#define MOVE(f, t, cp, pro, fl) ((f) | ((t) << 7) | ((cp) << 14) | ((pro) << 20) | (fl))
#define POSOFFBOARD(pos) (FilesBoard[(pos)] == OFFBOARD)

// sliding pieces
const int slidePieceList[8] = {wB, wR, wQ, 0, bB, bR, bQ, 0};
const int slideLoopIndex[2] = {0, 4};

// non-sliding pieces
const int nonSlidePieceList[6] = {wN, wK, 0, bN, bK, 0};
const int nonSlideLoopIndex[2] = {0, 3};

// number of directions for each piece
const int numDir[13] = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};

// list of board positions each piece can move to
// pawns are considered as 0 all across the board
// because there are dedicated move functions for pawns
const int pieceDir[13][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0}, // empty
  {0, 0, 0, 0, 0, 0, 0, 0}, // white pawn
  {-21, -19, -12, -8, 8, 12, 19, 21}, // white knight
  {-11, -9, 9, 11, 0, 0, 0, 0}, // white bishop
  {-10, -1, 1, 10, 0, 0, 0, 0}, // white rook
  {-11, -10, -9, -1, 1, 9, 10, 11}, // white queen
  {-11, -10, -9, -1, 1, 9, 10, 11}, // white king
  {0, 0, 0, 0, 0, 0, 0, 0}, // black pawn
  {-21, -19, -12, -8, 8, 12, 19, 21}, // black knight
  {-11, -9, 9, 11, 0, 0, 0, 0}, // black bishop
  {-10, -1, 1, 10, 0, 0, 0, 0}, // black rook
  {-11, -10, -9, -1, 1, 9, 10, 11}, // black queen
  {-11, -10, -9, -1, 1, 9, 10, 11}, // black king
};

// function to add a quiet move to the list
static void addQuietMove(const BoardStruct *b, int move, MoveListStruct *list) {
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
}

// function to add a capture move to the list
static void addCaptureMove(const BoardStruct *b, int move, MoveListStruct *list) {
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
}

// function to add an en passant move to the list
static void addEnPassantMove(const BoardStruct *b, int move, MoveListStruct *list) {
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
}

static void addWPMove(const BoardStruct *b, const int from, const int to,
  MoveListStruct *list) {
	ASSERT(posOnBoard(from));
	ASSERT(posOnBoard(to));

	if (RanksBoard[from] == RANK_7) {
		addQuietMove(b, MOVE(from, to, EMPTY, wQ, 0), list);
		addQuietMove(b, MOVE(from, to, EMPTY, wR, 0), list);
		addQuietMove(b, MOVE(from, to, EMPTY, wB, 0), list);
		addQuietMove(b, MOVE(from, to, EMPTY, wN, 0), list);
	}
  else {
		addQuietMove(b, MOVE(from, to, EMPTY, EMPTY, 0), list);
	}
}

static void addWPCapMove(const BoardStruct *b, const int from, const int to,
  const int cap, MoveListStruct *list) {
	ASSERT(PieceValidEmpty(cap));
	ASSERT(posOnBoard(from));
	ASSERT(posOnBoard(to));

	if (RanksBoard[from] == RANK_7) {
		addCaptureMove(b, MOVE(from, to, cap, wQ, 0), list);
		addCaptureMove(b, MOVE(from, to, cap, wR, 0), list);
		addCaptureMove(b, MOVE(from, to, cap, wB, 0), list);
		addCaptureMove(b, MOVE(from, to, cap, wN, 0), list);
	}
  else {
		addCaptureMove(b, MOVE(from, to, cap, EMPTY, 0), list);
	}
}

static void addBPMove( const BoardStruct *b, const int from, const int to,
  MoveListStruct *list ) {

	ASSERT(posOnBoard(from));
	ASSERT(posOnBoard(to));

	if (RanksBoard[from] == RANK_2) {
		addQuietMove(b, MOVE(from, to, EMPTY, bQ, 0), list);
		addQuietMove(b, MOVE(from, to, EMPTY, bR, 0), list);
		addQuietMove(b, MOVE(from, to, EMPTY, bB, 0), list);
		addQuietMove(b, MOVE(from, to, EMPTY, bN, 0), list);
	}
  else {
		addQuietMove(b, MOVE(from, to, EMPTY, EMPTY, 0), list);
	}
}

static void addBPCapMove(const BoardStruct *b, const int from, const int to,
  const int cap, MoveListStruct *list) {
	ASSERT(PieceValidEmpty(cap));
	ASSERT(posOnBoard(from));
	ASSERT(posOnBoard(to));

	if (RanksBoard[from] == RANK_2) {
		addCaptureMove(b, MOVE(from, to, cap, bQ, 0), list);
		addCaptureMove(b, MOVE(from, to, cap, bR, 0), list);
		addCaptureMove(b, MOVE(from, to, cap, bB, 0), list);
		addCaptureMove(b, MOVE(from, to, cap, bN, 0), list);
	}
  else {
		addCaptureMove(b, MOVE(from, to, cap, EMPTY, 0), list);
	}
}

void generateAllMoves(const BoardStruct *b, MoveListStruct *list) {
	ASSERT(CheckBoard(b));

	int piece = EMPTY;
	int side = b->side;
	int pos = 0;
  int temp = 0;
	int pieceNum = 0;
	int dir = 0;
	int i = 0;
	int index = 0;
  list->count = 0;

	if (side == WHITE) {
		for (pieceNum = 0; pieceNum < b->pieceNum[wP]; ++pieceNum) {
			pos = b->pieceList[wP][pieceNum];
			ASSERT(posOnBoard(pos));

			if (b->pieces[pos+10] == EMPTY) {
				addWPMove(b, pos, pos+10, list);
				if (RanksBoard[pos] == RANK_2 && b->pieces[pos+20] == EMPTY) {
					addQuietMove(b, MOVE(pos, (pos+20), EMPTY, EMPTY, MFLAGPS), list);
				}
			}

			if (!POSOFFBOARD(pos+9) && pieceColor[b->pieces[pos+9]] == BLACK) {
				addWPCapMove(b, pos, pos+9, b->pieces[pos+9], list);
			}
			if (!POSOFFBOARD(pos+11) && pieceColor[b->pieces[pos+11]] == BLACK) {
				addWPCapMove(b, pos, pos+11, b->pieces[pos+11], list);
			}

			if (pos+9 == b->enPas) {
				addCaptureMove(b, MOVE(pos, pos+9, EMPTY, EMPTY, MFLAGEP), list);
			}
			if (pos+11 == b->enPas) {
				addCaptureMove(b, MOVE(pos, pos+11, EMPTY, EMPTY, MFLAGEP), list);
			}
		}

		if (b->castlePermission & WKCA) {
			if (b->pieces[F1] == EMPTY && b->pieces[G1] == EMPTY) {
				if (!posAttacked(E1, BLACK, b) && !posAttacked(F1, BLACK, b) ) {
					addQuietMove(b, MOVE(E1, G1, EMPTY, EMPTY, MFLAGCP), list);
				}
			}
		}

		if (b->castlePermission & WQCA) {
			if (b->pieces[D1] == EMPTY && b->pieces[C1] == EMPTY && b->pieces[B1] == EMPTY) {
				if (!posAttacked(E1, BLACK, b) && !posAttacked(D1, BLACK, b) ) {
					addQuietMove(b, MOVE(E1, C1, EMPTY, EMPTY, MFLAGCP), list);
				}
			}
		}

	}
  else {
		for (pieceNum = 0; pieceNum < b->pieceNum[bP]; ++pieceNum) {
			pos = b->pieceList[bP][pieceNum];
			ASSERT(posOnBoard(pos));

			if (b->pieces[pos-10] == EMPTY) {
				addBPMove(b, pos, pos-10, list);
				if (RanksBoard[pos] == RANK_7 && b->pieces[pos-20] == EMPTY) {
					addQuietMove(b, MOVE(pos, (pos-20), EMPTY, EMPTY, MFLAGPS), list);
				}
			}
			if (!POSOFFBOARD(pos-9) && pieceColor[b->pieces[pos-9]] == WHITE) {
				addBPCapMove(b, pos, pos-9, b->pieces[pos-9], list);
			}
			if (!POSOFFBOARD(pos-11) && pieceColor[b->pieces[pos-11]] == WHITE) {
				addBPCapMove(b, pos, pos-11, b->pieces[pos-11], list);
			}
			if (pos-9 == b->enPas) {
				addCaptureMove(b, MOVE(pos, pos-9, EMPTY, EMPTY, MFLAGEP), list);
			}
			if (pos-11 == b->enPas) {
				addCaptureMove(b, MOVE(pos, pos-11, EMPTY, EMPTY, MFLAGEP), list);
			}
		}

		// castling
		if (b->castlePermission &  BKCA) {
			if (b->pieces[F8] == EMPTY && b->pieces[G8] == EMPTY) {
				if (!posAttacked(E8, WHITE, b) && !posAttacked(F8, WHITE, b) ) {
					addQuietMove(b, MOVE(E8, G8, EMPTY, EMPTY, MFLAGCP), list);
				}
			}
		}

		if (b->castlePermission &  BQCA) {
			if (b->pieces[D8] == EMPTY && b->pieces[C8] == EMPTY && b->pieces[B8] == EMPTY) {
				if (!posAttacked(E8, WHITE, b) && !posAttacked(D8, WHITE, b) ) {
					addQuietMove(b, MOVE(E8, C8, EMPTY, EMPTY, MFLAGCP), list);
				}
			}
		}
	}

	// slide pieces
	index = slideLoopIndex[side];
	piece = slidePieceList[index++];
	while (piece != 0) {
		ASSERT(PieceValid(piece));

		for (pieceNum = 0; pieceNum < b->pieceNum[piece]; ++pieceNum) {
			pos = b->pieceList[piece][pieceNum];
			ASSERT(posOnBoard(pos));

			for (i = 0; i < numDir[piece]; ++i) {
				dir = pieceDir[piece][i];
				temp = pos+ dir;

				while (!POSOFFBOARD(temp)) {
					if (b->pieces[temp] != EMPTY) {
						if ( pieceColor[b->pieces[temp]] == side ^ 1) {
							addCaptureMove(b, MOVE(pos, temp, b->pieces[temp], EMPTY, 0), list);
						}
						break;
					}
					addQuietMove(b, MOVE(pos, temp, EMPTY, EMPTY, 0), list);
					temp += dir;
				}
			}
		}
		piece = slidePieceList[index++];
	}

	// non-slide pieces 
	index = nonSlideLoopIndex[side];
	piece = nonSlidePieceList[index++];

	while (piece != 0) {
		ASSERT(PieceValid(piece));

		for (pieceNum = 0; pieceNum < b->pieceNum[piece]; ++pieceNum) {
			pos = b->pieceList[piece][pieceNum];
			ASSERT(posOnBoard(pos));

			for (i = 0; i < numDir[piece]; ++i) {
				dir = pieceDir[piece][i];
				temp = pos+ dir;

				if (POSOFFBOARD(temp)) {
					continue;
				}

				// BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
				if (b->pieces[temp] != EMPTY) {
					if (pieceColor[b->pieces[temp]] == side ^ 1) {
						addCaptureMove(b, MOVE(pos, temp, b->pieces[temp], EMPTY, 0), list);
					}
					continue;
				}
				addQuietMove(b, MOVE(pos, temp, EMPTY, EMPTY, 0), list);
			}
		}
		piece = nonSlidePieceList[index++];
	}
}
