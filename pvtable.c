/*
 * attack.c houses the piece principle variation logic for the
 * search parameter function  of the chess engine. Conencts to chessEngine.h.
 */

#include "stdio.h"
#include "chessEngine.h"

const int pvSize = 0x100000 * 2; // 2 MB

void initTable(PVTableStruct* table) {
  table->numEntries = pvSize / sizeof(PVEntryStruct);
  table->numEntries -= 2;
  free(table->pTable);
  table->pTable = (PVEntryStruct*)malloc(table->numEntries * sizeof(PVEntryStruct));
  clearTable(table);
  printf("Initialized PV Table with %d entries\n", table->numEntries);
  return;
}

void clearTable(PVTableStruct* table) {
  PVEntryStruct* p;
  for (p = table->pTable; p < table->pTable+table->numEntries; ++p) {
    p->posKey = 0ULL;
    p->move = 0;
  }
  return;
}
