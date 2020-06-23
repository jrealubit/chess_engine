/*
 * board.c is used for board representation type functionality
 * for the chess engine. Conencts to chessEngine.h.
 */

#include "stdio.h"
#include "chessEngine.h"

void resetBoard(BoardStruct *b) {
	int i = 0;

	// set the values to offboard for the 120-bit board structure
	for (i = 0; i < BRD_POS_NUM; ++i) {
		b->pieces[i] = OFFBOARD;
	}

	// set the values to EMPTY for the 64-bit board structure
	for (i = 0; i < 64; ++i) {
		b->pieces[B120(i)] = EMPTY;
	}

	// set all piece types to 0
	for (i = 0; i < 2; ++i) {
		b->bigPiece[i] = 0;
		b->majorPiece[i] = 0;
		b->minorPiece[i] = 0;
		b->material[i] = 0;
	}

	for (i = 0; i < 3; ++i) {
		b->pawns[i] = 0ULL;
	}

	// clear piece numbers on the board
	for (i = 0; i < 13; ++i) {
		b->pieceNum[i] = 0;
	}

	// set king bition to no square
  // reset side, en passat, and fifty mvoe variables
  // and ply, history ply, and castling permissions
	b->KingSq[WHITE] = b->KingSq[BLACK] = NO_SQ;
	b->side = BOTH;
	b->enPas = NO_SQ;
	b->fiftyMove = 0;
	b->ply = 0;
	b->hisPly = 0;
	b->castlePermission = 0;
	b->posKey = 0ULL;
}

int parseFenStr(char *fen_str, BoardStruct *b) {
	ASSERT(fen_str != NULL);
	ASSERT(b != NULL);

	int f = FILE_A;
	int r = RANK_8;
	int piece = 0;
	int count = 0;
	int i = 0;
	int b64 = 0;
	int b120 = 0;

	resetBoard(b); // reset the board

	while ((r >= RANK_1) && *fen_str) {
		count = 1;
		switch (*fen_str) {
			// set pieces based on current character
			case 'p': piece = bP; break;
			case 'r': piece = bR; break;
			case 'n': piece = bN; break;
			case 'b': piece = bB; break;
			case 'k': piece = bK; break;
			case 'q': piece = bQ; break;
			case 'P': piece = wP; break;
			case 'R': piece = wR; break;
			case 'N': piece = wN; break;
			case 'B': piece = wB; break;
			case 'K': piece = wK; break;
			case 'Q': piece = wQ; break;

			// ignore numbers
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
		    piece = EMPTY;
		    count = *fen_str - '0';
		    break;

			// reached end of rank; start new rank
			case '/':
			case ' ':
		    r--;
		    f = FILE_A;
		    fen_str++;
		    continue;

			default:
		    printf("Error: fen_str parsing error\n");
		    return -1;
    }

		// obtain file
		for (i = 0; i < count; i++) {
    	b64 = r*8 + f;
			b120 = B120(b64);

      if (piece != EMPTY) {
        b->pieces[b120] = piece;
      }
			f++;
    }
		fen_str++;
	}

	ASSERT(*fen_str == 'w' || *fen_str == 'b');
	b->side = (*fen_str == 'w') ? WHITE : BLACK;
	fen_str += 2; // increment castling permissions

	for (i = 0; i < 4; i++) {
	  if (*fen_str == ' ') break;

		switch(*fen_str) {
			case 'K': b->castlePermission |= WKCA; break;
			case 'Q': b->castlePermission |= WQCA; break;
			case 'k': b->castlePermission |= BKCA; break;
			case 'q': b->castlePermission |= BQCA; break;
			default: break;
    }
		fen_str++;
	}
	fen_str++;

	ASSERT(b->castlePermission>=0 && b->castlePermission <= 15);

	if (*fen_str != '-') {
		f = fen_str[0] - 'a';
		r = fen_str[1] - '1';

		ASSERT(f>=FILE_A && f <= FILE_H);
		ASSERT(r>=RANK_1 && r <= RANK_8);
		b->enPas = FR2SQ(f, r);
  }

	b->posKey = generatePosKey(b); // generate hash key
	updateMaterialLists(b); // update the materials on board list
	return 0;
}

void printBoard(const BoardStruct *b) {
	int pos, f, r, piece;

	// print the game board
	printf("\nGame Board:\n\n");
	for (r = RANK_8; r >= RANK_1; --r) {
		printf("%d  ",r+1);
		for (f = FILE_A; f <= FILE_H; ++f) {
			pos = FR2SQ(f, r);
			piece = b->pieces[pos];
			printf("%3c", pieceChar[piece]);
		}
		printf("\n");
	}

	printf("\n ");
	for (f = FILE_A; f <= FILE_H; ++f) {
		printf("%3c", 'a'+f);
	}

	printf("\n");
	printf("side: %c\n", sideChar[b->side]);
	printf("enPas: %d\n", b->enPas);
	printf("castle: %c%c%c%c\n",
			b->castlePermission & WKCA ? 'K' : '-',
			b->castlePermission & WQCA ? 'Q' : '-',
			b->castlePermission & BKCA ? 'k' : '-',
			b->castlePermission & BQCA ? 'q' : '-');
	printf("posKey:%llX\n", b->posKey);
}

