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
    board[rank2][file2] = board[rank1][file1];
    board[rank1][file1] = '-';
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