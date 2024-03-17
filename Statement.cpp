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

Statement_Scope_Block::Statement_Scope_Block(std::vector<stmt_ptr>& in_stmts) {
	//stmts.emplace_back(nullptr);

	//std::vector<stmt_ptr>::iterator src_it = in_stmts.begin();
	//std::vector<stmt_ptr>::iterator target_it = stmts.begin() + 1;

	// actual move takes place here

	//std::swap(*src_it, *target_it);
	//stmts.erase(src_it);

	for (auto& s : in_stmts) {
		stmts.push_back(std::move(s));
	}
}



void Statement_Scope_Block::accept(StatementVisitor& stmtVisitor) {
	stmtVisitor.visit_scope_block(*this);
}
