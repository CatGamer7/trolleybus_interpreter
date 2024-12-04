#include "ExpressionPrinter.h"
#include <iostream>
#include <string>

Value ExpressionPrinter::visit_Binary(Binary_Expression& expr) {
	std::wcout << L"(" << expr.binary_operator.string << L" ";
	expr.left->accept(*this);
	std::wcout << L" ";
	expr.right->accept(*this);
	std::wcout << L")";
	return { 0 };
}

Value ExpressionPrinter::visit_Unary(Unary_Expression& expr) {
	std::wcout << L"(" << expr.unary_operator.string << L" ";
	expr.expr->accept(*this);
	std::wcout << L" )";
	return { 0 };
}

Value ExpressionPrinter::visit_Grouping(Group_Expression& expr) {
	std::wcout << L"( ";
	expr.expr->accept(*this);
	std::wcout << L" )";
	return { 0 };
}

Value ExpressionPrinter::visit_Literal_Integer(Literal_Expression_Int& expr) {
	std::wcout << std::to_wstring(expr.value);
	return { 0 };
}

Value ExpressionPrinter::visit_Literal_Float(Literal_Expression_Float& expr) {
	std::wcout << std::to_wstring(expr.value);
	return { 0 };
}

Value ExpressionPrinter::visit_Literal_Wstring(Literal_Expression_Wstring& expr) {
	std::wcout << expr.value;
	return { 0 };
}

Value ExpressionPrinter::visit_Literal_Bool(Literal_Expression_Bool& expr) {
	if (expr.value) {
		std::wcout << L"true";
	}
	else {
		std::wcout << L"false";
	}
	return { 0 };
}

Value ExpressionPrinter::visit_Literal_Null(Literal_Expression_Null& expr) {
	std::wcout << L"null";
	return { 0 };
}

