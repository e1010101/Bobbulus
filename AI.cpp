#include "AI.h"
#include "GameState.h"

#include <unordered_map>

using std::cout;
using std::endl;

unordered_map<char, int> pieceScore = {
    {'K', 0}, {'Q', 10},  {'R', 5},  {'B', 3},  {'N', 3},  {'P', 1},
    {'k', 0}, {'q', -10}, {'r', -5}, {'b', -3}, {'n', -3}, {'p', -1}};

int CHECKMATE = 1000;
int STALEMATE = 0;

Move findRandomMove(vector<Move> validMoves) {
  int moveChosen = rand() % validMoves.size();
  return validMoves[moveChosen];
}

Move findBestMove(GameState myState, vector<Move> validMoves) {
  int turnMultiplier = myState.colorToMove == 0 ? 1 : -1;
  int bestMoveIndex = 0;
  int opponentMinMaxScore = CHECKMATE;
  int currentScore = 0;
  for (int i = 0; i < validMoves.size(); i++) {
    GameState newState = myState;
    newState.makeMove(validMoves[i]);
    vector<Move> opponentMoves = newState.getValidMoves();
    int opponentMaxScore = -CHECKMATE;
    for (int j = 0; j < opponentMoves.size(); j++) {
      GameState newState2 = newState;
      newState2.makeMove(opponentMoves[j]);
      if (newState2.isCheckmate()) {
        currentScore = -turnMultiplier * CHECKMATE;
      } else if (newState2.isStalemate()) {
        currentScore = STALEMATE;
      } else {
        currentScore = -turnMultiplier * evaluateGameState(newState2);
      }

      if (currentScore > opponentMaxScore) {
        opponentMaxScore = currentScore;
      }
    }

    if (opponentMaxScore < opponentMinMaxScore) {
      opponentMinMaxScore = opponentMaxScore;
      bestMoveIndex = i;
    }
  }
  cout << "Best move: " << validMoves[bestMoveIndex].getChessNotation() << endl;
  cout << "Score: " << opponentMinMaxScore << endl;
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