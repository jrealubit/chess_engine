/*
 * board.c is used for board representation type functionality
 * for the chess engine. Conencts to chessEngine.h.
 */

#include "stdio.h"
#include "chessEngine.h"

void resetBoard(BoardStruct* b) {
  int i = 0;

  // set the values to offboard for the 120-bit board structure
  for (i = 0; i < BOARD_NUM; ++i) {
    b->chessPieces[i] = OFFBOARD;
  }

  // set the values to EMPTY for the 64-bit board structure
  for (i = 0; i < 64; ++i) {
    b->chessPieces[B120(i)] = EMPTY;
  }

  // set all piece types to 0
  for (i = 0; i < 2; ++i) {
    b->pawns[i] = 0ULL;
    b->bigPieces[i] = 0;
    b->majorPieces[i] = 0;
    b->minorPieces[i] = 0;
    b->material[i] = 0;
  }

  // clear piece numbers on the board
  for (i = 0; i < 13; ++i) {
    b->pieceNum[i] = 0;
  }

  // set king bition to no square
  // reset side, en passat, and fifty mvoe variables
  // and ply, history ply, and castling permissions
  b->kingSq[WHITE] = b->kingSq[BLACK] = NO_SQ;
  b->side = BOTH;
  b->enPas = NO_SQ;
  b->fiftyMove = 0;
  b->ply = b->histPly = 0;
  b->castlePermission = 0;
  b->posKey = 0ULL;
}

void updateMaterialLists(BoardStruct* b) {
  int i, piece, sq, color;
  for (i = 0; i < BOARD_NUM; ++i) {
    sq = i;
    piece = b->chessPieces[i];
    if (piece != OFFBOARD && piece != EMPTY) {
      color = pieceColor[piece];
      if (pieceBig[piece] == TRUE) {
        b->bigPieces[color]++;
      }
      if (pieceMajor[piece] == TRUE) {
        b->majorPieces[color]++;
      }
      if (pieceMinor[piece] == TRUE) {
        b->minorPieces[color]++;
      }

      b->material[color] += pieceValue[piece];
      b->pieceList[piece][b->pieceNum[piece]] = sq;
      b->pieceNum[piece]++;

      // king piece
      if (piece == wK) {
        b->kingSq[WHITE] = sq;
      }
      if (piece == bK) {
        b->kingSq[BLACK] = sq;
      }

      // pawn piece
      if (piece == wP) {
        SETBIT(b->pawns[WHITE], B64(sq));
        SETBIT(b->pawns[BOTH], B64(sq));
      }
      else if (piece == bP) {
        SETBIT(b->pawns[BLACK], B64(sq));
        SETBIT(b->pawns[BOTH], B64(sq));
      }
    }
  }
}

int parseFenStr(char* fen_str, BoardStruct* b) {
  // assert neither fen_str nor board is null
  ASSERT(fen_str_str != NULL);
  ASSERT(b != NULL);

  int r = RANK_8;
  int f = FILE_A;
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
      case 'n': piece = bN; break;
      case 'b': piece = bB; break;
      case 'r': piece = bR; break;
      case 'k': piece = bK; break;
      case 'q': piece = bQ; break;
      case 'P': piece = wP; break;
      case 'N': piece = wN; break;
      case 'B': piece = wB; break;
      case 'R': piece = wR; break;
      case 'K': piece = wK; break;
      case 'Q': piece = wQ; break;

      // parse but ignore numbers
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

      // reached end of the rank; start new rank
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
    for (i = 0; i < count; ++i) {
      b64 = r*8 + f;
      b120 = B120(b64);

      if (piece != EMPTY) {
        b->chessPieces[b120] = piece;
      }

      f++;
    }
    fen_str++;
  }

  ASSERT(*fen_str == 'w' || *fen_str == 'b');
  if (*fen_str == 'w') {
    b->side = WHITE;
  }
  else {
    b->side = BLACK;
  }

  fen_str += 2; // increment to castling permissions

  // now parse the castling permissions
  for (i = 0; i < 4; ++i) {
    if (*fen_str == ' ') break;

    switch (*fen_str) {
      case 'K': b->castlePermission |= WKC; break;
      case 'Q': b->castlePermission |= WQC; break;
      case 'k': b->castlePermission |= BKC; break;
      case 'q': b->castlePermission |= BQC; break;
      default: break;
    }

    fen_str++;
  }

  fen_str++; // increment to the next character
  ASSERT(b->castlePermission >= 0 && b->castlePermission <= 15);

  if (*fen_str != '-') {
    r = fen_str[1] - '1';
    f = fen_str[0] - 'a';

    ASSERT(f >= FILE_A && f <= FILE_H);
    ASSERT(r >= RANK_1 && r <= RANK_8);
    b->enPas = FR2SQ(f, r);
  }

  b->posKey = generatePositionKey(b); // generate hash key
  updateMaterialLists(b); // update the materials on the board
  return 0;
}

void printBoard(const BoardStruct* b) {
  int sq, f, r, piece;

  // print the game baord
  printf("\nGame Board:\n\n");
  for (r = RANK_8; r >= RANK_1; --r) {
    printf("%d ", r+1);
    for (f = FILE_A; f <= FILE_H; ++f) {
      sq = FR2SQ(f, r);
      piece = b->chessPieces[sq];
      printf("%3c", pieceChar[piece]);
    }
    printf("\n");
  }

  printf("\n  ");
  for (f = FILE_A; f <= FILE_H; ++f) {
    printf("%3c", 'a'+f);
  }

  printf("\n\n");
  printf("side: %c\n", sideChar[b->side]);
  printf("enPas: %d\n", b->enPas);
  printf("castle: %c%c%c%c\n",
        b->castlePermission&WKC ? 'K' : '-',
        b->castlePermission&WQC ? 'Q' : '-',
        b->castlePermission&BKC ? 'k' : '-',
        b->castlePermission&BQC ? 'q' : '-');
  printf("PosKey: %llx\n", b->posKey);
}

// TODO: FINISH
int checkBoard(const BoardStruct* b) {
  return 0;
}
