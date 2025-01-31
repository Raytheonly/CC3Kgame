#ifndef ITEM_H
#define ITEM_H
#include "vec.h"

class Item {                   //ABC, never instantiated
    protected:
        Vec Position;
        bool available;
    public:
        Item(Vec pos);
        virtual ~Item() = 0;
        Vec getPos();   //accessor
        bool getStatus();   //getter
        void setUnavailable();   //setter
};

#endif