// function to update lists in board
void updateMaterialLists(BoardStruct *b) {
	int i, piece, pos, color;

	for (i = 0; i < BRD_POS_NUM; ++i) {
		pos = i;
		piece = b->pieces[i];
		if (piece != OFFBOARD && piece != EMPTY) {
			color = pieceColor[piece];

	    if (pieceBig[piece] == TRUE) b->bigPiece[color]++;
	    if (pieceMinor[piece] == TRUE) b->minorPiece[color]++;
	    if (pieceMajor[piece] == TRUE) b->majorPiece[color]++;

			b->material[color] += pieceValue[piece];
			b->pieceList[piece][b->pieceNum[piece]] = pos;
			b->pieceNum[piece]++;

			if (piece == wK) b->KingSq[WHITE] = pos;
			if (piece == bK) b->KingSq[BLACK] = pos;

			if (piece == wP) {
				SETBIT(b->pawns[WHITE],B64(pos));
				SETBIT(b->pawns[BOTH],B64(pos));
			}
			else if (piece == bP) {
				SETBIT(b->pawns[BLACK],B64(pos));
				SETBIT(b->pawns[BOTH],B64(pos));
			}
		}
	}
}

// function to check and validate the board
int checkBoard(const BoardStruct *b) {
	U64 tempPawns[3] = {0ULL, 0ULL, 0ULL};

	// initialize the temporary pawn values
	tempPawns[WHITE] = b->pawns[WHITE];
	tempPawns[BLACK] = b->pawns[BLACK];
	tempPawns[BOTH] = b->pawns[BOTH];

	// initalize of temporary list values as we do not want to mess
  // with the actual piece values of the game
	int b64, b120, color, count, t, tempNum;
	int tempPieceNum[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int tempBig[2] = {0, 0};
	int tempMajor[2] = {0, 0};
	int tempMinor[2] = {0, 0};
	int tempMaterial[2] = {0, 0};

	// check piece lists
	for (t = wP; t <= bK; ++t) {
		for (tempNum = 0; tempNum < b->pieceNum[t]; ++tempNum) {
			b120 = b->pieceList[t][tempNum];
			ASSERT(b->pieces[b120]==t);
		}
	}

	// check piece counts and counters
	for (b64 = 0; b64 < 64; ++b64) {
		b120 = B120(b64);
		t = b->pieces[b120];
		tempPieceNum[t]++;
		color = pieceColor[t];
		if (pieceBig[t] == TRUE) tempBig[color]++;
		if (pieceMinor[t] == TRUE) tempMinor[color]++;
		if (pieceMajor[t] == TRUE) tempMajor[color]++;
		tempMaterial[color] += pieceValue[t];
	}

	// assert that all piece values found on the board
  // are the same as the actually recorded number of pieces
	for (t = wP; t <= bK; ++t) {
		ASSERT(tempPieceNum[t]==b->pieceNum[t]);
	}

	// check bitboard count values
	count = CNT(tempPawns[WHITE]);
	ASSERT(count == b->pieceNum[wP]);
	count = CNT(tempPawns[BLACK]);
	ASSERT(count == b->pieceNum[bP]);
	count = CNT(tempPawns[BOTH]);
	ASSERT(count == (b->pieceNum[bP] + b->pieceNum[wP]));

	// check bitboard positions
	while (tempPawns[WHITE]) {
		b64 = POP(&tempPawns[WHITE]);
		ASSERT(b->pieces[b120(b64)] == wP);
	}

	while (tempPawns[BLACK]) {
		b64 = POP(&tempPawns[BLACK]);
		ASSERT(b->pieces[b120(b64)] == bP);
	}

	while (tempPawns[BOTH]) {
		b64 = POP(&tempPawns[BOTH]);
		ASSERT( (b->pieces[b120(b64)] == bP) || (b->pieces[b120(b64)] == wP) );
	}

	// additional checks
	ASSERT(tempMaterial[WHITE] == b->material[WHITE] && tempMaterial[BLACK] == b->material[BLACK]);
	ASSERT(tempMinor[WHITE] == b->minorPiece[WHITE] && tempMinor[BLACK] == b->minorPiece[BLACK]);
	ASSERT(tempMajor[WHITE] == b->majorPiece[WHITE] && tempMajor[BLACK] == b->majorPiece[BLACK]);
	ASSERT(tempBig[WHITE] == b->bigPiece[WHITE] && tempBig[BLACK] == b->bigPiece[BLACK]);
	ASSERT(b->side == WHITE || b->side == BLACK);
	ASSERT(generatePosKey(b) == b->posKey);

	// check en passant validity
	ASSERT(b->enPas == NO_SQ || ( RanksBoard[b->enPas] == RANK_6 && b->side == WHITE)
		 || ( RanksBoard[b->enPas] == RANK_3 && b->side == BLACK));

	// check king position
	ASSERT(b->pieces[b->KingSq[WHITE]] == wK);
	ASSERT(b->pieces[b->KingSq[BLACK]] == bK);
	return TRUE; // either return true or fail
}
