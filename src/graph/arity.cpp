#include <stdexcept>
#include "arity.h"
#include "constants.h"

Arity::Arity() {
    throw std::domain_error("must instantiate an Arity with a value");
}
Arity::Arity(int _arity) : _arity(_arity) {
    if (MIN_ARITY > _arity || _arity > MAX_ARITY ) {
        throw std::domain_error("Arity(int) must be instantiated with a value between MIN_ARITY and MAX_ARITY. See arity.h");
    }
};
int Arity::arity() const {
    return _arity;
}

