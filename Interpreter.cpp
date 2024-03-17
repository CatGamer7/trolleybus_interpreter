#include <cmath>
#include <iostream>
#include "Interpreter.h"

Interpreter::Interpreter() 
	:
	env(std::make_unique<Environment>())
{
}

void Interpreter::interpret(std::vector<stmt_ptr>& stmts, AbstractReporter& reporter) {

	for (auto & stmt : stmts) {
		try {
			stmt->accept(*this);
		}
		catch (Runtime_Exception e) {
			had_errors = true;
			reporter.report(e.what());
		}
	}
}

bool Interpreter::check_errors() {
	return had_errors;
}

void Interpreter::visit_expression(Statement_Expression& stmt) {
	stmt.expr->accept(*this);
}

void Interpreter::visit_print(Statement_Print& stmt) {
	Value v = stmt.expr->accept(*this);
	std::wcout << value_to_string(v) << std::endl;
}

void Interpreter::visit_var_dec(Statement_Variable_Declaration& stmt) {
	Value init_value = _null();
	if (stmt.init_expr != nullptr) {
		init_value = stmt.init_expr->accept(*this);
	}

	env->declare(stmt.name.string, init_value);
}

void Interpreter::visit_scope_block(Statement_Scope_Block& stmt) {
	this->env = std::make_unique<Environment>(this->env);

	try {
		for (auto& s : stmt.stmts) {
			s->accept(*this);
		}
	}
	catch (Runtime_Exception e) {
		this->env = std::move(this->env->get_outer());
	}

	this->env = std::move(this->env->get_outer());
}

Value Interpreter::visit_Binary(Binary_Expression& expr) {
	Value expr_val_left = expr.left->accept(*this);
	Value expr_val_right = expr.right->accept(*this);

	switch (expr.binary_operator.type)	{

	case token_type::PLUS:
		if (check_Integer_Operrands(expr_val_left, expr_val_right)) {
			return std::get<int>(expr_val_left) + std::get<int>(expr_val_right);
		}
		else if (check_Float_Operrands(expr_val_left, expr_val_right)) {
			return std::get<float>(expr_val_left) + std::get<float>(expr_val_right);
		}
		else if (check_String_Operrands(expr_val_left, expr_val_right)) {
			return std::get<std::wstring>(expr_val_left) + std::get<std::wstring>(expr_val_right);
		}
		else {
			throw Runtime_Exception(std::wstring(L"invalid operands for operator (int|float|string allowed)"), expr.binary_operator);
		}

	case token_type::MINUS:
		if (check_Integer_Operrands(expr_val_left, expr_val_right)) {
			return std::get<int>(expr_val_left) - std::get<int>(expr_val_right);
		}
		else if (check_Float_Operrands(expr_val_left, expr_val_right)) {
			return std::get<float>(expr_val_left) - std::get<float>(expr_val_right);
		}
		else {
			throw Runtime_Exception(std::wstring(L"invalid operands for operator (int|float allowed)"), expr.binary_operator);
		}
	
	case token_type::STAR:
		if (check_Integer_Operrands(expr_val_left, expr_val_right)) {
			return std::get<int>(expr_val_left) * std::get<int>(expr_val_right);
		}
		else if (check_Float_Operrands(expr_val_left, expr_val_right)) {
			return std::get<float>(expr_val_left) * std::get<float>(expr_val_right);
		}
		else {
			throw Runtime_Exception(std::wstring(L"invalid operands for operator (int|float allowed)"), expr.binary_operator);
		}

	case token_type::SLASH:
		if (check_Integer_Operrands(expr_val_left, expr_val_right)) {
			return std::get<int>(expr_val_left) / std::get<int>(expr_val_right);
		}
		else if (check_Float_Operrands(expr_val_left, expr_val_right)) {
			return std::get<float>(expr_val_left) / std::get<float>(expr_val_right);
		}
		else {
			throw Runtime_Exception(std::wstring(L"invalid operands for operator (int|float allowed)"), expr.binary_operator);
		}

	case token_type::GREATER:
		if (check_Integer_Operrands(expr_val_left, expr_val_right)) {
			return std::get<int>(expr_val_left) > std::get<int>(expr_val_right);
		}
		else if (check_Float_Operrands(expr_val_left, expr_val_right)) {
			return std::get<float>(expr_val_left) > std::get<float>(expr_val_right);
		}
		else {
			throw Runtime_Exception(std::wstring(L"invalid operands for operator (int|float allowed)"), expr.binary_operator);
		}

	case token_type::LESS:
		if (check_Integer_Operrands(expr_val_left, expr_val_right)) {
			return std::get<int>(expr_val_left) < std::get<int>(expr_val_right);
		}
		else if (check_Float_Operrands(expr_val_left, expr_val_right)) {
			return std::get<float>(expr_val_left) < std::get<float>(expr_val_right);
		}
		else {
			throw Runtime_Exception(std::wstring(L"invalid operands for operator (int|float allowed)"), expr.binary_operator);
		}

	case token_type::GREATER_EQUAL:
		if (check_Integer_Operrands(expr_val_left, expr_val_right)) {
			return std::get<int>(expr_val_left) >= std::get<int>(expr_val_right);
		}
		else if (check_Float_Operrands(expr_val_left, expr_val_right)) {
			return std::get<float>(expr_val_left) >= std::get<float>(expr_val_right);
		}
		else {
			throw Runtime_Exception(std::wstring(L"invalid operands for operator (int|float allowed)"), expr.binary_operator);
		}

	case token_type::LESS_EQUAL:
		if (check_Integer_Operrands(expr_val_left, expr_val_right)) {
			return std::get<int>(expr_val_left) <= std::get<int>(expr_val_right);
		}
		else if (check_Float_Operrands(expr_val_left, expr_val_right)) {
			return std::get<float>(expr_val_left) <= std::get<float>(expr_val_right);
		}
		else {
			throw Runtime_Exception(std::wstring(L"invalid operands for operator (int|float allowed)"), expr.binary_operator);
		}

	case token_type::EQUAL:
		return std::operator==(expr_val_left, expr_val_right);

	case token_type::NOT_EQUAL:
		return !std::operator==(expr_val_left, expr_val_right);

	default:
		throw Runtime_Exception(std::wstring(L"invalid operator for expression (+|-|*|/|>|<|>=|<=|==|!= allowed)"), expr.binary_operator);
	}
}

