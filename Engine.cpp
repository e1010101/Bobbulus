#include "Engine.h"

using std::string;
using std::vector;

using std::cin;
using std::cout;
using std::endl;

string makeMove(GameState &myState) {
  // cout << myState.colorToMove << endl;
  if (myState.moves.size() % 2 == 0) {
    myState.colorToMove = 0;
  } else {
    myState.colorToMove = 1;
  }

  vector<string> validMoves = getValidMoves(myState);
  // cout << "Number of valid moves: " << validMoves.size() << endl;
  if (validMoves.size() > 0) {
    myState.colorToMove = 1 - myState.colorToMove;
    string moveToMake = validMoves[0];
    handleMove(myState, moveToMake);
    return validMoves[0];
  }
  // e2e4
  return getMoveString(6, 4, 4, 4);
}

vector<string> getValidMoves(GameState &myState) {
  vector<string> validMoves;
  checkForPinsAndChecks(myState);
  cout << "In check: " << myState.inCheck << endl;
  cout << "Number of checks: " << myState.checks.size() << endl;
  int kingRow, kingCol;
  if (myState.colorToMove == 0) {
    kingRow = myState.whiteKingLocation[0];
    kingCol = myState.whiteKingLocation[1];
  } else {
    kingRow = myState.blackKingLocation[0];
    kingCol = myState.blackKingLocation[1];
  }

  if (myState.inCheck) {
    if (myState.checks.size() == 1) {
      vector<string> possibleMoves = getAllPossibleMoves(myState);
      cout << "Number of possible moves: " << possibleMoves.size() << endl;
      vector<int> check = myState.checks[0];
      cout << "Check: " << check[0] << " " << check[1] << " " << check[2] << " "
           << check[3] << endl;
      int checkRow = check[0];
      int checkCol = check[1];
      char pieceChecking = myState.getPieceAtSquare(checkRow, checkCol);
      vector<vector<int>> validSquares = {};
      if (pieceChecking == 'n') {
        validSquares = {{checkRow, checkCol}};
      } else {
        for (int i = 1; i < 8; i++) {
          vector<int> validSquare = {kingRow + (check[2]) * i,
                                     kingCol + (check[3]) * i};
          validSquares.push_back(validSquare);
          if (validSquare[0] == checkRow && validSquare[1] == checkCol) {
            break;
          }
        }
      }

      for (int i = possibleMoves.size() - 1; i >= 0; i--) {
        // IF THE PIECE THAT MOVED IS NOT THE KING
        vector<int> coords = getBoardCoordsFromMove(possibleMoves[i]);
        if (coords[0] != kingCol && coords[1] != kingCol) {
          cout << possibleMoves[i] << endl;
          cout << coords[0] << " " << coords[1] << endl;
          int moveEndRow = coords[2];
          int moveEndCol = coords[3];
          cout << moveEndRow << " " << moveEndCol << endl;
          bool validEndSquare = false;
          for (int j = 0; j < validSquares.size(); j++) {
            if (moveEndRow == validSquares[j][0] &&
                moveEndCol == validSquares[j][1]) {
              validEndSquare = true;
              break;
            }
          }

          if (!validEndSquare) {
            possibleMoves.erase(possibleMoves.begin() + i);
          }
        } else {
          cout << "KING MOVE" << endl;
          cout << possibleMoves[i] << endl;
        }
      }
      validMoves = possibleMoves;
    } else {
      getKingMoves(myState, kingRow, kingCol, myState.moves);
    }
  } else {
    validMoves = getAllPossibleMoves(myState);
  }
  cout << "Number of valid moves: " << validMoves.size() << endl;
  for (int i = 0; i < validMoves.size(); i++) {
    cout << validMoves[i] << endl;
  }
  return validMoves;
}

vector<string> getAllPossibleMoves(GameState &myState) {
  vector<string> possibleMoves;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (myState.getColorOfPiece(i, j) == myState.colorToMove) {
        if (myState.getPieceAtSquare(i, j) == 'p') {
          getPawnMoves(myState, i, j, possibleMoves);
        }
        // } else if (myState.getPieceAtSquare(i, j) == 'r') {
        //   getRookMoves(myState, i, j, possibleMoves);
        // } else if (myState.getPieceAtSquare(i, j) == 'b') {
        //   getBishopMoves(myState, i, j, possibleMoves);
        // } else if (myState.getPieceAtSquare(i, j) == 'n') {
        //   getKnightMoves(myState, i, j, possibleMoves);
        // } else if (myState.getPieceAtSquare(i, j) == 'q') {
        //   getQueenMoves(myState, i, j, possibleMoves);
        else if (myState.getPieceAtSquare(i, j) == 'k') {
          getKingMoves(myState, i, j, possibleMoves);
        }
      }
    }
  }
  // cout << "Number of moves: " << possibleMoves.size() << endl;
  // for (int i = 0; i < possibleMoves.size(); i++) {
  //   cout << possibleMoves[i] << endl;
  // }
  return possibleMoves;
}

