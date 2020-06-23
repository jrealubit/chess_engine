/*
 * attack.c houses the piece attack logic for the
 * board for the chess engine. Conencts to chessEngine.h.
 */

#include "stdio.h"
#include "chessEngine.h"

// directions from the piece square where certain
// pieces may potentially pose a threat
const int kingDir[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
const int knightDir[8] = {-21, -19, -12, -8, 8, 12, 19, 21};
const int rookDir[4] = {-10, -1, 1, 10};
const int bishopDir[4] = {-11, -9, 9, 11};

// function to check if board positions is threatened
int posAttacked(const int pos, const int side, const BoardStruct *b) {
	int i, piece, t, dir;

	ASSERT(posOnBoard(pos));
	ASSERT(SideValid(side));
	ASSERT(CheckBoard(b));

	// check pawns
	if (side == WHITE) {
		if (b->pieces[pos-9] == wP || b->pieces[pos-11] == wP) {
			return TRUE;
		}
	} else {
		if (b->pieces[pos+9] == bP || b->pieces[pos+11] == bP) {
			return TRUE;
		}
	}

	// check kings
	for (i = 0; i < 8; ++i) {
		piece = b->pieces[pos+kingDir[i]];
		if (piece != OFFBOARD && IsKing(piece) && pieceColor[piece]==side) {
			return TRUE;
		}
	}

	// check knights
	for (i = 0; i < 8; ++i) {
		piece = b->pieces[pos+knightDir[i]];
		if (piece != OFFBOARD && IsKnight(piece) && pieceColor[piece]==side) {
			return TRUE;
		}
	}

	// check rooks and queens
	for (i = 0; i < 4; ++i) {
		dir = rookDir[i];
		t = pos + dir;
		piece = b->pieces[t];
		while (piece != OFFBOARD) {
			if (piece != EMPTY) {
				if (IsRookQueen(piece) && pieceColor[piece] == side) {
					return TRUE;
				}
				break;
			}
			t += dir;
			piece = b->pieces[t];
		}
	}

	// check bishops and queens
	for (i = 0; i < 4; ++i) {
		dir = bishopDir[i];
		t = pos + dir;
		piece = b->pieces[t];
		while (piece != OFFBOARD) {
			if (piece != EMPTY) {
				if (IsBishopQueen(piece) && pieceColor[piece] == side) {
					return TRUE;
				}
				break;
			}
			t += dir;
			piece = b->pieces[t];
		}
	}

	return FALSE;
}
