#ifndef BOARD_H
#define BOARD_H

#include "Move.h"

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
  vector<vector<char>> board = {};
  int colorToMove;
  // vector<string> moveLog;
  vector<Move> moveLog;
  vector<int> whiteKingLocation = {7, 4};
  vector<int> blackKingLocation = {0, 4};
  bool inCheck;
  vector<vector<int>> pins;
  vector<vector<int>> checks;
  vector<int> enPassantSquare = {-1000, -1000};
  // CASTLE RIGHTS: WKS, WQS, BKS, BQS
  vector<bool> castleRights = {true, true, true, true};

  GameState();

  void makeMove(Move move);

  void updateCastlingRights(Move move);

  vector<Move> getValidMoves();

  vector<vector<vector<int>>> getPinsAndChecks();

  bool squareUnderAttack(int row, int col);

  void updateBoard(string move);

  char getPieceAtSquare(int row, int col);

  vector<Move> getAllPossibleMoves();

  void getPawnMoves(int row, int col, vector<Move> &moves);

  void getRookMoves(int row, int col, vector<Move> &moves);

  void getBishopMoves(int row, int col, vector<Move> &moves);

  void getKnightMoves(int row, int col, vector<Move> &moves);

  void getQueenMoves(int row, int col, vector<Move> &moves);

  void getKingMoves(int row, int col, vector<Move> &moves);

  void getCastleMoves(int row, int col, vector<Move> &moves);

  void getKingsideCastleMoves(int row, int col, vector<Move> &moves);

  void getQueensideCastleMoves(int row, int col, vector<Move> &moves);

  int getColorOfPiece(int row, int col);

  void printBoard();
};

extern string getSquareNotation(int rank, int file);

extern vector<int> getBoardCoordsFromMove(string move);

extern string getMoveString(int rank1, int file1, int rank2, int file2);

extern void parsePosition(GameState &myState, string InputFromGUI);

extern void parseFen(GameState &myState, string InputFromGUI);

extern void updateCastlingRights(GameState &myState, string move);

#endif // BOARD_H