#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;

class GameState {
private:
public:
  char board[8][8];
  int colorToMove;
  vector<int> whiteKingLocation = {7, 4};
  vector<int> blackKingLocation = {0, 4};
  vector<string> moves;
  vector<vector<int>> pins;
  vector<vector<int>> checks;
  bool inCheck;
  vector<int> enPassantSquare = {-1000, -1000};

  GameState();

  void updateBoard(string move);

  char getPieceAtSquare(int rank, int file);

  int getColorOfPiece(int rank, int file);

  void printBoard();
};

extern string getSquareNotation(int rank, int file);

extern vector<int> getBoardCoordsFromMove(string move);

extern string getMoveString(int rank1, int file1, int rank2, int file2);

extern void parsePosition(GameState &myState, string InputFromGUI);

#endif // BOARD_H