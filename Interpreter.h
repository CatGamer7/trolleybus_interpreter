#pragma once
#include "Statement.h"
#include "AbstractReporter.h"
#include "RuntimeError.h"
#include "Environment.h"

class Interpreter : public ExpressionVisitor, public StatementVisitor {
public:
	Interpreter();

	void interpret(std::vector<stmt_ptr>& stmts, AbstractReporter& reporter);
	bool check_errors();
private:
	void visit_expression(Statement_Expression& stmt) override;
	void visit_print(Statement_Print& stmt) override;
	void visit_var_dec(Statement_Variable_Declaration& stmt) override;
	void visit_scope_block(Statement_Scope_Block& stmt) override;

	Value visit_Binary(Binary_Expression& expr) override;
	Value visit_Unary(Unary_Expression& expr) override;
	Value visit_Grouping(Group_Expression& expr) override;
	Value visit_Literal_Integer(Literal_Expression_Int& expr) override;
	Value visit_Literal_Float(Literal_Expression_Float& expr) override;
	Value visit_Literal_Wstring(Literal_Expression_Wstring& expr) override;
	Value visit_Literal_Bool(Literal_Expression_Bool& expr) override;
	Value visit_Literal_Null(Literal_Expression_Null& expr) override;
	Value visit_Variable_Expression(Variable_Expression& expr) override;
	Value visit_Variable_Assignment_Expression(Variable_Assignment_Expression& expr) override;

	bool is_Truthy(Value& in_val);
	bool is_Equal(Value& left, Value& right);
	bool check_Integer_Operrands(Value& left, Value& right);
	bool check_Float_Operrands(Value& left, Value& right);
	bool check_String_Operrands(Value& left, Value& right);

	bool had_errors = false;

	std::unique_ptr<Environment> env = nullptr;
};