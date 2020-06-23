/*
 * validateff.c is used to house the validation function used on the
 * board for the chess engine. Conencts to chessEngine.h.
 */

#include "chessEngine.h"

int posOnBoard(const int pos) {
	return FilesBoard[pos]==OFFBOARD ? FALSE : TRUE;
}

int sideValid(const int side) {
	return (side==WHITE || side == BLACK) ? TRUE : FALSE;
}

int fileRankValid(const int fr) {
	return (fr >= FALSE && fr <= 7) ? TRUE : FALSE;
}

int pieceValidEmpty(const int piece) {
	return (piece >= EMPTY && piece <= bK) ? TRUE : FALSE;
}

int pieceValid(const int piece) {
	return (piece >= wP && piece <= bK) ? TRUE : FALSE;
}
