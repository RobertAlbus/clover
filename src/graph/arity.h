#pragma once

#include <stdexcept>
#include "constants.h"

template <int __arity>
class Arity
{
public:
    Arity()
    : arity(__arity) {
        if (MIN_ARITY > __arity || __arity > MAX_ARITY ) {
            throw std::domain_error("Arity<int> must have a value between MIN_ARITY and MAX_ARITY. See arity.h");
        }
    }
    
    int arity;
};