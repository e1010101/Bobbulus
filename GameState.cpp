#include "GameState.h"

using std::cin;
using std::cout;
using std::endl;

GameState::GameState()
    : board({}), colorToMove(0), inCheck(false), moveLog({}), pins({}),
      checks({}), checkmate(false), stalemate(false) {
  for (int i = 0; i < 8; i++) {
    board.push_back({});
  }
  board[0] = {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'};
  board[1] = {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'};
  board[2] = {'-', '-', '-', '-', '-', '-', '-', '-'};
  board[3] = {'-', '-', '-', '-', '-', '-', '-', '-'};
  board[4] = {'-', '-', '-', '-', '-', '-', '-', '-'};
  board[5] = {'-', '-', '-', '-', '-', '-', '-', '-'};
  board[6] = {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'};
  board[7] = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};
}

void GameState::makeMove(Move move) {
  board[move.startRow][move.startCol] = '-';
  board[move.endRow][move.endCol] = move.pieceMoved;
  moveLog.push_back(move);
  this->colorToMove = this->colorToMove == 0 ? 1 : 0;
  if (move.pieceMoved == 'K') {
    whiteKingLocation[0] = move.endRow;
    whiteKingLocation[1] = move.endCol;
  } else if (move.pieceMoved == 'k') {
    blackKingLocation[0] = move.endRow;
    blackKingLocation[1] = move.endCol;
  }

  if (move.isPawnPromotion) {
    board[move.endRow][move.endCol] = move.pawnPromotionPiece;
  }

  if (move.isEnPassant) {
    board[move.startRow][move.endCol] = '-';
  }

  if (move.pieceMoved == 'P' || move.pieceMoved == 'p') {
    if (abs(move.startRow - move.endRow) == 2) {
      enPassantSquare[0] = (move.startRow + move.endRow) / 2;
      enPassantSquare[1] = move.startCol;
    } else {
      enPassantSquare[0] = -1000;
      enPassantSquare[1] = -1000;
    }
  }

  if (move.isCastle) {
    if (move.endCol - move.startCol == 2) {
      board[move.endRow][move.endCol - 1] = board[move.endRow][move.endCol + 1];
      board[move.endRow][move.endCol + 1] = '-';

    } else if (move.endCol - move.startCol == -2) {
      board[move.endRow][move.endCol + 1] = board[move.endRow][move.endCol - 2];
      board[move.endRow][move.endCol - 2] = '-';
    }
  }

  updateCastlingRights(move);
}

void GameState::updateCastlingRights(Move move) {
  if (move.pieceMoved == 'K') {
    castleRights[0] = false;
    castleRights[1] = false;
  } else if (move.pieceMoved == 'k') {
    castleRights[2] = false;
    castleRights[3] = false;
  } else if (move.pieceMoved == 'R') {
    if (move.startRow == 7) {
      if (move.startCol == 0) {
        castleRights[1] = false;
      } else if (move.startCol == 7) {
        castleRights[0] = false;
      }
    }
  } else if (move.pieceMoved == 'r') {
    if (move.startRow == 0) {
      if (move.startCol == 0) {
        castleRights[3] = false;
      } else if (move.startCol == 7) {
        castleRights[2] = false;
      }
    }
  }
}

vector<Move> GameState::getValidMoves() {
  vector<Move> validMoves;
  vector<Move> possibleMoves;
  vector<vector<vector<int>>> pinsAndChecks = getPinsAndChecks();
  this->pins = pinsAndChecks[0];
  this->checks = pinsAndChecks[1];
  int kingRow;
  int kingCol;

  if (this->colorToMove == 0) {
    kingRow = whiteKingLocation[0];
    kingCol = whiteKingLocation[1];
  } else {
    kingRow = blackKingLocation[0];
    kingCol = blackKingLocation[1];
  }

  if (this->inCheck) {
    if (this->checks.size() == 1) {
      possibleMoves = getAllPossibleMoves();
      vector<int> check = this->checks[0];
      int checkRow = check[0];
      int checkCol = check[1];
      char pieceChecking = board[checkRow][checkCol];
      vector<vector<int>> validSquares = {};
      if (pieceChecking == 'N' || pieceChecking == 'n') {
        validSquares.push_back({checkRow, checkCol});
      } else {
        for (int i = 1; i < 8; i++) {
          vector<int> validSquare = {kingRow + check[2] * i,
                                     kingCol + check[3] * i};
          validSquares.push_back(validSquare);
          if (validSquare[0] == checkRow && validSquare[1] == checkCol) {
            break;
          }
        }
      }

      for (int i = 0; i < possibleMoves.size(); i++) {
        if (possibleMoves[i].pieceMoved != 'K' &&
            possibleMoves[i].pieceMoved != 'k') {
          for (int j = 0; j < validSquares.size(); j++) {
            bool foundValidSquare = false;
            if (possibleMoves[i].endRow == validSquares[j][0] &&
                possibleMoves[i].endCol == validSquares[j][1]) {
              foundValidSquare = true;
              validMoves.push_back(possibleMoves[i]);
              break;
            }
          }
        } else {
          // ALL KING MOVES FOUND ARE VALID
          validMoves.push_back(possibleMoves[i]);
        }
      }
    } else {
      getKingMoves(kingRow, kingCol, validMoves);
    }
  } else {
    validMoves = getAllPossibleMoves();
    getCastleMoves(kingRow, kingCol, validMoves);
  }

  if (validMoves.size() == 0) {
    if (this->inCheck) {
      this->checkmate = true;
    } else {
      this->stalemate = true;
    }
  } else {
    this->checkmate = false;
    this->stalemate = false;
  }

  return validMoves;
}

vector<vector<vector<int>>> GameState::getPinsAndChecks() {
  vector<vector<vector<int>>> pinsAndChecks;
  vector<vector<int>> pins;
  vector<vector<int>> checks;
  bool tempInCheck = false;

  int enemyColor;
  int allyColor;
  int startRow;
  int startCol;

  if (this->colorToMove == 0) {
    enemyColor = 1;
    allyColor = 0;
    startRow = whiteKingLocation[0];
    startCol = whiteKingLocation[1];
  } else {
    enemyColor = 0;
    allyColor = 1;
    startRow = blackKingLocation[0];
    startCol = blackKingLocation[1];
  }

  vector<vector<int>> directions = {{-1, 0},  {0, -1}, {1, 0},  {0, 1},
                                    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
  for (int j = 0; j < directions.size(); j++) {
    vector<int> direction = directions[j];
    vector<int> possiblePin = {-1000, -1000, -1000, -1000};
    for (int i = 1; i < 8; i++) {
      int endRow = startRow + direction[0] * i;
      int endCol = startCol + direction[1] * i;
      if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
        char endPiece = board[endRow][endCol];
        if (getColorOfPiece(endRow, endCol) == allyColor && endPiece != 'K' &&
            endPiece != 'k') {
          if (possiblePin[0] == -1000 && possiblePin[1] == -1000) {
            possiblePin[0] = endRow;
            possiblePin[1] = endCol;
            possiblePin[2] = direction[0];
            possiblePin[3] = direction[1];
          } else {
            break;
          }
        } else if (getColorOfPiece(endRow, endCol) == enemyColor) {
          char type = tolower(endPiece);
          if ((0 <= j && j <= 3 && type == 'r') ||
              (4 <= j && j <= 7 && type == 'b') ||
              (i == 1 && type == 'p' &&
               ((enemyColor == 0 && 6 <= j && j <= 7) ||
                (enemyColor == 1 && 4 <= j && j <= 5))) ||
              (type == 'q')) {
            if (possiblePin[0] == -1000 && possiblePin[1] == -1000) {
              tempInCheck = true;
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
    int endRow = startRow + knightMoves[i][0];
    int endCol = startCol + knightMoves[i][1];
    if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
      char endPiece = board[endRow][endCol];
      if (endPiece == (enemyColor == 0 ? 'N' : 'n')) {
        tempInCheck = true;
        checks.push_back(
            {endRow, endCol, knightMoves[i][0], knightMoves[i][1]});
      }
    }
  }

  inCheck = tempInCheck;
  pinsAndChecks.push_back(pins);
  pinsAndChecks.push_back(checks);

  return pinsAndChecks;
}

bool GameState::squareUnderAttack(int row, int col) {
  this->colorToMove = 1 - this->colorToMove;
  vector<Move> opponentMoves = getAllPossibleMoves();
  this->colorToMove = 1 - this->colorToMove;
  for (int i = 0; i < opponentMoves.size(); i++) {
    Move move = opponentMoves[i];
    if (move.endRow == row && move.endCol == col) {
      return true;
    }
  }
  return false;
}

vector<Move> GameState::getAllPossibleMoves() {
  vector<Move> moves = {};
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[0].size(); j++) {
      if (getColorOfPiece(i, j) == this->colorToMove) {
        char piece = board[i][j];
        if (piece == 'P' || piece == 'p') {
          getPawnMoves(i, j, moves);
        } else if (piece == 'R' || piece == 'r') {
          getRookMoves(i, j, moves);
        } else if (piece == 'B' || piece == 'b') {
          getBishopMoves(i, j, moves);
        } else if (piece == 'N' || piece == 'n') {
          getKnightMoves(i, j, moves);
        } else if (piece == 'Q' || piece == 'q') {
          getQueenMoves(i, j, moves);
        } else if (piece == 'K' || piece == 'k') {
          getKingMoves(i, j, moves);
        }
      }
    }
  }
  return moves;
}

void GameState::getPawnMoves(int row, int col, vector<Move> &moves) {
  bool piecePinned = false;
  vector<int> pinDirection = {-1000, -1000};
  for (int i = pins.size() - 1; i >= 0; i--) {
    if (pins[i][0] == row && pins[i][1] == col) {
      piecePinned = true;
      pinDirection[0] = pins[i][2];
      pinDirection[1] = pins[i][3];
      pins.erase(pins.begin() + i);
      break;
    }
  }

  if (this->colorToMove == 0) {
    if (board[row - 1][col] == '-') {
      if (!piecePinned || (pinDirection[0] == -1 && pinDirection[1] == 0)) {
        moves.push_back(Move({row, col}, {row - 1, col}, board));
        if (row == 6 && board[row - 2][col] == '-') {
          moves.push_back(Move({row, col}, {row - 2, col}, board));
        }
      }
    }
    if (col - 1 >= 0) {
      if (getColorOfPiece(row - 1, col - 1) == 1) {
        if (!piecePinned || (pinDirection[0] == -1 && pinDirection[1] == -1)) {
          moves.push_back(Move({row, col}, {row - 1, col - 1}, board));
        } else if (enPassantSquare[0] == row - 1 &&
                   enPassantSquare[1] == col - 1) {
          moves.push_back(
              Move({row, col}, {row - 1, col - 1}, board, true, false));
        }
      }
    }
    if (col + 1 <= 7) {
      if (getColorOfPiece(row - 1, col + 1) == 1) {
        if (!piecePinned || (pinDirection[0] == -1 && pinDirection[1] == 1)) {
          moves.push_back(Move({row, col}, {row - 1, col + 1}, board));
        } else if (enPassantSquare[0] == row - 1 &&
                   enPassantSquare[1] == col + 1) {
          moves.push_back(
              Move({row, col}, {row - 1, col + 1}, board, true, false));
        }
      }
    }
  } else {
    if (board[row + 1][col] == '-') {
      if (!piecePinned || (pinDirection[0] == 1 && pinDirection[1] == 0)) {
        moves.push_back(Move({row, col}, {row + 1, col}, board));
        if (row == 1 && board[row + 2][col] == '-') {
          moves.push_back(Move({row, col}, {row + 2, col}, board));
        }
      }
    }
    if (col - 1 >= 0) {
      if (getColorOfPiece(row + 1, col - 1) == 0) {
        if (!piecePinned || (pinDirection[0] == 1 && pinDirection[1] == -1)) {
          moves.push_back(Move({row, col}, {row + 1, col - 1}, board));
        } else if (enPassantSquare[0] == row + 1 &&
                   enPassantSquare[1] == col - 1) {
          moves.push_back(
              Move({row, col}, {row + 1, col - 1}, board, true, false));
        }
      }
    }
    if (col + 1 <= 7) {
      if (getColorOfPiece(row + 1, col + 1) == 0) {
        if (!piecePinned || (pinDirection[0] == 1 && pinDirection[1] == 1)) {
          moves.push_back(Move({row, col}, {row + 1, col + 1}, board));
        } else if (enPassantSquare[0] == row + 1 &&
                   enPassantSquare[1] == col + 1) {
          moves.push_back(
              Move({row, col}, {row + 1, col + 1}, board, true, false));
        }
      }
    }
  }
}

void GameState::getRookMoves(int row, int col, vector<Move> &moves) {
  bool piecePinned = false;
  vector<int> pinDirection = {-1000, -1000};
  for (int i = pins.size() - 1; i >= 0; i--) {
    if (pins[i][0] == row && pins[i][1] == col) {
      piecePinned = true;
      pinDirection = {pins[i][2], pins[i][3]};
      if (getPieceAtSquare(row, col) != 'Q' &&
          getPieceAtSquare(row, col) != 'q') {
        pins.erase(pins.begin() + i);
      }
      break;
    }
  }

  vector<vector<int>> directions = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
  int enemyColor = this->colorToMove == 0 ? 1 : 0;
  for (int i = 0; i < directions.size(); i++) {
    for (int j = 1; j < 8; j++) {
      int endRow = row + directions[i][0] * j;
      int endCol = col + directions[i][1] * j;
      if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
        if (!piecePinned ||
            (pinDirection[0] == directions[i][0] &&
             pinDirection[1] == directions[i][1]) ||
            (pinDirection[0] == -directions[i][0] &&
             pinDirection[1] == -directions[i][1])) {
          char endPiece = board[endRow][endCol];
          if (endPiece == '-') {
            moves.push_back(Move({row, col}, {endRow, endCol}, board));
          } else if (getColorOfPiece(endRow, endCol) == enemyColor) {
            moves.push_back(Move({row, col}, {endRow, endCol}, board));
            break;
          } else {
            break;
          }
        }
      } else {
        break;
      }
    }
  }
}

void GameState::getBishopMoves(int row, int col, vector<Move> &moves) {
  bool piecePinned = false;
  vector<int> pinDirection = {-1000, -1000};
  for (int i = pins.size() - 1; i >= 0; i--) {
    if (pins[i][0] == row && pins[i][1] == col) {
      piecePinned = true;
      pinDirection = {pins[i][2], pins[i][3]};
      if (board[row][col] == 'Q' || board[row][col] == 'q') {
        pins.erase(pins.begin() + i);
      }
      break;
    }
  }

  vector<vector<int>> directions = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
  int enemyColor = this->colorToMove == 0 ? 1 : 0;
  for (int i = 0; i < directions.size(); i++) {
    for (int j = 1; j < 8; j++) {
      int endRow = row + directions[i][0] * j;
      int endCol = col + directions[i][1] * j;
      if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
        if (!piecePinned ||
            (pinDirection[0] == directions[i][0] &&
             pinDirection[1] == directions[i][1]) ||
            (pinDirection[0] == -directions[i][0] &&
             pinDirection[1] == -directions[i][1])) {
          char endPiece = board[endRow][endCol];
          if (endPiece == '-') {
            moves.push_back(Move({row, col}, {endRow, endCol}, board));
          } else if (getColorOfPiece(endRow, endCol) == enemyColor) {
            moves.push_back(Move({row, col}, {endRow, endCol}, board));
            break;
          } else {
            break;
          }
        }
      } else {
        break;
      }
    }
  }
}

void GameState::getKnightMoves(int row, int col, vector<Move> &moves) {
  bool piecePinned = false;
  for (int i = pins.size() - 1; i >= 0; i--) {
    if (pins[i][0] == row && pins[i][1] == col) {
      piecePinned = true;
      pins.erase(pins.begin() + i);
      break;
    }
  }

  vector<vector<int>> knightMoves = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                                     {1, -2},  {1, 2},  {2, -1},  {2, 1}};

  int allyColor = this->colorToMove == 0 ? 0 : 1;
  for (int i = 0; i < knightMoves.size(); i++) {
    int endRow = row + knightMoves[i][0];
    int endCol = col + knightMoves[i][1];
    if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
      if (!piecePinned) {
        char endPiece = board[endRow][endCol];
        if (getColorOfPiece(endRow, endCol) != allyColor) {
          moves.push_back(Move({row, col}, {endRow, endCol}, board));
        }
      }
    }
  }
}

