/*
 * bitboards.c
 *
 * This file is the bit representation of the
 * board for the chess engine.
 * */

 #include "stdio.h"
 #include "chessEngine.h"

 const int BitTable[64] = {
   63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
   51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
   26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
   58, 20, 37, 17, 36, 8
 };

 int countBits(U64 b) {
   int i;
   for(i= 0; b; i++, b &= b - 1);
   return i;
 }

 int popBit(U64 *bb) {
   U64 b = *bb ^ (*bb - 1);
   unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
   *bb &= (*bb - 1);
   return BitTable[(fold * 0x783a9b23) >> 26];
 }

 void printBitBoard(U64 bb) {
 	U64 shiftNum = 1ULL;

 	int r = 0;
 	int f = 0;
 	int b = 0;
 	int b64 = 0;

 	printf("\n");
 	for(r = RANK_8; r >= RANK_1; --r) {
 		for(f = FILE_A; f <= FILE_H; ++f) {
 			b = FR2SQ(f, r);	// 120 based converison
 			b64 = B64(b); // 64 based conversion

 			if((shiftNum << b64) & bb) {
 				printf("X");
       }
 			else {
 				printf("-");
       }
 		}
 		printf("\n");
 	}
     printf("\n\n");
 }
