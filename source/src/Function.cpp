#include "Function.h"

Function::Function(func_stmt_ptr& in_declaration, std::shared_ptr<Environment>& in_closure)
    :
    declaration(std::move(in_declaration)),
    closure(in_closure)
{
}

Value Function::call(Interpreter& in_interpreter, std::vector<expr_ptr>& arguments) {
    in_interpreter.visit_function(declaration.get()->body);

    return Value();
}

int Function::arity() {
    return declaration.get()->arguments.size();
}
