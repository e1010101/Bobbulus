#include <iostream>
#include <stdio.h>

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
      cout << "id author Ezra" << endl;
      cout << "uciok" << endl;
    } else if (InputFromGUI.substr(0, 2) == "go") {
      cout << makeMove(myState) << endl;
    } else if (InputFromGUI.substr(0, 8) == "position") {
      parsePosition(myState, InputFromGUI);
      // string col = MyBoard.colorToMove == 0 ? "white" : "black";
      // cout << col << endl;
    } else if (InputFromGUI.substr(0, 4) == "quit") {
      break;
    }
  }
}

int main() {
  UciLoop();
  return 0;
}