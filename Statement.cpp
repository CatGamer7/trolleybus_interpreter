#include "Statement.h"

Statement_Expression::Statement_Expression(expr_ptr& in_expr) 
	:
	expr(std::move(in_expr))
{}

void Statement_Expression::accept(StatementVisitor& stmtVisitor) {
	stmtVisitor.visit_expression(*this);
}

Statement_Print::Statement_Print(expr_ptr & in_expr)
	:
	expr(std::move(in_expr))
{}

void Statement_Print::accept(StatementVisitor & stmtVisitor) {
	stmtVisitor.visit_print(*this);
}

Statement_Variable_Declaration::Statement_Variable_Declaration(expr_ptr& in_expr, Token& name) : init_expr(std::move(in_expr)), name(name) {}

void Statement_Variable_Declaration::accept(StatementVisitor& stmtVisitor) {
	stmtVisitor.visit_var_dec(*this);
}
