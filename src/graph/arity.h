#pragma once

#include <stdexcept>
#include "constants.h"

/// A base class for objects with channal arity.
/// It may be useful to cast a Node with unknown arity to this class to read it's arity;
class Arity
{
public:
    Arity();
    Arity(int _arity);
    int arity() const;
    
protected:
    int _arity;
};