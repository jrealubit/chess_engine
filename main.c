#include "stdio.h"
#include "stdlib.h"
#include "chessEngine.h"

int main() {
	allInit();

	BoardStruct board[1];
	// parseFenStr(START_FEN, board);
	// printBoard(board);
	//
	// parseFenStr("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1", board);
	// printBoard(board);
	//
	// parseFenStr("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2", board);
	// printBoard(board);

	int move = 0;
	int from = A2;
	int to = H7;
	int cap = wR;
	int prom = bK;

	move = ((from) | (to<<7) | (cap<<14) | (prom<<20));
	printf("from:%d to:%d cap:%d prom:%d\n",
		FromBPos(move), ToBPos(move), Captured(move),
		Promoted(move));

	printf("Algebraic from:%s\n", printBPos(from));
	printf("Algebraic to:%s\n", printBPos(to));
	printf("Algebraic move:%s\n", printMove(move));

	return 0;
}
