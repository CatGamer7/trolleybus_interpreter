#include "CallableInterface.h"
#include "Environment.h"


using func_stmt_ptr = std::unique_ptr<Statement_Function>;

class Function: public CallableInterface {
    Function(func_stmt_ptr& in_declaration, std::shared_ptr<Environment>& in_closure);

    Value call(Interpreter& in_interpreter, std::vector<expr_ptr>& arguments);
    int arity();
    
    func_stmt_ptr declaration;
    std::shared_ptr<Environment> closure = nullptr;
};