#pragma once
#include<algorithm>

/// A base class for objects with channal arity.
/// It may be useful to cast a Node with unknown arity to this class to read it's arity;
class Arity
{
public:
    Arity() {};
    Arity(int arity) : arity(arity) {};
    int getArity() {
        return arity;
    }
    bool isSupportedConversion(int targetArity) {
        int supportedSourceArities[] = {1, 2};
        int supportedTargetArities[] = {1, 2};
        
        return
            std::any_of(
                std::begin(supportedSourceArities),
                std::end(supportedSourceArities),
                [&](int i) { 
                    return targetArity  == i; 
                }
            ) &&
            std::any_of(
                std::begin(supportedTargetArities),
                std::end(supportedTargetArities),
                [&](int i) { 
                    return arity  == i; 
                }
            );
    }
private:
    int arity;
};