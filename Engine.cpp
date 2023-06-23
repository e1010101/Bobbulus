#include "Engine.h"

using std::string;
using std::vector;

using std::cin;
using std::cout;
using std::endl;

string makeMove(GameState &myState) {
  myState.printBoard();
  cout << myState.colorToMove << endl;
  if (myState.moves.size() % 2 == 0) {
    myState.colorToMove = 0;
  } else {
    myState.colorToMove = 1;
  }

  vector<string> validMoves = getValidMoves(myState);
  cout << "Number of valid moves: " << validMoves.size() << endl;
  if (validMoves.size() > 0) {
    myState.colorToMove = 1 - myState.colorToMove;
    myState.moves.push_back(validMoves[0]);
    myState.updateBoard(validMoves[0]);
    myState.printBoard();
    return validMoves[0];
  }
  return getMoveString(0, 4, 2, 4);
}

vector<string> getValidMoves(GameState &myState) {
  return getAllPossibleMoves(myState);
}

vector<string> getAllPossibleMoves(GameState &myState) {
  vector<string> moves;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (myState.getColorOfPiece(i, j) == myState.colorToMove) {
        if (tolower(myState.getPieceAtSquare(i, j)) == 'p') {
          getPawnMoves(myState, i, j, moves);
        }
      }
    }
  }
  return moves;
}

void getPawnMoves(GameState &myState, int row, int col, vector<string> &moves) {
  // WHITE PAWN MOVES
  if (myState.colorToMove == 0) {
    if (myState.board[row - 1][col] == '-') {
      moves.push_back(getMoveString(row, col, row - 1, col));
      if (myState.board[row - 2][col] == '-') {
        moves.push_back(getMoveString(row, col, row - 2, col));
      }
    }
    if (col - 1 >= 0) {
      if (myState.getColorOfPiece(row - 1, col - 1) == 1) {
        moves.push_back(getMoveString(row, col, row - 1, col - 1));
      }
    }
  } else {
    if (myState.board[row + 1][col] == '-') {
      moves.push_back(getMoveString(row, col, row + 1, col));
      if (myState.board[row + 2][col] == '-') {
        moves.push_back(getMoveString(row, col, row + 2, col));
      }
    }
    if (col - 1 >= 0) {
      if (myState.getColorOfPiece(row + 1, col - 1) == 1) {
        moves.push_back(getMoveString(row, col, row + 1, col - 1));
      }
    }
  }
}

void getRookMoves(GameState &myState, vector<string> &validMoves) {
  // WHITE ROOK MOVES
  if (myState.colorToMove == 0) {
  } else {
  }
}