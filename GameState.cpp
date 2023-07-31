#include "GameState.h"

using std::cin;
using std::cout;
using std::endl;

GameState::GameState()
    : colorToMove(0), inCheck(false), moves({}), pins({}), checks({}) {

  board[0][0] = 'r';
  board[0][1] = 'n';
  board[0][2] = 'b';
  board[0][3] = 'q';
  board[0][4] = 'k';
  board[0][5] = 'b';
  board[0][6] = 'n';
  board[0][7] = 'r';
  for (int i = 0; i < 8; i++) {
    board[1][i] = 'p';
  }

  board[7][0] = 'R';
  board[7][1] = 'N';
  board[7][2] = 'B';
  board[7][3] = 'Q';
  board[7][4] = 'K';
  board[7][5] = 'B';
  board[7][6] = 'N';
  board[7][7] = 'R';
  for (int i = 0; i < 8; i++) {
    board[6][i] = 'P';
  }

  for (int rank = 2; rank < 6; rank++) {
    for (int file = 0; file < 8; file++) {
      board[rank][file] = '-';
    }
  }
}

void GameState::updateBoard(string move) {
  if (move.length() == 4) {
    int rank1 = 7 - (move[1] - '1');
    int file1 = move[0] - 'a';
    int rank2 = 7 - (move[3] - '1');
    int file2 = move[2] - 'a';
    char pieceMoved = board[rank1][file1];

    board[rank2][file2] = board[rank1][file1];
    board[rank1][file1] = '-';

    // KING POSITION UPDATE
    if (pieceMoved == 'K') {
      whiteKingLocation[0] = rank2;
      whiteKingLocation[1] = file2;
    } else if (pieceMoved == 'k') {
      blackKingLocation[0] = rank2;
      blackKingLocation[1] = file2;
    }

    if (pieceMoved == 'K' || pieceMoved == 'k') {
      if (file2 - file1 == 2) {
        board[rank2][file2 - 1] = board[rank2][file2 + 1];
        board[rank2][file2 + 1] = '-';
      } else {
        board[rank2][file2 + 1] = board[rank2][file2 - 2];
        board[rank2][file2 - 2] = '-';
      }
    }

    updateCastlingRights(*this, move);

  } else if (move.length() == 5) {
    // PAWN PROMOTION
    string piece = move.substr(4, 1);
    cout << piece << endl;
    int rank1 = 7 - (move[1] - '1');
    int file1 = move[0] - 'a';
    int rank2 = 7 - (move[3] - '1');
    int file2 = move[2] - 'a';

    // WE SWAPPED COLORS FIRST
    if (colorToMove == 0) {
      board[rank2][file2] = (char)tolower(piece[0]);
    } else if (colorToMove == 1) {
      board[rank2][file2] = (char)toupper(piece[0]);
    }
    board[rank1][file1] = '-';
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

string getSquareNotation(int rank, int file) {
  rank = 7 - rank;
  std::string notation;
  notation += static_cast<char>('a' + file);
  notation += static_cast<char>('1' + rank);
  return notation;
}

vector<int> getBoardCoordsFromMove(string move) {
  vector<int> coords;
  coords.push_back(move[0] - 'a');
  coords.push_back(7 - (move[1] - '1'));
  coords.push_back(move[2] - 'a');
  coords.push_back(7 - (move[3] - '1'));
  return coords;
}

string getMoveString(int rank1, int file1, int rank2, int file2) {
  return getSquareNotation(rank1, file1) + getSquareNotation(rank2, file2);
}

void parsePosition(GameState &myState, string InputFromGUI) {
  // position startpos
  if (InputFromGUI.substr(9, 8) == "startpos" and InputFromGUI.length() == 17) {
    myState.colorToMove = 0;
  } else if (InputFromGUI.substr(18, 5) == "moves") {
    vector<string> moves;
    stringstream ss(InputFromGUI);
    string token;
    int currentTurn = myState.colorToMove;
    int numMovesMave = myState.moves.size();
    int movesCounter = 0;

    ss >> token; // position
    ss >> token; // startpos
    ss >> token; // moves

    while (ss >> token) {
      myState.colorToMove = currentTurn == 0 ? 1 : 0;
      if (movesCounter < numMovesMave) {
        movesCounter++;
        continue;
      }
      myState.updateBoard(token);
      myState.moves.push_back(token);
    }

    myState.colorToMove = currentTurn;
  }
}

void parseFen(GameState &myState, string InputFromGUI) {
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
          myState.board[i][file] = '-';
          file++;
        }
      } else if (fen[j] != ' ') {
        myState.board[i][file] = fen[j];
        file++;
      }
    }
    fen = fen.substr(fen.find('/') + 1, fen.length() - fen.find('/'));
  }

  if (fen[0] == 'w') {
    myState.colorToMove = 0;
  } else {
    myState.colorToMove = 1;
  }

  if (fen[2] == 'K') {
    myState.castleRights[0] = true;
  } else {
    myState.castleRights[0] = false;
  }

  if (fen[3] == 'Q') {
    myState.castleRights[1] = true;
  } else {
    myState.castleRights[1] = false;
  }

  if (fen[4] == 'k') {
    myState.castleRights[2] = true;
  } else {
    myState.castleRights[2] = false;
  }

  if (fen[5] == 'q') {
    myState.castleRights[3] = true;
  } else {
    myState.castleRights[3] = false;
  }

  if (moves != "") {
    vector<string> movesVector;
    stringstream ss(moves);
    string token;
    int currentTurn = myState.colorToMove;
    int numMovesMave = myState.moves.size();
    int movesCounter = 0;

    while (ss >> token) {
      cout << token << endl;
      myState.colorToMove = currentTurn == 0 ? 1 : 0;
      if (movesCounter < numMovesMave) {
        movesCounter++;
        continue;
      }
      myState.updateBoard(token);
      myState.moves.push_back(token);
    }
  }
  cout << myState.colorToMove << endl;
}

void updateCastlingRights(GameState &myState, string move) {
  vector<int> coords = getBoardCoordsFromMove(move);
  int startCol = coords[0];
  int startRow = coords[1];
  int endCol = coords[2];
  int endRow = coords[3];

  if (myState.getPieceAtSquare(startRow, startCol) == 'k' &&
      myState.getColorOfPiece(startRow, startCol) == 0) {
    myState.castleRights[0] = false;
    myState.castleRights[1] = false;
  } else if (myState.getPieceAtSquare(startRow, startCol) == 'k' &&
             myState.getColorOfPiece(startRow, startCol) == 1) {
    myState.castleRights[2] = false;
    myState.castleRights[3] = false;
  } else if (myState.getPieceAtSquare(startRow, startCol) == 'r' &&
             myState.getColorOfPiece(startRow, startCol) == 0) {
    if (startRow == 7) {
      if (startCol == 0) {
        myState.castleRights[1] = false;
      } else if (startCol == 7) {
        myState.castleRights[0] = false;
      }
    }
  } else if (myState.getPieceAtSquare(startRow, startCol) == 'r' &&
             myState.getColorOfPiece(startRow, startCol) == 1) {
    if (startRow == 0) {
      if (startCol == 0) {
        myState.castleRights[3] = false;
      } else if (startCol == 7) {
        myState.castleRights[2] = false;
      }
    }
  }
}