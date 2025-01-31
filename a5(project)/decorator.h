#ifndef DECORATOR_H
#define DECORATOR_H
#include "pc.h"
#include "vec.h"

class Decorator: public PC {
    protected:
        PC* next;
    public:
        Decorator(Vec pos, PC* p);
        virtual ~Decorator() override = default;     // Notice we don't `{delete next;}` because we don't want to delete the concrete component in this case (the player), we maintain player across floors
        void decorate(PC* p);
};

#endif
