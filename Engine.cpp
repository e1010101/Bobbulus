#include "Engine.h"

using std::string;
using std::vector;

using std::cin;
using std::cout;
using std::endl;

string makeMove(GameState &myState) {
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
  // e2e4
  return getMoveString(6, 4, 4, 4);
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
        } else if (tolower(myState.getPieceAtSquare(i, j)) == 'r') {
          getRookMoves(myState, i, j, moves);
        } else if (tolower(myState.getPieceAtSquare(i, j)) == 'b') {
          getBishopMoves(myState, i, j, moves);
        } else if (tolower(myState.getPieceAtSquare(i, j)) == 'n') {
          getKnightMoves(myState, i, j, moves);
        } else if (tolower(myState.getPieceAtSquare(i, j)) == 'q') {
          getQueenMoves(myState, i, j, moves);
        } else if (tolower(myState.getPieceAtSquare(i, j)) == 'k') {
          // getKingMoves(myState, i, j, moves);
        }
      }
    }
  }
  cout << "Number of moves: " << moves.size() << endl;
  // for (int i = 0; i < moves.size(); i++) {
  //   cout << moves[i] << endl;
  // }
  return moves;
}

void getPawnMoves(GameState &myState, int row, int col, vector<string> &moves) {
  // WHITE PAWN MOVES
  // MOVE FORWARD + MOVE FORWARD TWICE
  if (myState.colorToMove == 0) {
    if (myState.board[row - 1][col] == '-') {
      moves.push_back(getMoveString(row, col, row - 1, col));
      if (row == 6 && myState.board[row - 2][col] == '-') {
        moves.push_back(getMoveString(row, col, row - 2, col));
      }
    }
    // CAPTURES
    if (col - 1 >= 0) {
      if (myState.getColorOfPiece(row - 1, col - 1) == 1) {
        moves.push_back(getMoveString(row, col, row - 1, col - 1));
      }
    }
    if (col + 1 < 8) {
      if (myState.getColorOfPiece(row - 1, col + 1) == 1) {
        moves.push_back(getMoveString(row, col, row - 1, col + 1));
      }
    }
  } else {
    // BLACK PAWN MOVES
    // MOVE FORWARD + MOVE FORWARD TWICE
    if (myState.board[row + 1][col] == '-') {
      moves.push_back(getMoveString(row, col, row + 1, col));
      if (row == 1 && myState.board[row + 2][col] == '-') {
        moves.push_back(getMoveString(row, col, row + 2, col));
      }
    }
    // CAPTURES
    if (col - 1 >= 0) {
      if (myState.getColorOfPiece(row + 1, col - 1) == 1) {
        moves.push_back(getMoveString(row, col, row + 1, col - 1));
      }
    }
    if (col + 1 < 8) {
      if (myState.getColorOfPiece(row + 1, col + 1) == 1) {
        moves.push_back(getMoveString(row, col, row + 1, col + 1));
      }
    }
  }
}

void getRookMoves(GameState &myState, int row, int col,
                  vector<string> &validMoves) {
  vector<vector<int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  int enemyColor = 1 - myState.colorToMove;
  for (int d = 0; d < directions.size(); d++) {
    for (int i = 1; i < 8; i++) {
      int endRow = row + directions[d][0] * i;
      int endCol = col + directions[d][1] * i;

      if ((0 <= endRow && endRow < 8) && (0 <= endCol && endCol < 8)) {
        if (myState.getPieceAtSquare(endRow, endCol) == '-') {
          // MOVEMENT
          cout << "endRow: " << endRow << " endCol: " << endCol << endl;
          validMoves.push_back(getMoveString(row, col, endRow, endCol));
        } else if (myState.getColorOfPiece(endRow, endCol) == enemyColor) {
          // CAPTURES
          // cout << "endRow: " << endRow << " endCol: " << endCol << endl;
          validMoves.push_back(getMoveString(row, col, endRow, endCol));
          break;
        } else {
          break;
        }
      } else {
        break;
      }
    }
  }
}

void getBishopMoves(GameState &myState, int row, int col,
                    vector<string> &validMoves) {
  vector<vector<int>> directions = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
  int enemyColor = 1 - myState.colorToMove;
  for (int d = 0; d < directions.size(); d++) {
    for (int i = 1; i < 8; i++) {
      int endRow = row + directions[d][0] * i;
      int endCol = col + directions[d][1] * i;

      if ((0 <= endRow && endRow < 8) && (0 <= endCol && endCol < 8)) {
        if (myState.getPieceAtSquare(endRow, endCol) == '-') {
          // MOVEMENT
          cout << "endRow: " << endRow << " endCol: " << endCol << endl;
          validMoves.push_back(getMoveString(row, col, endRow, endCol));
        } else if (myState.getColorOfPiece(endRow, endCol) == enemyColor) {
          // CAPTURES
          // cout << "endRow: " << endRow << " endCol: " << endCol << endl;
          validMoves.push_back(getMoveString(row, col, endRow, endCol));
          break;
        } else {
          break;
        }
      } else {
        break;
      }
    }
  }
}

void getKnightMoves(GameState &myState, int row, int col,
                    vector<string> &validMoves) {
  vector<vector<int>> directions = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                                    {1, -2},  {1, 2},  {2, -1},  {2, 1}};
  int enemyColor = 1 - myState.colorToMove;
  for (int d = 0; d < directions.size(); d++) {
    int endRow = row + directions[d][0];
    int endCol = col + directions[d][1];

    if ((0 <= endRow && endRow < 8) && (0 <= endCol && endCol < 8)) {
      if (myState.getColorOfPiece(endRow, endCol) == enemyColor ||
          myState.getPieceAtSquare(endRow, endCol) == '-') {
        validMoves.push_back(getMoveString(row, col, endRow, endCol));
      }
    }
  }
}

void getQueenMoves(GameState &myState, int row, int col,
                   vector<string> &validMoves) {
  getRookMoves(myState, row, col, validMoves);
  getBishopMoves(myState, row, col, validMoves);
}