#ifndef PC_H
#define PC_H
#include "character.h"
#include "vec.h"

class PC: public Character {
    // Playable Races Stats Constants
    const int SHADE_HP = 125;
    const int SHADE_ATK = 25;
    const int SHADE_DEF = 25;
    //Special: None
    const int DROW_HP = 150;
    const int DROW_ATK = 25;
    const int DROW_DEF = 15;
    //Special: Potion effects boosted by 50%
    const int VAMPIRE_HP = 50;
    const int VAMPIRE_ATK = 25;
    const int VAMPIRE_DEF = 25;
    //Special: +5HP for every successful attack, no MaxHP
    const int TROLL_HP = 120;
    const int TROLL_ATK = 25;
    const int TROLL_DEF = 15;
    //Special: +5HP every turn, capped by MaxHP
    const int GOBLIN_HP = 110;
    const int GOBLIN_ATK = 15;
    const int GOBLIN_DEF = 20;
    //Special: +5 additional gold from every enemy defeated

    char type = 's';                    // 's':Shade; 'd':Drow; 'v':Vampire; 't':troll; 'g': goblin
    int gold = 0;
    
    public:
        PC(Vec pos, char race);
        void gainGold(int amount);
        void gainHp(int amount);
        int getGold() const;   //getter
        char getType() const;  //getter
        virtual ~PC() override = default;
};

#endif
