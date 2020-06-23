// vice.c

#include <stdio.h>
#include <stdlib.h>
#include "chessEngine.h"

int main() {
	allInit();

	BoardStruct board[1];
	MoveListStruct list[1];

	char fen_str[] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
	parseFenStr(fen_str, board);
	perftTest(5, board);

	printf("Move count value in the board is %d\n", list->count);
	return 0;
}
