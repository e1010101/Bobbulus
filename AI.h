#include "GameState.h"
#include "Move.h"

#include <iostream>
#include <vector>

Move findRandomMove(vector<Move> validMoves);

Move findBestMove(GameState myState, vector<Move> validMoves);

int findMoveMinMax(GameState myState, vector<Move> validMoves, int depth,
                   int colorToMove);

int findMoveNegaMax(GameState myState, vector<Move> validMoves, int depth,
                    int colorToMove);

int evaluateGameState(GameState myState);