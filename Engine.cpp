#include "Engine.h"
#include <algorithm>
#include <random>

using std::shuffle;

auto rng = std::default_random_engine{};

string makeMove(GameState &myState) {
  vector<Move> validMoves = myState.getValidMoves();
  shuffle(begin(validMoves), end(validMoves), rng);
  Move bestMove = findBestMove(myState, validMoves);
  myState.makeMove(bestMove);
  myState.printBoard();
  cout << bestMove.getChessNotation() << endl;
  cout << bestMove.pieceMoved << endl;
  return bestMove.getChessNotation();
}