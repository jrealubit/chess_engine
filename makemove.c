// makemove.c

#include "stdio.h"
#include "chessEngine.h"

// assume board is called b or definitions will fail
#define HASH_PCE(piece,pos) (b->posKey ^= (pieceKeys[(piece)][(pos)]))
#define HASH_CA (b->posKey ^= (castleKeys[(b->castlePermission)]))
#define HASH_SIDE (b->posKey ^= (sideKey))
#define HASH_EP (b->posKey ^= (pieceKeys[EMPTY][(b->enPas)]))

const int castlePerm[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

// function to clear a piece from a position
static void clearPiece(const int pos, BoardStruct *b) {
	ASSERT(posOnBoard(pos));
  int piece = b->pieces[pos];
  ASSERT(PieceValid(piece));

  int color = pieceColor[piece];
  int i = 0;
  int tempNum = -1;
  HASH_PCE(piece,pos);

	b->pieces[pos] = EMPTY;
  b->material[color] -= pieceValue[piece];

	if (pieceBig[piece]) {
		b->bigPiece[color]--;
		if (pieceMajor[piece]) {
			b->majorPiece[color]--;
		}
    else {
			b->minorPiece[color]--;
		}
	}
  else {
		CLRBIT(b->pawns[color], B64(pos));
		CLRBIT(b->pawns[BOTH], B64(pos));
	}

	for (i = 0; i < b->pieceNum[piece]; ++i) {
		if (b->pieceList[piece][i] == pos) {
			tempNum = i;
			break;
		}
	}

	ASSERT(tempNum != -1);
	b->pieceNum[piece]--;
	b->pieceList[piece][tempNum] = b->pieceList[piece][b->pieceNum[piece]];
}

static void addPiece(const int pos, BoardStruct *b, const int piece) {
  ASSERT(PieceValid(piece));
  ASSERT(posOnBoard(pos));

	int color = pieceColor[piece];
  HASH_PCE(piece,pos);
	b->pieces[pos] = piece;

  if (pieceBig[piece]) {
		b->bigPiece[color]++;
  	if (pieceMajor[piece]) {
  		b->majorPiece[color]++;
  	}
    else {
  		b->minorPiece[color]++;
  	}
	}
  else {
		SETBIT(b->pawns[color],B64(pos));
		SETBIT(b->pawns[BOTH],B64(pos));
	}

	b->material[color] += pieceValue[piece];
	b->pieceList[piece][b->pieceNum[piece]++] = pos;
}

static void movePiece(const int from, const int to, BoardStruct *b) {
  ASSERT(posOnBoard(from));
  ASSERT(posOnBoard(to));

	int i = 0;
	int piece = b->pieces[from];
	int color = pieceColor[piece];

  #ifdef DEBUG
  	int t_PieceNum = FALSE;
  #endif

	HASH_PCE(piece,from);
	b->pieces[from] = EMPTY;

	HASH_PCE(piece,to);
	b->pieces[to] = piece;

	if (!pieceBig[piece]) {
		CLRBIT(b->pawns[color],B64(from));
		CLRBIT(b->pawns[BOTH],B64(from));
		SETBIT(b->pawns[color],B64(to));
		SETBIT(b->pawns[BOTH],B64(to));
	}

	for (i = 0; i < b->pieceNum[piece]; ++i) {
		if (b->pieceList[piece][i] == from) {
			b->pieceList[piece][i] = to;
      #ifdef DEBUG
      			t_PieceNum = TRUE;
      #endif
			break;
		}
	}
	ASSERT(t_PieceNum);
}

int makeMove(BoardStruct *b, int move) {
	ASSERT(CheckBoard(b));

	int from = FromBPos(move);
  int to = ToBPos(move);
  int side = b->side;

	ASSERT(posOnBoard(from));
  ASSERT(posOnBoard(to));
  ASSERT(SideValid(side));
  ASSERT(PieceValid(b->pieces[from]));
	b->history[b->hisPly].posKey = b->posKey;

	if (move & MFLAGEP) {
        if (side == WHITE) {
          clearPiece(to-10,b);
        }
        else {
          clearPiece(to+10,b);
        }
    } else if (move & MFLAGCP) {
        switch(to) {
          case C1:
            movePiece(A1, D1, b);
            break;
          case C8:
            movePiece(A8, D8, b);
            break;
          case G1:
            movePiece(H1, F1, b);
            break;
          case G8:
            movePiece(H8, F8, b);
            break;
          default:
            ASSERT(FALSE);
            break;
        }
    }

	if (b->enPas != NO_SQ) {
   HASH_EP;
  }

  HASH_CA;

	b->history[b->hisPly].move = move;
  b->history[b->hisPly].fiftyMove = b->fiftyMove;
  b->history[b->hisPly].enPas = b->enPas;
  b->history[b->hisPly].castlePermission = b->castlePermission;
  b->castlePermission &= castlePerm[from];
  b->castlePermission &= castlePerm[to];
  b->enPas = NO_SQ;
	HASH_CA;

	int captured = Captured(move);
  b->fiftyMove++;

	if (captured != EMPTY) {
    ASSERT(PieceValid(captured));
    clearPiece(to, b);
    b->fiftyMove = 0;
  }

	b->hisPly++;
	b->ply++;

	if (piecePawn[b->pieces[from]]) {
    b->fiftyMove = 0;
    if (move & MFLAGPS) {
      if (side == WHITE) {
        b->enPas = from+10;
        ASSERT(RanksBrd[b->enPas] == RANK_3);
      }
      else {
        b->enPas=from-10;
        ASSERT(RanksBrd[b->enPas] == RANK_6);
      }
      HASH_EP;
    }
  }

	movePiece(from, to, b);

	int promoted = Promoted(move);
    if (promoted != EMPTY)   {
      ASSERT(PieceValid(promoted) && !piecePawn[promoted]);
      clearPiece(to, b);
      addPiece(to, b, promoted);
    }

	if (pieceKing[b->pieces[to]]) {
    b->KingSq[b->side] = to;
  }

	b->side ^= 1;
  HASH_SIDE;
  ASSERT(CheckBoard(b));

	if (posAttacked(b->KingSq[side],b->side,b))  {
    takeMove(b);
    return FALSE;
  }
	return TRUE;
}

void takeMove(BoardStruct *b) {
	ASSERT(CheckBoard(b));

	b->hisPly--;
  b->ply--;

  int move = b->history[b->hisPly].move;
  int from = FromBPos(move);
  int to = ToBPos(move);

  ASSERT(posOnBoard(from));
  ASSERT(posOnBoard(to));

	if (b->enPas != NO_SQ) HASH_EP;
  HASH_CA;

  b->castlePermission = b->history[b->hisPly].castlePermission;
  b->fiftyMove = b->history[b->hisPly].fiftyMove;
  b->enPas = b->history[b->hisPly].enPas;

  if (b->enPas != NO_SQ) HASH_EP;
  HASH_CA;

  b->side ^= 1;
  HASH_SIDE;

	if (MFLAGEP & move) {
    if (b->side == WHITE) {
      addPiece(to-10, b, bP);
    }
    else {
      addPiece(to+10, b, wP);
    }
  }
  else if (MFLAGCP & move) {
    switch(to) {
      case C1:
        movePiece(D1, A1, b);
        break;
      case C8:
        movePiece(D8, A8, b);
        break;
      case G1:
        movePiece(F1, H1, b);
        break;
      case G8:
        movePiece(F8, H8, b);
        break;
      default:
        ASSERT(FALSE);
        break;
    }
  }

	movePiece(to, from, b);

	if (pieceKing[b->pieces[from]]) {
    b->KingSq[b->side] = from;
  }

	int captured = Captured(move);
  if (captured != EMPTY) {
    ASSERT(PieceValid(captured));
    addPiece(to, b, captured);
  }
	if (Promoted(move) != EMPTY)   {
    ASSERT(PieceValid(Promoted(move)) && !piecePawn[Promoted(move)]);
    clearPiece(from, b);
    addPiece(from, b, (pieceColor[Promoted(move)] == WHITE ? wP : bP));
  }
  ASSERT(CheckBoard(b));
}
