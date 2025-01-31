#include "decorator.h"

Decorator::Decorator(Vec pos, PC* p): PC{pos, 's'}, next{p} {}      // doesn't make sense for a decorator to have a race, but we need to be able to decorate the player's atk,def accessors, so we have to conform to PC's constructor

void Decorator::decorate(PC* p) {
    next = p;
}
