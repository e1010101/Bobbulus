#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "GameState.h"

using std::string;
using std::vector;

extern string makeMove(GameState &myState);

extern void handleMove(GameState &myState, string move);