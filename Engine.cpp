#include "Engine.h"

using std::string;
using std::vector;

using std::cin;
using std::cout;
using std::endl;

string makeMove(GameState &myState) {
  vector<Move> validMoves = myState.getValidMoves();
  if (validMoves.size() > 0) {
    int moveChosen = rand() % validMoves.size();
    myState.makeMove(validMoves[moveChosen]);
    string moveToMake = validMoves[moveChosen].getChessNotation();
    return moveToMake;
  }
  // e2e4
  return "e2e4";
}