void handleMove(GameState &myState, string move) {
  string startSq = move.substr(0, 2);
  string endSq = move.substr(2, 2);
  vector<int> coords = getBoardCoordsFromMove(move);
  int startRow = coords[0];
  int startCol = coords[1];
  int endRow = coords[2];
  int endCol = coords[3];

  // UPDATE KING POSITION
  if (myState.getPieceAtSquare(startRow, startCol) == 'k') {
    if (myState.colorToMove == 0) {
      myState.whiteKingLocation[0] = endRow;
      myState.whiteKingLocation[1] = endCol;
    } else {
      myState.blackKingLocation[0] = endRow;
      myState.blackKingLocation[1] = endCol;
    }
  }

  if (myState.getPieceAtSquare(startSq[0] - '0', startSq[1] - '0') == 'p') {
    // PAWN PROMOTION
    if (myState.colorToMove == 0 && endSq[1] == '8') {
      myState.board[endSq[0] - '0'][endSq[1] - '0'] = 'Q';
    } else if (myState.colorToMove == 1 && endSq[1] == '1') {
      myState.board[endSq[0] - '0'][endSq[1] - '0'] = 'q';
    }

    // EN PASSANT
    // START COL != END COL
  }

  myState.moves.push_back(move);
  myState.updateBoard(move);
  myState.printBoard();
}

