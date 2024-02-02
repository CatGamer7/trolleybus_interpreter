#include "Expression.h"

Binary_Expression::Binary_Expression(Token in_type, expr_ptr& in_left, expr_ptr& in_right) 
:
	left(std::move(in_left)),
	right(std::move(in_right))
{
	binary_operator = in_type;
}

Value Binary_Expression::accept(ExpressionVisitor& exprVisitor){
	return exprVisitor.visit_Binary(*this);
}

Unary_Expression::Unary_Expression(Token in_type, expr_ptr& in_expr)
	:
	expr(std::move(in_expr))
{
	unary_operator = in_type;
}

Value Unary_Expression::accept(ExpressionVisitor& exprVisitor) {
	return exprVisitor.visit_Unary(*this);
}

Group_Expression::Group_Expression(expr_ptr& in_expr)
	:
	expr(std::move(in_expr)) 
{}

Value Group_Expression::accept(ExpressionVisitor& exprVisitor) {
	return exprVisitor.visit_Grouping(*this);
}

Literal_Expression_Float::Literal_Expression_Float(float in_value) {
	value = in_value;
}

Value Literal_Expression_Float::accept(ExpressionVisitor& exprVisitor) {
	return exprVisitor.visit_Literal_Float(*this);
}

Literal_Expression_Int::Literal_Expression_Int(int in_value) {
	value = in_value;
}

Value Literal_Expression_Int::accept(ExpressionVisitor& exprVisitor) {
	return exprVisitor.visit_Literal_Integer(*this);
}

Literal_Expression_Wstring::Literal_Expression_Wstring(std::wstring& in_value) {
	value = in_value;
}

Value Literal_Expression_Wstring::accept(ExpressionVisitor& exprVisitor) {
	return exprVisitor.visit_Literal_Wstring(*this);
}

Literal_Expression_Bool::Literal_Expression_Bool(bool in_value) {
	value = in_value;
}

Value Literal_Expression_Bool::accept(ExpressionVisitor& exprVisitor) {
	return exprVisitor.visit_Literal_Bool(*this);
}

Literal_Expression_Null::Literal_Expression_Null() {}

Value Literal_Expression_Null::accept(ExpressionVisitor& exprVisitor) {
	return exprVisitor.visit_Literal_Null(*this);
}

Value Expression::accept(ExpressionVisitor& exprVisitor){
	return Value();
}

Variable_Expression::Variable_Expression(Token& in_name) : name(in_name) {}

Value Variable_Expression::accept(ExpressionVisitor& exprVisitor) {
	return exprVisitor.visit_Variable_Expression(*this);
}

Variable_Assignment_Expression::Variable_Assignment_Expression(Token& in_name, expr_ptr& in_expr) : name(in_name), expr(std::move(expr)) {}

Value Variable_Assignment_Expression::accept(ExpressionVisitor& exprVisitor) {
	return exprVisitor.visit_Variable_Assignment_Expression(*this);
}

Value Expression_Type_Visitor::visit_Binary(Binary_Expression& expr) {
	return Value((int)Expression_Type::Binary);
}

Value Expression_Type_Visitor::visit_Unary(Unary_Expression& expr) {
	return Value((int)Expression_Type::Unary);
}

Value Expression_Type_Visitor::visit_Grouping(Group_Expression& expr) {
	return Value((int)Expression_Type::Grouping);
}

Value Expression_Type_Visitor::visit_Literal_Integer(Literal_Expression_Int& expr) {
	return Value((int)Expression_Type::Literal_Integer);
}

Value Expression_Type_Visitor::visit_Literal_Float(Literal_Expression_Float& expr) {
	return Value((int)Expression_Type::Literal_Float);
}

Value Expression_Type_Visitor::visit_Literal_Wstring(Literal_Expression_Wstring& expr) {
	return Value((int)Expression_Type::Literal_Wstring);
}

Value Expression_Type_Visitor::visit_Literal_Bool(Literal_Expression_Bool& expr) {
	return Value((int)Expression_Type::Literal_Bool);
}

Value Expression_Type_Visitor::visit_Literal_Null(Literal_Expression_Null& expr){
	return Value((int)Expression_Type::Literal_Null);
}

Value Expression_Type_Visitor::visit_Variable_Expression(Variable_Expression& expr) {
	return Value((int)Expression_Type::Variable);
}

Value Expression_Type_Visitor::visit_Variable_Assignment_Expression(Variable_Assignment_Expression& expr) {
	return Value((int)Expression_Type::Variable_Assignment);
}
