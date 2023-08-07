#include <iostream>
#include <stdio.h>
#include <string>

#include "Engine.h"
#include "GameState.h"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

void UciLoop() {
  GameState myState = GameState();
  while (1) {
    std::string InputFromGUI = "";
    fflush(stdout);
    getline(cin, InputFromGUI);

    if (InputFromGUI.substr(0, 7) == "isready")
      cout << "readyok" << endl;
    else if (InputFromGUI == "uci") {
      cout << "id name Bobbulus" << endl;
      cout << "uciok" << endl;
    } else if (InputFromGUI.substr(0, 2) == "go") {
      cout << "bestmove " + makeMove(myState) << endl;
    } else if (InputFromGUI == "ucinewgame") {
      myState = GameState();
    } else if (InputFromGUI.substr(0, 12) == "position fen") {
      myState.parseFen(InputFromGUI);
    } else if (InputFromGUI.substr(0, 8) == "position") {
      myState.parsePosition(InputFromGUI);
    } else if (InputFromGUI.substr(0, 4) == "quit") {
      break;
    }
  }
}

int main() {
  UciLoop();
  return 0;
}