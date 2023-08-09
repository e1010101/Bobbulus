#include "AI.h"
#include "GameState.h"

#include <unordered_map>

using std::cout;
using std::endl;

unordered_map<char, int> pieceScore = {{'K', 0}, {'Q', 10}, {'R', 5}, {'B', 3},
                                       {'N', 3}, {'P', 1},  {'k', 0}, {'q', 10},
                                       {'r', 5}, {'b', 3},  {'n', 3}, {'p', 1}};

int CHECKMATE = 1000;
int STALEMATE = 0;

Move findRandomMove(vector<Move> validMoves) {
  int moveChosen = rand() % validMoves.size();
  return validMoves[moveChosen];
}

Move findBestMove(GameState myState, vector<Move> validMoves) {
  int turnMultiplier = myState.colorToMove == 0 ? 1 : -1;
  int bestMoveIndex = 0;
  int bestMoveScore = -CHECKMATE;
  int currentScore = 0;
  for (int i = 0; i < validMoves.size(); i++) {
    GameState newState = myState;
    newState.makeMove(validMoves[i]);
    if (newState.isCheckmate()) {
      currentScore = CHECKMATE;
    } else if (newState.isStalemate()) {
      currentScore = STALEMATE;
    } else {
      currentScore = evaluateGameState(newState) * turnMultiplier;
    }

    if (currentScore > bestMoveScore) {
      bestMoveScore = currentScore;
      bestMoveIndex = i;
    }
  }
  cout << "Best move: " << validMoves[bestMoveIndex].getChessNotation() << endl;
  cout << "Score: " << bestMoveScore << endl;
  return validMoves[bestMoveIndex];
}

int evaluateGameState(GameState myState) {
  int score = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++)
      score += pieceScore[myState.board[i][j]];
  }

  return score;
}