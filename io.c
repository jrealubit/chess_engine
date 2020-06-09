/*
 * io.c is used to house the piece movement information for the
 * board for the chess engine. Conencts to chessEngine.h.
 */

 #include "chessEngine.h"
 #include "stdio.h"

 char* printMove(const int move) {
   static char moveStr[6];

   int ff = FileOnBoard[FromBPos(move)];
   int rf = RankOnBoard[FromBPos(move)];
   int ft = FileOnBoard[ToBPos(move)];
   int rt = RankOnBoard[ToBPos(move)];
   int promoted = Promoted(move);

   if (promoted) {
     char pieceChar = 'q';
     if (IsKnight(promoted)) {
       pieceChar = 'n';
     }
     else if (IsRookQueen(promoted) && !IsBishopQueen(promoted)) {
       pieceChar = 'r';
     }
     else if (IsBishopQueen(promoted) && !IsRookQueen(promoted)) {
       pieceChar = 'b';
     }

     sprintf(moveStr, "%c%c%c%c%c", ('a'+ff), ('1'+rf),
            ('a'+ft), ('1'+rt), pieceChar);
   }
   else {
     sprintf(moveStr, "%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
   }

   return moveStr;
 }

 char* printBPos(const int bpos) {
   static char bposStr[3];

   int f = FileOnBoard[bpos];
   int r = RankOnBoard[bpos];
   sprintf(bposStr, "%c%c", ('a'+f), ('1'+r));
   return bposStr;
 }

 void printMoveList(const MoveListStruct* list) {
   int i = 0;
   int score = 0;
   int move = 0;

   printf("Number of moves: %d\nMove list:\n", list->count);

   for (i = 0; i < list->count; ++i) {
     score = list->movesList[i].score;
     move = list->movesList[i].move;
     printf("Move: %d > %s (Score: %d)\n", i+1, printMove(move), score);
   }
 }
