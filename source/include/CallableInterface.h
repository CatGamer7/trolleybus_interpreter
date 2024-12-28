#pragma once
#include "interpreter.h"


class CallableInterface {
    virtual Value call(Interpreter& in_interpreter, std::vector<expr_ptr> arguments) = 0;
    virtual int arity() = 0;
};