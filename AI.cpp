#include "AI.h"

Move findRandomMove(vector<Move> validMoves) {
  int moveChosen = rand() % validMoves.size();
  return validMoves[moveChosen];
}
