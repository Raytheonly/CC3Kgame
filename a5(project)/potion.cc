#include "potion.h"

Potion::Potion(Vec pos, std::string ty): Decorator{pos, nullptr}, type{ty} {}

std::string Potion::getType() const {return type;}

int Potion::getAtk() const {
    float multiplier = 1;
    if (drowpc) multiplier = boostedpoteffect;
    if (type == "BA") {
        return static_cast<int>(std::ceil(multiplier * BA) + next->getAtk());
    } else if (type == "WA") {
        return static_cast<int>(std::ceil(multiplier * WA) + next->getAtk());
    } else {        // not an Atk potion but a Def potion so no effect on atk
        return next->getAtk();
    }
}

int Potion::getDef() const {
    float multiplier = 1;
    if (drowpc) multiplier = boostedpoteffect;
    if (type == "BD") {
        return static_cast<int>(std::ceil(multiplier * BD) + next->getDef());
    } else if (type == "WD") {
        return static_cast<int>(std::ceil(multiplier * WD) + next->getDef());
    } else {        // not a Def potion but an Atk potion so no effect on def
        return next->getAtk();
    }
}

void Potion::setdrowpc() {drowpc = true;}