void GameState::getQueenMoves(int row, int col, vector<Move> &moves) {
  getRookMoves(row, col, moves);
  getBishopMoves(row, col, moves);
}

void GameState::getKingMoves(int row, int col, vector<Move> &moves) {
  vector<int> rowMoves = {-1, -1, -1, 0, 0, 1, 1, 1};
  vector<int> colMoves = {-1, 0, 1, -1, 1, -1, 0, 1};
  int allyColor = this->colorToMove == 0 ? 0 : 1;
  for (int i = 0; i < 8; i++) {
    int endRow = row + rowMoves[i];
    int endCol = col + colMoves[i];
    if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
      char endPiece = board[endRow][endCol];
      if (getColorOfPiece(endRow, endCol) != allyColor) {
        if (allyColor == 0) {
          whiteKingLocation = {endRow, endCol};
        } else {
          blackKingLocation = {endRow, endCol};
        }
        vector<vector<vector<int>>> pinsAndChecks = getPinsAndChecks();
        if (pinsAndChecks[1].size() == 0) {
          moves.push_back(Move({row, col}, {endRow, endCol}, board));
        }
        if (allyColor == 0) {
          whiteKingLocation = {row, col};
        } else {
          blackKingLocation = {row, col};
        }
      }
    }
  }
}

