#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "GameState.h"

using std::string;
using std::vector;

extern string makeMove(GameState &myState);

extern vector<string> getValidMoves(GameState &myState);

extern vector<string> getAllPossibleMoves(GameState &myState);

extern void getPawnMoves(GameState &myState, int row, int col,
                         vector<string> &validMoves);

extern void getRookMoves(GameState &myState, int row, int col,
                         vector<string> &validMoves);

extern void getBishopMoves(GameState &myState, int row, int col,
                           vector<string> &validMoves);

extern void getKnightMoves(GameState &myState, int row, int col,
                           vector<string> &validMoves);

extern void getQueenMoves(GameState &myState, int row, int col,
                          vector<string> &validMoves);

extern void getKingMoves(GameState &myState, int row, int col,
                         vector<string> &validMoves);

extern void checkForPinsAndChecks(GameState &myState);

extern bool inCheck(GameState &myState);

extern bool squareAttacked(GameState &myState, int row, int col);