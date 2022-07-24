#pragma once
#include<algorithm>

/// A base class for objects with channal arity.
/// It may be useful to cast a Node with unknown arity to this class to read it's arity;
class Arity
{
public:
    Arity() {
        throw std::domain_error("must instantiate an Arity with a value");
    };
    Arity(int _arity) : _arity(_arity) {};
    int arity() const {
        return _arity;
    }
    
protected:
    int _arity;
};