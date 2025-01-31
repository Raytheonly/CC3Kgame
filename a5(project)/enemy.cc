#include "enemy.h"

Enemy::Enemy(Vec pos, char ty): Character{pos}, type{ty} {
    switch (ty) {
        case 'H':
            hp = HUMAN_HP;
            atk = HUMAN_ATK;
            def = HUMAN_DEF;
            break;
        case 'W':
            hp = DWARF_HP;
            atk = DWARF_ATK;
            def = DWARF_DEF;
            break;
        case 'E':
            hp = ELF_HP;
            atk = ELF_ATK;
            def = ELF_DEF;
            break;
        case 'O':
            hp = ORC_HP;
            atk = ORC_ATK;
            def = ORC_DEF;
            break;
        case 'M':
            hp = MERCHANT_HP;
            atk = MERCHANT_ATK;
            def = MERCHANT_DEF;
            break;
        case 'D':
            hp = DRAGON_HP;
            atk = DRAGON_ATK;
            def = DRAGON_DEF;
            break;
        case 'L':
            hp = HALFLING_HP;
            atk = HALFLING_ATK;
            def = HALFLING_DEF;
            break;
    }
    maxhp = hp;
}

char Enemy::getType() const {return type;}

void Enemy::setDead() {alive = false;}
bool Enemy::getStatus() const {return alive;}
