#include "game.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>

using namespace std;

Game::Game(string race) {
    // didn't have time to implement random generation
    return;
}

Game::Game(ifstream& file, string race): map(max_d, std::vector<std::vector<char>>(max_h, std::vector<char>(max_w, ' '))),
                                         enemies(max_d, std::vector<Enemy*>()),
                                         potions(max_d, std::vector<Potion*>()),
                                         goldpiles(max_d, std::vector<Gold*>())
{
    // The information extraction process below assumes a specific format of the provided file
    for (int d = 0; d < max_d; d++) {
        for (int h = 0; h < max_h; h++) {
            char c;
            for (int w = 0; w < max_w; w++) {
                file.get(c);

                // Decode input, initialize enemies and items when needed
                int num = c - '0';      // compare ASCII value difference; if difference between 0 and 9, then c is a digit->convert to item
                if (num >= 0 && num <= 5) {             // potion
                    potions[d].push_back(new Potion{Vec{w, h}, POTIONTYPES.at(num)});   // can't use POTIONTYPES[num] cuz operator[] can modify and we declared POTIONTYPES as const
                    c = 'P';
                } else if (num >= 6 && num <= 9) {      // gold
                    goldpiles[d].push_back(new Gold{Vec{w, h}, GOLDTYPES.at(num)});
                    c = 'G';
                } else if (c == 'H' || c == 'W' || c == 'E' || c == 'O' || c == 'M' || c == 'D' || c == 'L') {
                    enemies[d].push_back(new Enemy{Vec{w, h}, c});
                } else if (c == '@' && player == nullptr) {                  // player: only initialize if not initialized. Otherwise risk of overwriting and leaking memory.
                    player = new PC{Vec{w, h}, race[0]};    // notice type convert the string to a char
                    boostedplayer = player;
                } else {
                    c = c;      // nothing to do
                }
                map[d][h][w] = c;
            }
            file.get(c);                //take out the new-line character but don't store it
        }
    }
    //Drow perk
    if (race == "d") Potion::setdrowpc();
}

bool Game::move(string dir) {
    const Vec curr_pos = player->getPos();
    const Vec dest_pos = getCardinalCell(curr_pos.x, curr_pos.y, dir);
    if (dest_pos.x == -1) return false;     // attemping to move out of bounds
    const char c = map[current_floor][dest_pos.y][dest_pos.x];
    //cout << "attempting to move from: (" << curr_pos.x << "," << curr_pos.y << ")" << endl;
    //cout << "to: (" << dest_pos.x << "," << dest_pos.y << ")" << endl;
    
    if (c != '+' && c != '#' && c != '.' && c != 'G' && c != 'P' && c != '\\' ) {   //only valid cells to move to
        cout << "Invalid move" << endl;
        return false;
    }

    map[current_floor][curr_pos.y][curr_pos.x] = celltype;  // turn current cell to its initial state: either +,#,.
    map[current_floor][dest_pos.y][dest_pos.x] = '@';       // turn new cell to player
    player->updatePos(dest_pos);

    if (c == '+' || c == '#' || c == '.') {

        celltype = c;                        // update new cell's type
    } else if (c == 'G') {                   // dest occupied by gold
        // if not a dragon hoard, add gold to player
        for (auto gd: goldpiles[current_floor]) {
            if (!gd->getStatus()) continue;     // gold was already picked up
            Vec position = gd->getPos();
            if (position.x == dest_pos.x && position.y == dest_pos.y) {
                char type = gd->getType();
                if (type == 's') {
                    player->gainGold(SMALL_PILE);
                } else if (type == 'n') {
                    player->gainGold(NORMAL_PILE);
                } else if (type == 'm') {
                    player->gainGold(MERCHANT_HOARD);
                } else if (type == 'd') {
                    // if a player steps on a dragon hoard, it means the dragon hasn't been defeated since dragon_hoard gold is automatically given to player once guarding dragon dies, so can't pick up
                }

                if (type != 'd') {
                    celltype = '.';     //once player leaves, cell is empty tile
                    gd->setUnavailable();
                } else {        // dragon_hoard is not picked up, restore original 'G' cell.
                    celltype = 'G';
                }
                break;
            }
        }
    } else if (c == 'P') {                   // dest occupied by potion
        usePotion(dir, false);
        celltype = '.';
    } else if (c == '\\') {                  // reached stairs
        if (current_floor == max_d-1) {      // finished the game
            cout << "Final Score: " << player->getGold() << endl;
            return true;
        }
        floorChange();
    }
    return false;
}

