#include <iostream>
#include <fstream>
#include "game.h"
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    string command;
    string direction;
    bool started = false;           // whether the player has provided a valid starting command

    // Starting commands
    while (!started) {
        cout << "Select your character" << endl;
        cin >> command;
        if (command == "s" || command == "d" || command == "v" || command == "g" || command == "t") {
            started = true;
        } else if (command == "q") {
            return 0;
        } else {
            cout << "Invalid command" << endl;
        }
    }

    Game* game;
    if (argc >= 2) {               // Optional command-line argument specifying map layout
        string filename = argv[1]; //skip first arg: program name
        ifstream file {filename};
        game = new Game{file, command};
    } else {                       // Otherwise, normal random map generation
        game = new Game{command};
    }
    game->display();

    // In-game commands
    while (cin >> command) {
        if (command == "q") {
            break;
        } else if (command == "no" || command == "so" || command == "ea" || command == "we" || command == "ne" || command == "nw" || command == "se" || command == "sw") {
            bool win = game->move(command);
            if (win) break;
        } else if (command == "a") {
            cin >> direction;
            bool enemykilled = game->attack(direction);
            if (enemykilled) cout << "Enemy killed" << endl;
        } else if (command == "u") {
            cin >> direction;
            game->usePotion(direction, true);
        } else if (command == "r") {
            // might implement if I have time after debugging all the rest
            continue;
        } else if (command == "f") {
            game->turnHaxOn();
        } else {
            cout << "Invalid command" << endl;
            continue;
        }
        bool playerkilled = game->enemiesAct();
        game->display();    // display state of the game after each round
        if (playerkilled) {
            cout << "Game Over" << endl;
            break;
        }
    }
    delete game;        // Free all memory
    return 0;
}
