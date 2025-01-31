#ifndef VEC_H
#define VEC_H

struct Vec {    // Helps for passing or returning 2D coordinates (functions can't return two values)
    int x = 0;
    int y = 0;
    // Copy constructor will be used often, but no need for deep copy so default copy constructor suffices
};

#endif
