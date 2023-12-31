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
int POSITION_COUNTER = 0;
Move nextMove = Move({0, 0}, {0, 0}, {{'0'}}, false, false);

Move findRandomMove(vector<Move> validMoves) {
  int moveChosen = rand() % validMoves.size();
  return validMoves[moveChosen];
}

Move findBestMove(GameState myState, vector<Move> validMoves) {
  POSITION_COUNTER = 0;
  int score =
      findMoveNegaMaxAlphaBeta(myState, validMoves, MAX_DEPTH, -CHECKMATE,
                               CHECKMATE, myState.colorToMove);
  // for (int i = 0; i < validMoves.size(); i++) {
  //   cout << validMoves[i].getChessNotation() << endl;
  // }
  cout << "Best move: " << nextMove.getChessNotation() << endl;
  cout << "Evaluation: " << score << endl;
  cout << "Searched " << POSITION_COUNTER << " positions" << endl;
  return nextMove;
}

int findMoveNegaMaxAlphaBeta(GameState myState, vector<Move> validMoves,
                             int depth, int alpha, int beta, int colorToMove) {
  POSITION_COUNTER++;
  int turnMultiplier = colorToMove == 0 ? 1 : -1;
  if (depth == 0) {
    return turnMultiplier * evaluateGameState(myState);
  }

  int maxScore = -CHECKMATE - 1;
  for (int i = 0; i < validMoves.size(); i++) {
    myState.makeMove(validMoves[i]);
    vector<Move> opponentMoves = myState.getValidMoves();
    int score = -findMoveNegaMaxAlphaBeta(myState, opponentMoves, depth - 1,
                                          -beta, -alpha, -colorToMove);
    if (score > maxScore) {
      maxScore = score;
      if (depth == MAX_DEPTH) {
        nextMove = validMoves[i];
      }
    }
    myState.undoMove();
    if (maxScore > alpha) {
      alpha = maxScore;
    }
    if (alpha >= beta) {
      break;
    }
  }
  return maxScore;
}

int evaluateGameState(GameState myState) {
  if (myState.checkmate) {
    if (myState.colorToMove == 0) {
      return -CHECKMATE;
    } else {
      return CHECKMATE;
    }
  } else if (myState.stalemate) {
    return STALEMATE;
  }

  int score = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++)
      score += pieceScore[myState.board[i][j]];
  }

  return score;
}