void Game::usePotion(string dir, bool ranged) {
    const Vec curr_pos = player->getPos();
    cout << "Player position: (" << curr_pos.x << "," << curr_pos.y << ")" << endl;
    const Vec dest_pos = (ranged? getCardinalCell(curr_pos.x, curr_pos.y, dir): curr_pos);      //if invoked by Game::move, just check player position
    if (dest_pos.x == -1) return;     // attemping to move out of bounds
    const char c = map[current_floor][dest_pos.y][dest_pos.x];
    if (ranged && c != 'P') {       //already checked in Game::move if !ranged
        cout << "No potion at location" << endl;
        return;
    }
    for (auto pot: potions[current_floor]) {
        Vec position = pot->getPos();
        if (position.x == dest_pos.x && position.y == dest_pos.y) {
            string type = pot->getType();
            if (type == "RH") {
                player->gainHp(RH);
                cout << "Restored " << RH << "HP" << endl;
            } else if (type == "PH") {
                player->takeDamage(PH);
                cout << "Lost " << PH << "HP" << endl;
            } else {        // BA or PA or BD or PD
                pot->decorate(boostedplayer);       //store existing decorator chain as "next"
                boostedplayer = pot;        //add decorator to the front
                cout << "Obtained a mysterious Attack or Defense potion" << endl;
            }
            if (ranged) map[current_floor][position.y][position.x] = '.';       // only replace if player is not on the Potion tile (not invoked by Game::move)
            return;
        }
    }
}

bool Game::attack(string dir) {
    const Vec curr_pos = player->getPos();
    const Vec dest_pos = getCardinalCell(curr_pos.x, curr_pos.y, dir);
    if (dest_pos.x == -1) return false;     // attemping to move out of bounds
    const char c = map[current_floor][dest_pos.y][dest_pos.x];
    if (c != 'H' && c != 'W' && c != 'E' && c != 'O' && c != 'M' && c != 'D' && c != 'L') {     // not an enemy at the location
        cout << "No enemy at location" << endl;
        return false;
    }

    for (auto enemyp: enemies[current_floor]) {
        Vec position = enemyp->getPos();
        if (position.x == dest_pos.x && position.y == dest_pos.y && enemyp->getStatus()) {     // There is an alive enemy in the direction of attack ()
            if (enemyp->getType() == 'M') merchantmad = true;   // attacked a merchant, all merchants become hostile
            return combat(enemyp, true);
        }
    }
}

