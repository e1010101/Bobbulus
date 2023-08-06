#include "Engine.h"

using std::string;
using std::vector;

using std::cin;
using std::cout;
using std::endl;

string makeMove(GameState &myState) {
  // cout << myState.colorToMove << endl;
  if (myState.moveLog.size() % 2 == 0) {
    myState.colorToMove = 0;
  } else {
    myState.colorToMove = 1;
  }

  vector<Move> validMoves = myState.getValidMoves();
  cout << "Number of valid moves: " << validMoves.size() << endl;
  for (int i = 0; i < validMoves.size(); i++) {
    cout << validMoves[i].getChessNotation() << endl;
  }
  if (validMoves.size() > 0) {
    myState.colorToMove = 1 - myState.colorToMove;
    int moveChosen = rand() % validMoves.size();
    myState.makeMove(validMoves[moveChosen]);
    string moveToMake = validMoves[moveChosen].getChessNotation();
    // handleMove(myState, moveToMake);
    return moveToMake;
  }
  // e2e4
  return getMoveString(6, 4, 4, 4);
}