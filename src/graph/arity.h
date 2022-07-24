#pragma once

#include <stdexcept>


#ifndef MAX_ARITY
#define MAX_ARITY 2
#endif

#ifndef MIN_ARITY
#define MIN_ARITY 0
#endif

/// A base class for objects with channal arity.
/// It may be useful to cast a Node with unknown arity to this class to read it's arity;
class Arity
{
public:
    Arity() {
        throw std::domain_error("must instantiate an Arity with a value");
    };
    Arity(int _arity) : _arity(_arity) {
        if (MIN_ARITY > _arity || _arity > MAX_ARITY ) {
            throw std::domain_error("Arity(int) must be instantiated with a value between MIN_ARITY and MAX_ARITY. See arity.h");
        }
    };
    int arity() const {
        return _arity;
    }
    
protected:
    int _arity;
};