bool Game::combat(Enemy* enemy, bool playerinit) {
    Character* attacker;
    Character* defender;
    if (!playerinit) {              
        attacker = enemy;
        defender = boostedplayer;
    } else {
        attacker = boostedplayer;
        defender = enemy;
    }
    
    char en_type = enemy->getType();
    char player_race = player->getType();
    //Halfling perk
    if (playerinit && en_type == 'L') {
        int miss = randNumGen(2);   // 0(no miss) or 1(miss)
        if (miss) {
            cout << "Player missed against the Halfling" << endl; return false;
        }
    }
    //Vampire perk
    if (playerinit && player_race == 'v') {
        if (en_type == 'W') {       //allergic to Dwarves
            player->takeDamage(VAMP_HPLOSS);
        } else {
            player->gainHp(VAMP_HPGAIN);
        }
    }
    //Elf perk
    int atk_count = 1;
    if (!playerinit && en_type == 'E' && player_race != 'd') atk_count = ELF_REPEATATK;
    //Orc perk
    float dmg_mult = 1;
    if (!playerinit && en_type == 'O' && player_race == 'g') dmg_mult = ORC_GOB_MULT;

    // Two-way damage calculation and defender damage reception
    const int defender_def = defender->getDef();
    const int attacker_atk = attacker->getAtk();
    float damage = std::ceil(100.0/(100.0 + defender_def) * attacker_atk * dmg_mult);
    damage = static_cast<int>(damage);
    cout << "atk: " << attacker_atk << ", def: " << defender_def << ", damage: " << damage << endl;
    int total_damage = 0;
    for (int i = 1; i <= atk_count; i++) {
        int damage_instance = damage;
        if (!playerinit) {
            int miss = randNumGen(2);   // enemy attack: 50% chance miss: 0(no miss) or 1(miss)
            if (miss) damage_instance = 0;
        }
        total_damage += damage_instance;
    }
    
    if (!playerinit) defender = player;           // This is very messy, we need to turn back into player otherwise the potion will take damage in their place, which is ridiculous and incorrect. The alternative is to overwrite the Character methods in Potion to call `next->method_name;`
    defender->takeDamage(total_damage);
    // Output
    if (playerinit) {
        cout << "Player dealt " << total_damage << " damage to " << ENEMYNAME.at(en_type) << "(" << enemy->getHp() << "HP)." << endl;
    } else {
        cout << ENEMYNAME.at(en_type) << " dealt " << total_damage << " damage to Player(" << player->getHp() << "HP)." << endl;
    }

    // Death consequences
    bool defender_alive = (defender->getHp() > 0);
    if (playerinit && !defender_alive) {          // enemy dead
        enemy->setDead();
        dropGold(*enemy);
        return true;
    } else if (!playerinit && !defender_alive) {  // player dead
        return true;    // this return value won't matter, we will have a check at the end of every round regardless to check if player is dead since there are multiple ways to die
    } else {                                      // defender alive, nothing to do
        return false;
    }
}

void Game::dropGold(Enemy& deadenemy) {
    char type = deadenemy.getType();
    Vec en_pos = deadenemy.getPos();
    if (type == 'D') {                          // overwrite dragon and dragon hoard cells, auto-add gold to player
        Vec dh_pos = deadenemy.dragon_hoard->getPos();
        map[current_floor][en_pos.y][en_pos.x] = '.';
        map[current_floor][dh_pos.y][dh_pos.x] = '.';
        player->gainGold(DRAGON_HOARD);
    } else if (type == 'M' || type == 'H') {    // both drop the equivalent of a merchant hoard in place (delete merchant or human)
        //replace cell with 'G', add generated merchant_hoard to the list of gold items
        goldpiles[current_floor].push_back(new Gold{Vec{en_pos.x, en_pos.y}, 'm'});
        map[current_floor][en_pos.y][en_pos.x] = 'G';
    } else {                                    // all other enemies drop either small or normal pile
        int sorn = randNumGen(2);               // 0(small) or 1(normal)
        int amount = (sorn == 0? SMALL_PILE : NORMAL_PILE);
        player->gainGold(amount);
        map[current_floor][en_pos.y][en_pos.x] = '.';
    }
    // Goblin perk
    if (player->getType() == 'g') player->gainGold(GOBLIN_XTRAGOLD);
}

