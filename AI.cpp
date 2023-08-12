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
int MAX_DEPTH = 2;
Move nextMove = Move({0, 0}, {0, 0}, {{'0'}}, false, false);

Move findRandomMove(vector<Move> validMoves) {
  int moveChosen = rand() % validMoves.size();
  return validMoves[moveChosen];
}

Move findBestMove(GameState myState, vector<Move> validMoves) {
  int score =
      findMoveNegaMax(myState, validMoves, MAX_DEPTH, myState.colorToMove);
  cout << "Score: " << score << endl;
  return nextMove;
}

// int findMoveMinMax(GameState myState, vector<Move> validMoves, int depth,
//                    int colorToMove) {
//   if (depth == 0) {
//     return evaluateGameState(myState);
//   }

//   if (colorToMove == 0) {
//     int maxScore = -CHECKMATE;
//     for (int i = 0; i < validMoves.size(); i++) {
//       GameState newState = myState;
//       newState.makeMove(validMoves[i]);
//       vector<Move> opponentMoves = newState.getValidMoves();
//       int score = findMoveMinMax(newState, opponentMoves, depth - 1, 1);
//       if (score > maxScore) {
//         maxScore = score;
//         if (depth == MAX_DEPTH) {
//           nextMove = validMoves[i];
//         }
//       }
//     }
//     return maxScore;
//   } else {
//     int minScore = CHECKMATE;
//     for (int i = 0; i < validMoves.size(); i++) {
//       GameState newState = myState;
//       newState.makeMove(validMoves[i]);
//       vector<Move> opponentMoves2 = newState.getValidMoves();
//       int score = findMoveMinMax(newState, opponentMoves2, depth - 1, 0);
//       if (score < minScore) {
//         minScore = score;
//         if (depth == MAX_DEPTH) {
//           nextMove = validMoves[i];
//         }
//       }
//     }
//     return minScore;
//   }
// }

int findMoveNegaMax(GameState myState, vector<Move> validMoves, int depth,
                    int colorToMove) {
  int turnMultiplier = colorToMove == 0 ? 1 : -1;
  if (depth == 0) {
    return turnMultiplier * evaluateGameState(myState);
  }

  int maxScore = -CHECKMATE;
  for (int i = 0; i < validMoves.size(); i++) {
    GameState newState = myState;
    newState.makeMove(validMoves[i]);
    vector<Move> opponentMoves = newState.getValidMoves();
    int score =
        -findMoveNegaMax(newState, opponentMoves, depth - 1, -colorToMove);
    if (score > maxScore) {
      maxScore = score;
      if (depth == MAX_DEPTH) {
        nextMove = validMoves[i];
      }
    }
  }

  return maxScore;
}

int evaluateGameState(GameState myState) {
  if (myState.isCheckmate()) {
    if (myState.colorToMove == 0) {
      return -CHECKMATE;
    } else {
      return CHECKMATE;
    }
  } else if (myState.isStalemate()) {
    return STALEMATE;
  }

  int score = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++)
      score += pieceScore[myState.board[i][j]];
  }

  return score;
}