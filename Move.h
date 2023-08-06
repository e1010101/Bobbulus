#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;

class Move {
private:
public:
  unordered_map<char, int> ranksToRows = {{'1', 7}, {'2', 6}, {'3', 5},
                                          {'4', 4}, {'5', 3}, {'6', 2},
                                          {'7', 1}, {'8', 0}};
  unordered_map<int, char> rowsToRanks = {{7, '1'}, {6, '2'}, {5, '3'},
                                          {4, '4'}, {3, '5'}, {2, '6'},
                                          {1, '7'}, {0, '8'}};
  unordered_map<char, int> filesToCols = {{'a', 0}, {'b', 1}, {'c', 2},
                                          {'d', 3}, {'e', 4}, {'f', 5},
                                          {'g', 6}, {'h', 7}};
  unordered_map<int, char> colsToFiles = {{0, 'a'}, {1, 'b'}, {2, 'c'},
                                          {3, 'd'}, {4, 'e'}, {5, 'f'},
                                          {6, 'g'}, {7, 'h'}};
  int startRow;
  int startCol;
  int endRow;
  int endCol;
  char pieceMoved;
  char pieceCaptured;
  bool isPawnPromotion;
  bool isEnPassant;
  bool isCastle;
  char pawnPromotionPiece;

  Move(vector<int> startSq, vector<int> endSq, vector<vector<char>> board);

  Move(vector<int> startSq, vector<int> endSq, vector<vector<char>> board,
       bool isEnPassant, bool isCastle);

  Move(vector<int> startSq, vector<int> endSq, vector<vector<char>> board,
       bool isEnPassant, bool isCastle, char pawnPromotionPiece);

  string getRankFile(int row, int col);

  string getChessNotation();
};

#endif // MOVE_H