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