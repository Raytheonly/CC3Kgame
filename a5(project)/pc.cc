#include "pc.h"

PC::PC(Vec pos, char rc): Character{pos}, type{rc} {
    switch (rc) {
        case 's':
            hp = SHADE_HP;
            atk = SHADE_ATK;
            def = SHADE_DEF;
            break;
        case 'd':
            hp = DROW_HP;
            atk = DROW_ATK;
            def = DROW_DEF;
            break;
        case 'v':
            hp = VAMPIRE_HP;
            atk = VAMPIRE_ATK;
            def = VAMPIRE_DEF;
            break;
        case 't':
            hp = TROLL_HP;
            atk = TROLL_ATK;
            def = TROLL_DEF;
            break;
        case 'g':
            hp = GOBLIN_HP;
            atk = GOBLIN_ATK;
            def = GOBLIN_DEF;
            break;
    }
    maxhp = hp;     // in the case of vampire this won't matter
}

void PC::gainHp(int amount) {
    if (hp + amount <= maxhp || type == 'v') {   // if vampire bypass restriction
        hp += amount;
    } else if (hp + amount > maxhp) {
        hp = maxhp;     // can't exceed maxhp
    }
}

void PC::gainGold(int amount) {gold += amount;}

int PC::getGold() const {
    //Shade perk
    int multiplier = (type == 's'? 1.5: 1.0);       // 1.5 score multiplier for Shade
    int score = static_cast<int>(multiplier * gold);

    return score;
}

char PC::getType() const {return type;}
