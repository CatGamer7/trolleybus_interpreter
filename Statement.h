#pragma once
#include "Token.h"
#include "Expression.h"

struct StatementVisitor;

struct Statement {
	virtual void accept(StatementVisitor& exprVisitor) {};
};

typedef std::unique_ptr<Statement> stmt_ptr;

struct Statement_Expression : public Statement {
	Statement_Expression(expr_ptr& in_expr);

	void accept(StatementVisitor& stmtVisitor);

	expr_ptr expr;
};

struct Statement_Print : public Statement {
	Statement_Print(expr_ptr& in_expr);

	void accept(StatementVisitor& stmtVisitor);

	expr_ptr expr;
};

struct Statement_Variable_Declaration : public Statement {
	Statement_Variable_Declaration(expr_ptr& in_expr, Token& name);

	void accept(StatementVisitor& stmtVisitor);

	expr_ptr init_expr;
	Token name;
};

struct StatementVisitor {
	virtual void visit_expression(Statement_Expression& stmt) = 0;
	virtual void visit_print(Statement_Print& stmt) = 0;
	virtual void visit_var_dec(Statement_Variable_Declaration& stmt) = 0;
};