void getPawnMoves(GameState &myState, int row, int col, vector<string> &moves) {
  bool piecePinned = false;
  vector<int> pinDirection = {};
  for (int i = myState.pins.size() - 1; i >= 0; i--) {
    if (myState.pins[i][0] == row && myState.pins[i][1] == col) {
      piecePinned = true;
      pinDirection = {myState.pins[i][2], myState.pins[i][3]};
      myState.pins.erase(myState.pins.begin() + i);
      break;
    }
  }
  // WHITE PAWN MOVES
  // MOVE FORWARD + MOVE FORWARD TWICE
  if (myState.colorToMove == 0) {
    if (myState.board[row - 1][col] == '-') {
      if (!piecePinned || (pinDirection[0] == -1 && pinDirection[1] == 0)) {
        moves.push_back(getMoveString(row, col, row - 1, col));
        if (row == 6 && myState.board[row - 2][col] == '-') {
          moves.push_back(getMoveString(row, col, row - 2, col));
        }
      }
    }
    // CAPTURES
    if (col - 1 >= 0) {
      if (myState.getColorOfPiece(row - 1, col - 1) == 1) {
        if (!piecePinned || (pinDirection[0] == -1 && pinDirection[1] == -1)) {
          moves.push_back(getMoveString(row, col, row - 1, col - 1));
        }
      }
    }
    if (col + 1 < 8) {
      if (myState.getColorOfPiece(row - 1, col + 1) == 1) {
        if (!piecePinned || (pinDirection[0] == -1 && pinDirection[1] == 1)) {
          moves.push_back(getMoveString(row, col, row - 1, col + 1));
        }
      }
    }
  } else {
    // BLACK PAWN MOVES
    // MOVE FORWARD + MOVE FORWARD TWICE
    if (myState.board[row + 1][col] == '-') {
      if (!piecePinned || (pinDirection[0] == 1 && pinDirection[1] == 0)) {
        moves.push_back(getMoveString(row, col, row + 1, col));
        if (row == 1 && myState.board[row + 2][col] == '-') {
          moves.push_back(getMoveString(row, col, row + 2, col));
        }
      }
    }
    // CAPTURES
    if (col - 1 >= 0) {
      if (myState.getColorOfPiece(row + 1, col - 1) == 0) {
        if (!piecePinned || (pinDirection[0] == 1 && pinDirection[1] == -1)) {
          moves.push_back(getMoveString(row, col, row + 1, col - 1));
        }
      }
    }
    if (col + 1 < 8) {
      if (myState.getColorOfPiece(row + 1, col + 1) == 0) {
        if (!piecePinned || (pinDirection[0] == 1 && pinDirection[1] == 1)) {
          moves.push_back(getMoveString(row, col, row + 1, col + 1));
        }
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
          // cout << "endRow: " << endRow << " endCol: " << endCol << endl;
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

void getKingMoves(GameState &myState, int row, int col,
                  vector<string> &validMoves) {
  vector<int> rowMoves = {-1, -1, -1, 0, 0, 1, 1, 1};
  vector<int> colMoves = {-1, 0, 1, -1, 1, -1, 0, 1};
  int allyColor = myState.colorToMove;
  for (int i = 0; i < 8; i++) {
    int endRow = row + rowMoves[i];
    int endCol = col + colMoves[i];
    if ((0 <= endRow && endRow < 8) && (0 <= endCol && endCol < 8)) {
      int endPiece = myState.getColorOfPiece(endRow, endCol);
      if (endPiece != allyColor) {
        if (allyColor == 0) {
          myState.whiteKingLocation = {endRow, endCol};
        } else {
          myState.blackKingLocation = {endRow, endCol};
        }
        cout << "king checking" << endl;
        checkForPinsAndChecks(myState);
        if (myState.inCheck == false) {
          validMoves.push_back(getMoveString(row, col, endRow, endCol));
        }
        if (allyColor == 0) {
          myState.whiteKingLocation = {row, col};
        } else {
          myState.blackKingLocation = {row, col};
        }
      }
    }
  }
}

void checkForPinsAndChecks(GameState &myState) {
  vector<vector<int>> pins = {};
  vector<vector<int>> checks = {};
  bool inCheck = false;
  int enemyColor, allyColor, startRow, startCol;

  if (myState.colorToMove == 0) {
    enemyColor = 1;
    allyColor = 0;
    startRow = myState.whiteKingLocation[0];
    startCol = myState.whiteKingLocation[1];
  } else {
    enemyColor = 0;
    allyColor = 1;
    startRow = myState.blackKingLocation[0];
    startCol = myState.blackKingLocation[1];
  }

  vector<vector<int>> directions = {{-1, 0},  {1, 0},  {0, -1}, {0, 1},
                                    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  for (int i = 0; i < directions.size(); i++) {
    vector<int> direction = directions[i];
    vector<int> possiblePin = {-1, -1, -1, -1};
    for (int j = 1; j < 8; j++) {
      int endRow = startRow + direction[0] * j;
      int endCol = startCol + direction[1] * j;
      if ((0 <= endRow && endRow < 8) && (0 <= endCol && endCol < 8)) {
        char endPiece = myState.getPieceAtSquare(endRow, endCol);
        if (endPiece == allyColor && endPiece != 'k') {
          if (possiblePin[0] == -1) {
            possiblePin = {endRow, endCol, direction[0], direction[1]};
          } else {
            break;
          }
        } else if (myState.getColorOfPiece(endRow, endCol) == enemyColor) {
          if ((0 <= i && i <= 3 && tolower(endPiece) == 'r') ||
              (4 <= i && i <= 7 && tolower(endPiece) == 'b') ||
              (j == 1 && tolower(endPiece) == 'p' &&
               ((enemyColor == 0 && (i == 6 || i == 7)) ||
                (enemyColor == 1 && (i == 4 || i == 5)))) ||
              (tolower(endPiece) == 'q')) {
            if (possiblePin[0] == -1) {
              inCheck = true;
              checks.push_back({endRow, endCol, direction[0], direction[1]});
              break;
            } else {
              pins.push_back(possiblePin);
              break;
            }
          } else {
            break;
          }
        }
      } else {
        break;
      }
    }
  }

  vector<vector<int>> knightMoves = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                                     {1, -2},  {1, 2},  {2, -1},  {2, 1}};

  for (int i = 0; i < knightMoves.size(); i++) {
    vector<int> knightMove = knightMoves[i];
    int endRow = startRow + knightMove[0];
    int endCol = startCol + knightMove[1];
    if ((0 <= endRow && endRow < 8) && (0 <= endCol && endCol < 8)) {
      char endPiece = myState.getPieceAtSquare(endRow, endCol);
      if (myState.getColorOfPiece(endRow, endCol) == enemyColor &&
          tolower(endPiece) == 'n') {
        inCheck = true;
        cout << "Knight check" << endl;
        checks.push_back({endRow, endCol, knightMove[0], knightMove[1]});
      }
    }
  }

  if (!inCheck) {
    cout << "Not in check" << endl;
  }

  myState.pins = pins;
  myState.checks = checks;
  myState.inCheck = inCheck;
}

bool inCheck(GameState &myState) {
  if (myState.colorToMove == 0) {
    return squareAttacked(myState, myState.whiteKingLocation[0],
                          myState.whiteKingLocation[1]);
  } else {
    return squareAttacked(myState, myState.blackKingLocation[0],
                          myState.blackKingLocation[1]);
  }
}

bool squareAttacked(GameState &myState, int row, int col) {
  myState.colorToMove = myState.colorToMove == 0 ? 1 : 0;
  vector<string> oppMoves = getAllPossibleMoves(myState);
  myState.colorToMove = myState.colorToMove == 0 ? 1 : 0;
  for (int i = 0; i < oppMoves.size(); i++) {
    string move = oppMoves[i];
    int endRow = move[2] - '0';
    int endCol = move[3] - '0';
    if (endRow == row && endCol == col) {
      return true;
    }
  }
  return false;
}