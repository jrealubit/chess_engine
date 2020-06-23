/*
 * io.c is used to house the piece movement information for the
 * board for the chess engine. Conencts to chessEngine.h.
 */

#include "stdio.h"
#include "chessEngine.h"

char* printPos(const int pos) {
	static char posStr[3];
	int f = FilesBoard[pos];
	int r = RanksBoard[pos];
	sprintf(posStr, "%c%c", ('a'+f), ('1'+r));
	return posStr;
}

char* printMove(const int move) {
	static char moveStr[6];
	int ff = FilesBoard[FromBPos(move)];
	int rf = RanksBoard[FromBPos(move)];
	int ft = FilesBoard[ToBPos(move)];
	int rt = RanksBoard[ToBPos(move)];

	int promoted = Promoted(move);
	if (promoted) {
		char pieceChar = 'q';
		if (IsKnight(promoted)) {
			pieceChar = 'n';
		}
		else if (IsRookQueen(promoted) && !IsBishopQueen(promoted))  {
			pieceChar = 'r';
		}
		else if (!IsRookQueen(promoted) && IsBishopQueen(promoted))  {
			pieceChar = 'b';
		}
		sprintf(moveStr, "%c%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt), pieceChar);
	}
	else {
		sprintf(moveStr, "%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
	}
	return moveStr;
}

void printMoveList(const MoveListStruct* list) {
	int i = 0;
	int score = 0;
	int move = 0;
	printf("Move List:\n");

	for (i = 0; i < list->count; ++i) {
		move = list->moves[i].move;
		score = list->moves[i].score;
		printf("Move:%d > %s (score:%d)\n", i+1, printMove(move), score);
	}
	printf("MoveList Total %d Moves:\n\n", list->count);
}
