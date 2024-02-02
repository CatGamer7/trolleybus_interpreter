#pragma once
#include "Expression.h"

class ExpressionPrinter : public ExpressionVisitor {
	Value visit_Binary(Binary_Expression& expr) override;
	Value visit_Unary(Unary_Expression& expr) override;
	Value visit_Grouping(Group_Expression& expr) override;
	Value visit_Literal_Integer(Literal_Expression_Int& expr) override;
	Value visit_Literal_Float(Literal_Expression_Float& expr) override;
	Value visit_Literal_Wstring(Literal_Expression_Wstring& expr) override;
	Value visit_Literal_Bool(Literal_Expression_Bool& expr) override;
	Value visit_Literal_Null(Literal_Expression_Null& expr) override;
};
