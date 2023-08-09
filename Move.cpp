#include "Move.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

Move::Move(vector<int> startSq, vector<int> endSq, vector<vector<char>> board)
    : isEnPassant(false), isCastle(false) {
  startRow = startSq[0];
  startCol = startSq[1];
  endRow = endSq[0];
  endCol = endSq[1];
  pieceMoved = board[startRow][startCol];
  pieceCaptured = board[endRow][endCol];
  isPawnPromotion =
      (pieceMoved == 'P' && endRow == 0) || (pieceMoved == 'p' && endRow == 7);
  if (isEnPassant) {
    pieceCaptured = pieceMoved == 'P' ? 'p' : 'P';
  }
}

Move::Move(vector<int> startSq, vector<int> endSq, vector<vector<char>> board,
           bool isEnPassant, bool isCastle)
    : isEnPassant(isEnPassant), isCastle(isCastle) {
  startRow = startSq[0];
  startCol = startSq[1];
  endRow = endSq[0];
  endCol = endSq[1];
  pieceMoved = board[startRow][startCol];
  pieceCaptured = board[endRow][endCol];
  isPawnPromotion =
      (pieceMoved == 'P' && endRow == 0) || (pieceMoved == 'p' && endRow == 7);
  if (isEnPassant) {
    pieceCaptured = pieceMoved == 'P' ? 'p' : 'P';
  }
  if (isPawnPromotion) {
    if (pieceMoved == 'P') {
      pawnPromotionPiece = 'Q';
    } else if (pieceMoved == 'p') {
      pawnPromotionPiece = 'q';
    }
  }
}

Move::Move(vector<int> startSq, vector<int> endSq, vector<vector<char>> board,
           bool isEnPassant, bool isCastle, char pawnPromotionPiece)
    : isEnPassant(isEnPassant), isCastle(isCastle),
      pawnPromotionPiece(pawnPromotionPiece) {
  startRow = startSq[0];
  startCol = startSq[1];
  endRow = endSq[0];
  endCol = endSq[1];
  pieceMoved = board[startRow][startCol];
  pieceCaptured = board[endRow][endCol];
  isPawnPromotion =
      (pieceMoved == 'P' && endRow == 0) || (pieceMoved == 'p' && endRow == 7);
  if (isEnPassant) {
    pieceCaptured = pieceMoved == 'P' ? 'p' : 'P';
  }
  if (isPawnPromotion) {
    if (pieceMoved == 'P') {
      pawnPromotionPiece = 'Q';
    } else if (pieceMoved == 'p') {
      pawnPromotionPiece = 'q';
    }
  }
}

string Move::getRankFile(int row, int col) {
  char file = colsToFiles[col];
  char rank = rowsToRanks[row];
  return string(1, file) + string(1, rank);
}

string Move::getChessNotation() {
  if (isPawnPromotion) {
    return getRankFile(startRow, startCol) + getRankFile(endRow, endCol) +
           string(1, pawnPromotionPiece);
  }
  return getRankFile(startRow, startCol) + getRankFile(endRow, endCol);
}