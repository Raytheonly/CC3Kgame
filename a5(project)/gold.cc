#include "gold.h"

Gold::Gold(Vec pos, char ty): Item{pos}, type{ty} {}

char Gold::getType() const {return type;}