void GameState::getCastleMoves(int row, int col, vector<Move> &moves) {
  if (squareUnderAttack(row, col)) {
    return;
  }
  if ((this->colorToMove == 0 && castleRights[0]) ||
      (this->colorToMove == 1 && castleRights[2])) {
    getKingsideCastleMoves(row, col, moves);
  }
  if ((this->colorToMove == 0 && castleRights[1]) ||
      (this->colorToMove == 1 && castleRights[3])) {
    getQueensideCastleMoves(row, col, moves);
  }
}

void GameState::getKingsideCastleMoves(int row, int col, vector<Move> &moves) {
  if (board[row][col + 1] == '-' && board[row][col + 2] == '-') {
    if (!squareUnderAttack(row, col + 1) && !squareUnderAttack(row, col + 2)) {
      moves.push_back(Move({row, col}, {row, col + 2}, board, false, true));
    }
  }
}

void GameState::getQueensideCastleMoves(int row, int col, vector<Move> &moves) {
  if (board[row][col - 1] == '-' && board[row][col - 2] == '-' &&
      board[row][col - 3] == '-') {
    if (!squareUnderAttack(row, col - 1) && !squareUnderAttack(row, col - 2)) {
      moves.push_back(Move({row, col}, {row, col - 2}, board, false, true));
    }
  }
}

