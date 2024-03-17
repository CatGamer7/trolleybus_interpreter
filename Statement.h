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

struct Statement_Scope_Block : public Statement {
	Statement_Scope_Block(std::vector<stmt_ptr>& in_stmts);

	void accept(StatementVisitor& stmtVisitor);

	std::vector<stmt_ptr> stmts;
};

struct Statement_If : public Statement {
	Statement_If(expr_ptr& in_condition, stmt_ptr& in_stmt, stmt_ptr& in_else_stmt);
	Statement_If(expr_ptr& in_condition, stmt_ptr& in_stmt);

	void accept(StatementVisitor& stmtVisitor);

	expr_ptr condition;
	stmt_ptr stmt;
	stmt_ptr else_stmt;
};

struct Statement_While : public Statement {
	Statement_While(expr_ptr& in_condition, stmt_ptr& in_stmt);

	void accept(StatementVisitor& stmtVisitor);

	expr_ptr condition;
	stmt_ptr stmt;
};

struct Statement_For : public Statement {
	Statement_For(stmt_ptr& in_initializer, expr_ptr& in_condition, expr_ptr& in_increment, stmt_ptr& in_stmt);

	void accept(StatementVisitor& stmtVisitor);

	stmt_ptr initializer;
	expr_ptr condition;
	expr_ptr increment;
	stmt_ptr stmt;
};

struct StatementVisitor {
	virtual void visit_expression(Statement_Expression& stmt) = 0;
	virtual void visit_print(Statement_Print& stmt) = 0;
	virtual void visit_var_dec(Statement_Variable_Declaration& stmt) = 0;
	virtual void visit_scope_block(Statement_Scope_Block& stmt) = 0;
	virtual void visit_if(Statement_If& stmt) = 0;
	virtual void visit_while(Statement_While& stmt) = 0;
	virtual void visit_for(Statement_For& stmt) = 0;
};