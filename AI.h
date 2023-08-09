#include "GameState.h"
#include "Move.h"

#include <iostream>
#include <vector>


Move findRandomMove(vector<Move> validMoves);

Move findBestMove(GameState myState, vector<Move> validMoves);

int evaluateGameState(GameState myState);