int GameState::getColorOfPiece(int rank, int file) {
  if (board[rank][file] == '-') {
    return -1;
  } else if (board[rank][file] >= 'a' and board[rank][file] <= 'z') {
    return 1;
  } else {
    return 0;
  }
}

void GameState::printBoard() {
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      cout << board[rank][file] << " ";
    }
    cout << endl;
  }
}

char GameState::getPieceAtSquare(int rank, int file) {
  return tolower(board[rank][file]);
}

void GameState::parsePosition(string InputFromGUI) {
  // position startpos
  if (InputFromGUI.substr(9, 8) == "startpos" and InputFromGUI.length() == 17) {
    this->colorToMove = 0;
  } else if (InputFromGUI.substr(18, 5) == "moves") {
    stringstream ss(InputFromGUI);
    string token;
    int numMovesMade = moveLog.size();
    int movesCounter = 0;

    ss >> token; // position
    ss >> token; // startpos
    ss >> token; // moves

    while (ss >> token) {
      if (movesCounter < numMovesMade) {
        movesCounter++;
        continue;
      }
      Move move = parseMoveToken(token);
      makeMove(move);
    }
  }
}

void GameState::parseFen(string InputFromGUI) {
  string fen = InputFromGUI.substr(12, InputFromGUI.length() - 12);
  string moves = "";
  for (int i = 0; i < fen.length(); i++) {
    if (fen[i] == 'm' && fen[i + 1] == 'o' && fen[i + 2] == 'v' &&
        fen[i + 3] == 'e' && fen[i + 4] == 's') {
      moves = fen.substr(i + 6, fen.length() - i - 6);
      fen = fen.substr(0, i - 1);
      break;
    }
  }

  for (int i = 0; i < 8; i++) {
    int file = 0;
    for (int j = 0; j < fen.length(); j++) {
      if (fen[j] == '/') {
        break;
      } else if (fen[j] >= '1' and fen[j] <= '8') {
        int numSpaces = fen[j] - '0';
        for (int k = 0; k < numSpaces; k++) {
          board[i][file] = '-';
          file++;
        }
      } else if (fen[j] != ' ') {
        board[i][file] = fen[j];
        file++;
      }
    }
    fen = fen.substr(fen.find('/') + 1, fen.length() - fen.find('/'));
  }
  fen = fen.substr(fen.find(' ') + 1, fen.length() - fen.find(' '));

  if (fen[0] == 'w') {
    this->colorToMove = 0;
  } else {
    this->colorToMove = 1;
  }

  if (fen[2] == 'K') {
    castleRights[0] = true;
  } else {
    castleRights[0] = false;
  }

  if (fen[3] == 'Q') {
    castleRights[1] = true;
  } else {
    castleRights[1] = false;
  }

  if (fen[4] == 'k') {
    castleRights[2] = true;
  } else {
    castleRights[2] = false;
  }

  if (fen[5] == 'q') {
    castleRights[3] = true;
  } else {
    castleRights[3] = false;
  }
  if (moves != "") {
    stringstream ss(moves);
    string token;

    while (ss >> token) {
      Move move = parseMoveToken(token);
      makeMove(move);
    }
  }
}

