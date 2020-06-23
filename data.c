/*
 * data.c is a file used that serves as a collection of requirement data types
 * for many data structures in the chess engine. Conencts to chessEngine.h.
 */

#include "chessEngine.h"

char pieceChar[] = ".PNBRQKpnbrqk";
char sideChar[] = "wb-";
char fileChar[] = "12345678";
char rankChar[] = "abcdefgh";

int pieceBig[13] = {FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE};
int pieceMajor[13] = {FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE};
int pieceMinor[13] = {FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE};
int pieceValue[13]= {0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000};
int pieceColor[13] = {BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};
int piecePawn[13] = {FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE};
int pieceKnight[13] = {FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE};
int pieceKing[13] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE};
int pieceRookQueen[13] = {FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE};
int pieceBishopQueen[13] = {FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE};
int pieceSlides[13] = {FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE};
