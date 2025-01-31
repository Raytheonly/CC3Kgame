#ifndef ENEMY_H
#define ENEMY_H
#include "character.h"
#include "vec.h"
#include "gold.h"

class Enemy: public Character {
    // Enemy Stats Constants
    const int HUMAN_HP = 140;
    const int HUMAN_ATK = 20;
    const int HUMAN_DEF = 20;
    //Special: Drop 2 normal piles of gold(equivalent to 1 merchant hoard), not auto-added to player
    const int DWARF_HP = 100;
    const int DWARF_ATK = 20;
    const int DWARF_DEF = 30;
    //Special: Vampires lose 5 HP when attacking dwarves
    const int ELF_HP = 140;
    const int ELF_ATK = 30;
    const int ELF_DEF = 10;
    //Special: 2 attacks against every race except Drow
    const int ORC_HP = 180;
    const int ORC_ATK = 30;
    const int ORC_DEF = 25;
    //Special: +50% damage to Goblin
    const int MERCHANT_HP = 30;
    const int MERCHANT_ATK = 70;
    const int MERCHANT_DEF = 5;
    //Special: All merchants become aggressive when one of them is attacked, otherwise neutral
    const int DRAGON_HP = 150;
    const int DRAGON_ATK = 20;
    const int DRAGON_DEF = 20;
    //Special: Always with a treasure hoard; can attack player within 1 block of itself or treasure hoard; stationary
    const int HALFLING_HP = 100;
    const int HALFLING_ATK = 15;
    const int HALFLING_DEF = 20;
    //Special: 50% chance player attacks miss

    char type = 'H';
    bool alive = true;                   //needed so that dead enemies can't act in Game::enemiesAct(). Prevents from having to delete and remove from list of enemies.

    public:
        Gold* dragon_hoard = nullptr;        //access to a dragon_hoard when the type is a Dragon; needed by Game::dropGold()
        Enemy(Vec pos, char type);
        virtual ~Enemy() override = default;
        char getType() const;
        void setDead();
        bool getStatus() const;
};

#endif
