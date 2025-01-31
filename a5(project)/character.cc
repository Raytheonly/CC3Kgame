#include "character.h"

Character::Character(Vec pos): Position{pos} {}

Vec Character::getPos() const {return Position;}

void Character::updatePos(Vec pos) {Position = pos;}

int Character::getmaxHp() const {return maxhp;}
int Character::getHp() const {return hp;}

void Character::takeDamage(int dmg) {hp -= dmg;}

int Character::getAtk() const {return atk;}
int Character::getDef() const {return def;}

Character::~Character() {}
