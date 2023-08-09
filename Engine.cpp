#include "Engine.h"

string makeMove(GameState &myState) {
  vector<Move> validMoves = myState.getValidMoves();
  Move bestMove = findBestMove(myState, validMoves);
  myState.makeMove(bestMove);
  return bestMove.getChessNotation();
}