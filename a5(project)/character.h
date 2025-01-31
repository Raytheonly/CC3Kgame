#ifndef CHARACTER_H
#define CHARACTER_H
#include "vec.h"

class Character {                   //ABC, never instantiated
    protected:
        int maxhp;
        int hp;
        int atk;
        int def;
        Vec Position;
    public:
        Character(Vec pos);
        virtual ~Character() = 0;

        Vec getPos() const;      //getter
        void updatePos(Vec pos); //setter

        int getmaxHp() const; //getter
        int getHp() const;  //getter
        void takeDamage(int damage);
        
        virtual int getAtk() const;   //to be decorated by potions (only for PC)
        virtual int getDef() const;   //to be decorated by potions (only for PC)
};

#endif
