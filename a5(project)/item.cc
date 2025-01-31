#include "item.h"

Item::Item(Vec pos): Position{pos} {}

Item::~Item() {}

Vec Item::getPos() {return Position;}

bool Item::getStatus() {return available;}

void Item::setUnavailable() {available = false;}