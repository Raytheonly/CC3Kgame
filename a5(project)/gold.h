#ifndef GOLD_H
#define GOLD_H
#include "item.h"
#include "vec.h"

class Gold: public Item {
    char type = 's';                    // 's':small pile; 'n':normal pile; 'm':merchant hoard; 'd':dragon hoard
    public:
        Gold(Vec pos, char type);
        virtual ~Gold() override = default;
        char getType() const;
};

#endif