Value Interpreter::visit_Unary(Unary_Expression& expr) {
	Value expr_val = expr.expr->accept(*this);

	switch (expr.unary_operator.type) {

	case token_type::INVERSE:
		return !is_Truthy(expr_val);

	case token_type::MINUS:

		switch ((value_index)expr_val.index()) {

		case value_index::_int:
			expr_val = -std::get<int>(expr_val);
			return expr_val;

		case value_index::_float:
			expr_val = -std::get<float>(expr_val);
			return expr_val;

		default:
			throw Runtime_Exception(std::wstring(L"type is incompatible with operator"), expr.unary_operator);
		}

	default:
		throw Runtime_Exception(std::wstring(L"unknown unary operator"), expr.unary_operator);
	}

}

Value Interpreter::visit_Grouping(Group_Expression& expr) {
	return expr.expr->accept(*this);
}

Value Interpreter::visit_Literal_Integer(Literal_Expression_Int& expr) {
	Value v;
	v = expr.value;
	return v;
}

Value Interpreter::visit_Literal_Float(Literal_Expression_Float& expr) {
	Value v;
	v = expr.value;
	return v;
}

Value Interpreter::visit_Literal_Wstring(Literal_Expression_Wstring& expr) {
	Value v;
	v = expr.value;
	return v;
}

Value Interpreter::visit_Literal_Bool(Literal_Expression_Bool& expr) {
	Value v;
	v = expr.value;
	return v;
}

Value Interpreter::visit_Literal_Null(Literal_Expression_Null& expr) {
	Value v;
	v = _null();
	return v;
}

Value Interpreter::visit_Variable_Expression(Variable_Expression& expr) {
	return env->get(expr.name);
}

Value Interpreter::visit_Variable_Assignment_Expression(Variable_Assignment_Expression& expr) {
	Value val = expr.expr->accept(*this);
	env->assign(expr.name, val);
	return val;
}

Value Interpreter::visit_Logical_Binary(Binary_Logical_Expression& expr) {
	Value expr_val_left = expr.left->accept(*this);

	if (expr.binary_operator.type == token_type::AND) {
		if (!is_Truthy(expr_val_left)) {
			return expr_val_left;
		}
		else {
			Value expr_val_right = expr.right->accept(*this);
			return expr_val_right;
		}
	}
	else {
		if (is_Truthy(expr_val_left)) {
			return expr_val_left;
		}
		else {
			Value expr_val_right = expr.right->accept(*this);
			return expr_val_right;
		}
	}
}

bool Interpreter::is_Truthy(Value& in_val) {
	switch ((value_index)in_val.index()) {

	case value_index::_int:
		return std::get<int>(in_val) != 0;
	
	case value_index::_float: {
		float f = std::get<float>(in_val);
		return !((f == 0.0f) || (std::isnan(f)));
	}
	
	case value_index::_bool:
		return std::get<bool>(in_val);
	
	case value_index::_wstring:
		return !std::get<std::wstring>(in_val).empty();
	
	case value_index::_null:
		return false;

	default:
		return true;
	}
}

bool Interpreter::is_Equal(Value& left, Value& right) {
	return false;
}

bool Interpreter::check_Integer_Operrands(Value& left, Value& right) {
	return (((value_index)left.index() == value_index::_int) && ((value_index)right.index() == value_index::_int));
}

bool Interpreter::check_Float_Operrands(Value& left, Value& right) {
	return (((value_index)left.index() == value_index::_float) && ((value_index)right.index() == value_index::_float));
}

bool Interpreter::check_String_Operrands(Value& left, Value& right) {
	return (((value_index)left.index() == value_index::_wstring) && ((value_index)right.index() == value_index::_wstring));
}