bool Game::enemiesAct() {
    for (auto enemyp: enemies[current_floor]) {
        if (godmode) break;                 // ALL enemies not allowed to act at all if godmode is on
        if (!enemyp->getStatus()) continue; // enemy not allowed to act if they are dead
        Vec position = enemyp->getPos();
        char type = enemyp->getType();
    
        vector<Vec> avail_tiles = {position};        // collection of available cells to move to (to be sampled from); contains current position by default (in case no available cell to move to)
        bool fightingplayer = false;       // only move if not fighting
        for (int y = -1; y <= 1; y++) {
            for (int x = -1; x <= 1; x++) {
                Vec dest_pos = {position.x+x,position.y+y};
                char c = map[current_floor][dest_pos.y][dest_pos.x];
                if (c == '@') {         //player in vicinity;
                    if (type == 'M' && !merchantmad) continue; //merchants don't attack unless triggered
                    combat(enemyp, false);    // Don't care about return value, we check player status below anyway
                    fightingplayer = true;
                } else if (c == '.' && type != 'D') {   // sole allowed tile for enemies; dragon is stationary
                    avail_tiles.push_back(dest_pos);  
                } else if (type == 'D' && enemyp->dragon_hoard == nullptr && c == 'G') {   // bind dragon hoard to dragon if not already bound; Would be better to place at the end of constructor
                    for (auto gd: goldpiles[current_floor]) {   //identify the gold
                        Vec gd_pos = gd->getPos();
                        if (gd->getType() == 'd' && gd_pos.y == dest_pos.y && gd_pos.x == dest_pos.x) {
                            enemyp->dragon_hoard = gd;
                        }
                    }
                } else {    
                    // no available tile to move to, stay in place
                }
            }
        }
        if (!fightingplayer) {      // only move if not fighting
            map[current_floor][position.y][position.x] = '.';                   // turn current cell to its initial state: empty tile
            Vec move_tile = avail_tiles[randNumGen(avail_tiles.size())];        // pick a random cell from available cells
            map[current_floor][move_tile.y][move_tile.x] = enemyp->getType();   // turn new cell to enemy's symbol
            enemyp->updatePos(move_tile);                                       // update position
        }
        
        //Dragon perk: vision extends to surroundings of dragon hoard it guards
        if (type == 'D' && !fightingplayer) {               // don't fight twice if already fought player
            Vec position = enemyp->dragon_hoard->getPos();
            for (int y = -1; y <= 1; y++) {
                for (int x = -1; x <= 1; x++) {
                    Vec dest_pos = {position.x-x,position.y-y};
                    char c = map[current_floor][dest_pos.y][dest_pos.x];
                    if (c == '@') {     // player in vicinity of dragon hoard
                        combat(enemyp, false);
                    }
                }
            }
        }
    }
    //Troll perk
    if (player->getType() == 't' && player->getHp() > 0) player->gainHp(TROLL_HPREGEN);

    return (player->getHp() <= 0);
}

void Game::floorChange() {
    boostedplayer = player;     //reset player potion decorator chain
    current_floor += 1;
    cout << "You have reached Floor " << current_floor << endl;
    return;
}

void Game::display() {
    for (int h = 0; h < max_h; h++) {
        for (int w = 0; w < max_w; w++) {
            // TO-DO: Colour code 
            cout << map[current_floor][h][w];
        }
        cout << endl;               // new-line
    }
}

void Game::turnHaxOn() {godmode = true;}

Vec Game::getCardinalCell(int x, int y, string dir) {
    if (dir == "no") {
        y -= 1;
    } else if (dir == "so") {
        y += 1;
    } else if (dir == "ea") {
        x += 1;
    } else if (dir == "we") {
        x -= 1;
    } else if (dir == "ne") {
        x += 1;
        y -= 1;
    } else if (dir == "nw") {
        x -= 1;
        y -= 1;
    } else if (dir == "se") {
        x += 1;
        y += 1;
    } else if (dir == "sw") {
        x -= 1;
        y += 1;
    }

    // Check out of bounds (although this doesn't happen since everything is surrounded by walls and a player can't occupy a wall cell)
    if (x < 0 || x >= max_w || y < 0 || y >= max_h) {
        cout << "Out of bounds" << endl;
        return Vec{-1,-1};                        // caller checks for this to know something went wrong
    }
    // Check valid cell for moving, potion or enemy in respective function. Not here.
    cout << "Destination: (" << x << "," << y << ")" << endl;
    return Vec{x, y};
}

int randNumGen(int n) {
    return rand() % n;      // 0,...,n-1 
}

Game::~Game() {
    // Free game objects
    for (int i = 0; i < max_d; i++) {
        for (auto enmy: enemies[i]) {
            delete enmy;
        }
        enemies[i].clear();     // clear resulting dangling pointers
        for (auto pot: potions[i]) {
            delete pot;
        }
        potions[i].clear();
        for (auto gd: goldpiles[i]) {
            delete gd;
        }
        goldpiles[i].clear();
    }
    delete player;
}
