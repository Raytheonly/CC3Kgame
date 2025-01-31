#ifndef POTION_H
#define POTION_H
#include "vec.h"
#include "item.h"
#include "decorator.h"
#include <cmath>
#include <string>

class Potion: public Decorator {        // Logically it should also be inheriting from Item, but we need Potion to inherit the getAtk() method from PC from Character who has the same "Position" data field as Item -> Diamond problem
    // Static values shared across instances
    inline static const float boostedpoteffect = 1.5;
    inline static bool drowpc = false;                // If player is a Drow, potion effect boosted by 50%

    // Potion Effect Constants: (note: RH and PH are not part of the decorator thing, they are instead needed for Game::usePotion())
    const int BA = 5;
    const int BD = 5;
    
    const int WA = 5;
    const int WD = 5;

    std::string type = "BA";
     public:
        Potion(Vec pos, std::string type);
        virtual ~Potion() override = default;
        std::string getType() const;
        virtual int getAtk() const override;
        virtual int getDef() const override;
        static void setdrowpc();        //static field setter
};

#endif