Move GameState::parseMoveToken(string token) {
  if (token.length() == 4) {
    int startCol = token[0] - 'a';
    int startRow = 7 - (token[1] - '1');
    int endCol = token[2] - 'a';
    int endRow = 7 - (token[3] - '1');
    char pieceMoved = board[startRow][startCol];

    if (pieceMoved == 'P' || pieceMoved == 'p') {
      if (endCol == startCol + 1 || endCol == startCol - 1) {
        if (board[endRow][endCol] == '-') {
          return Move({startRow, startCol}, {endRow, endCol}, board, true,
                      false);
        } else {
          return Move({startRow, startCol}, {endRow, endCol}, board);
        }
      } else {
        return Move({startRow, startCol}, {endRow, endCol}, board);
      }
    } else if (pieceMoved == 'K' || pieceMoved == 'k') {
      if (abs(endCol - startCol) == 2) {
        return Move({startRow, startCol}, {endRow, endCol}, board, false, true);
      } else {
        return Move({startRow, startCol}, {endRow, endCol}, board);
      }
    } else {
      return Move({startRow, startCol}, {endRow, endCol}, board);
    }
  } else {
    int startCol = token[0] - 'a';
    int startRow = 7 - (token[1] - '1');
    int endCol = token[2] - 'a';
    int endRow = 7 - (token[3] - '1');
    char promotionPiece = token[4];
    return Move({startRow, startCol}, {endRow, endCol}, board, false, false,
                promotionPiece);
  }
}