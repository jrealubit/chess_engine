#include "stdio.h"
#include "stdlib.h"
#include "chessEngine.h"

int main() {
	allInit();

	BoardStruct board[1];
	parseFenStr(START_FEN, board);
	printBoard(board);

	parseFenStr("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1", board);
	printBoard(board);
	return 0;
}
