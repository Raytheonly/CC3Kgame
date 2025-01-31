#ifndef GAME_H
#define GAME_H
#include "character.h"
#include "pc.h"
#include "enemy.h"
#include "vec.h"
#include "potion.h"
#include "gold.h"
#include <vector>
#include <fstream>
#include <string>
#include <map>

class Game {
    // Dictionaries to convert inputfile numbers to item types
    inline static const std::map<int, std::string> POTIONTYPES = {
        {0, "RH"}, {1, "BA"}, {2, "BD"},
        {3, "PH"}, {4, "WA"}, {5, "WD"}
    };
    inline static const std::map<int, char> GOLDTYPES = {
        {6, 'n'}, {7, 's'}, {8, 'm'}, {9, 'd'}
    };
    // Dictionaries to convert between enemy symbol and enemy types
    inline static const std::map<char, std::string> ENEMYNAME = {
        {'H', "Human"}, {'W', "Dwarf"}, {'E', "Elf"}, {'O', "Orc"}, {'M', "Merchant"}, {'D', "Dragon"}, {'L', "Halfling"}
    };

    // Map constants
    const int max_h = 25;
    const int max_w = 79;
    const int max_d = 5;  // 5 floors
    std::vector<std::vector<std::vector<char>>> map;    // (5x25x79) 3D char array storing the game state

    // Gold constants
    const int SMALL_PILE = 1;
    const int NORMAL_PILE = 2;
    const int MERCHANT_HOARD = 4;
    const int DRAGON_HOARD = 6;
    // HP Potion constants
    const int RH = 10;
    const int PH = 10;
    // Characters perks constants
    const int VAMP_HPGAIN = 5;
    const int VAMP_HPLOSS = 5;
    const int TROLL_HPREGEN = 5;
    const int GOBLIN_XTRAGOLD = 5;
    const int ELF_REPEATATK = 2;
    const float ORC_GOB_MULT = 1.5;
    
    // 2D Lists of game objects (1 per floor); IMPORTANT NOTE: Everything stays in the lists/nothing is deleted until the game ends: this is to have a central place to free all the memory.
    std::vector<std::vector<Enemy*>> enemies;
    std::vector<std::vector<Potion*>> potions;
    std::vector<std::vector<Gold*>> goldpiles;

    // Misc state trackers
    int current_floor = 0;      // keep track of floor number
    char celltype = '.';        // keep track of the type of floor the player is occupying so we can turn it back to the right type once they leave. Enemies don't have this problem as they can only occupy floor tiles. Player spawns on floor tile by default.
    bool godmode = false;       // 'f' command
    bool merchantmad = false;   // game-wide state that stays true once activated

    PC* player;
    PC* boostedplayer;          // player decorated by potions, used only in Game::combat(). Ex: Potion2->Potion1->player. Initialized to player. Reset to player on floor change.

    //TO-DO initialize player in constructor
    public:
        Game(std::string race);
        Game(std::ifstream& in, std::string race);
        ~Game();
        void display();
        bool move(std::string direction);   //player move
        void usePotion(std::string direction, bool ranged);  // used both on its own for the 'u' command and in Game::move(), some lines are repeated in the latter case but not a big issue; ranged: whether the player in on the Potion's tile(move command) or using it at distance('u' command)
        bool attack(std::string direction); //returns whether enemy was killed; very similar code to usePotion
        bool combat(Enemy* enemy, bool playerinit);       //two-way combat function; takes in whether player is attacker, returns whether defender died
        void dropGold(Enemy& deadenemy);    //Game handles this, otherwise would need to allow Enemy access to map data; Also takes care of reverting the cell back to empty tile
        bool enemiesAct();                  //call after any player turn: loop through enemies list, if player in range, attack, if not, random movement; first implement troll perk; also checks and returns if player died during this round
        void floorChange();
        void turnHaxOn();                   //'f' setter
        Vec getCardinalCell(int x, int y, std::string direction);   //converts direction into coordinates
};

int randNumGen(int upperbound);     // Utility function to generate a random number between 0 and upperbound-1 (inclusive)

#endif
