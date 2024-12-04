#pragma once
#include <memory>
#include "Value.h"
#include "Token.h"

class ExpressionVisitor;

struct Expression {
	virtual Value accept(ExpressionVisitor& exprVisitor);
};

typedef std::unique_ptr<Expression> expr_ptr;

struct Binary_Expression : public Expression {
	Binary_Expression(Token in_type, expr_ptr& in_left, expr_ptr& in_right);

	Value accept(ExpressionVisitor& exprVisitor) override;

	Token binary_operator;
	expr_ptr left;
	expr_ptr right;
};

struct Binary_Logical_Expression : public Expression {
	Binary_Logical_Expression(Token in_type, expr_ptr& in_left, expr_ptr& in_right);

	Value accept(ExpressionVisitor& exprVisitor) override;

	Token binary_operator;
	expr_ptr left;
	expr_ptr right;
};

struct Unary_Expression : public Expression {
	Unary_Expression(Token in_type, expr_ptr& in_expr);

	Value accept(ExpressionVisitor& exprVisitor) override;

	Token unary_operator;
	expr_ptr expr;
};

struct Group_Expression : public Expression {
	Group_Expression(expr_ptr& in_expr);

	Value accept(ExpressionVisitor& exprVisitor) override;

	expr_ptr expr;
};


struct Literal_Expression_Int : public Expression {
	Literal_Expression_Int(int in_value);

	Value accept(ExpressionVisitor& exprVisitor) override;

	int value;
};

struct Literal_Expression_Float : public Expression {
	Literal_Expression_Float(float in_value);

	Value accept(ExpressionVisitor& exprVisitor) override;

	float value;
};

struct Literal_Expression_Wstring : public Expression {
	Literal_Expression_Wstring(std::wstring& in_value);

	Value accept(ExpressionVisitor& exprVisitor) override;

	std::wstring value;
};

struct Literal_Expression_Bool : public Expression {
	Literal_Expression_Bool(bool in_value);

	Value accept(ExpressionVisitor& exprVisitor) override;

	bool value;
};

struct Literal_Expression_Null : public Expression {
	Literal_Expression_Null();

	Value accept(ExpressionVisitor& exprVisitor) override;

	int value = NULL;
};

struct Variable_Expression : public Expression {
	Variable_Expression(Token& in_name);

	Value accept(ExpressionVisitor& exprVisitor) override;

	Token name;
};

struct Variable_Assignment_Expression : public Expression {
	Variable_Assignment_Expression(Token& in_name, expr_ptr& in_expr);

	Value accept(ExpressionVisitor& exprVisitor) override;

	Token name;
	expr_ptr expr;
};

class ExpressionVisitor {
public:
	virtual Value visit_Binary(Binary_Expression& expr) = 0;
	virtual Value visit_Unary(Unary_Expression& expr) = 0;
	virtual Value visit_Grouping(Group_Expression& expr) = 0;
	virtual Value visit_Literal_Integer(Literal_Expression_Int& expr) = 0;
	virtual Value visit_Literal_Float(Literal_Expression_Float& expr) = 0;
	virtual Value visit_Literal_Wstring(Literal_Expression_Wstring& expr) = 0;
	virtual Value visit_Literal_Bool(Literal_Expression_Bool& expr) = 0;
	virtual Value visit_Literal_Null(Literal_Expression_Null& expr) = 0;
	virtual Value visit_Variable_Expression(Variable_Expression& expr) = 0;
	virtual Value visit_Variable_Assignment_Expression(Variable_Assignment_Expression& expr) = 0;
	virtual Value visit_Logical_Binary(Binary_Logical_Expression& expr) = 0;
};

enum class Expression_Type {
	Binary, Unary, Grouping, Literal_Integer, Literal_Float, Literal_Wstring, Literal_Bool, Literal_Null, Variable, Variable_Assignment, Logic_Binary
};

class Expression_Type_Visitor : public ExpressionVisitor {
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
	Value visit_Logical_Binary(Binary_Logical_Expression& expr) override;
};

class Expression_Token_String_Visitor : public ExpressionVisitor {
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
	Value visit_Logical_Binary(Binary_Logical_Expression& expr) override;
};