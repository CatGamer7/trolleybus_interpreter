#include "Parser.h"

Parser::Parser(std::vector<Token>& in_tokens){
	tokens = in_tokens;
	current = tokens.begin();
}

std::unique_ptr< std::vector<stmt_ptr> > Parser::parse(AbstractReporter& reporter) {
	std::unique_ptr< std::vector<stmt_ptr> > out = std::make_unique< std::vector<stmt_ptr> >();

	while (!end()) {
		try {
			out->push_back(declaration());
		}
		catch (Invalid_Syntax_Exception e) {
			errors = true;
			reporter.report(e.what());
			synchronize();
		}
	}

	return out;
}

std::unique_ptr<Expression> Parser::expression() {
	return assignment();
}

expr_ptr Parser::assignment() {
	std::unique_ptr<Expression> left = equality();
	if (!end()) {
		if (peek().type == token_type::ASSIGNMENT) {
			Token op = get();
			std::unique_ptr<Expression> right = expression();

			auto type_visitor = Expression_Type_Visitor();
			if ((Expression_Type)std::get<int>(left->accept(type_visitor)) == Expression_Type::Variable) { //To hell with downcast, return Token with Visitor pattern
				//std::unique_ptr<Variable_Expression> var_expr = std::unique_ptr<Variable_Expression>(dynamic_cast<Variable_Expression*>(left.get()));

				return std::make_unique<Variable_Assignment_Expression>(Variable_Assignment_Expression());
			}

			else {
				throw Invalid_Syntax_Exception(std::wstring(L"invalid assignment target"), op);
			}
		}
	}
	return left;
}

std::unique_ptr<Expression> Parser::equality() {
	std::unique_ptr<Expression> left = comparison();
	while (peek_match({ token_type::EQUAL, token_type::NOT_EQUAL })) {
		if (!end()) {
			Token op = get();
			std::unique_ptr<Expression> right = comparison();
			left = std::make_unique<Binary_Expression>(Binary_Expression(op, left, right));
		}
	}
	return left;
}

std::unique_ptr<Expression> Parser::comparison() {
	std::unique_ptr<Expression> left = term();
	while (peek_match({ token_type::GREATER, token_type::GREATER_EQUAL, token_type::LESS, token_type::LESS_EQUAL })) {
		if (!end()) {
			Token op = get();
			std::unique_ptr<Expression> right = term();
			left = std::make_unique<Binary_Expression>(Binary_Expression(op, left, right));
		}
	}
	return left;
}

std::unique_ptr<Expression> Parser::term() {
	std::unique_ptr<Expression> left = factor();
	while (peek_match({ token_type::PLUS, token_type::MINUS })) {
		if (!end()) {
			Token op = get();
			std::unique_ptr<Expression> right = factor();
			left = std::make_unique<Binary_Expression>(Binary_Expression(op, left, right));
		}
	}
	return left;
}

std::unique_ptr<Expression> Parser::factor() {
	std::unique_ptr<Expression> left = unary();
	while (peek_match({ token_type::STAR, token_type::SLASH })) {
		if (!end()) {
			Token op = get();
			std::unique_ptr<Expression> right = unary();
			left = std::make_unique<Binary_Expression>(Binary_Expression(op, left, right));
		}
	}
	return left;
}

std::unique_ptr<Expression> Parser::unary() {
	if (peek_match({ token_type::INVERSE, token_type::MINUS })) {
		if (!end()) {
			Token op = get();
			std::unique_ptr<Expression> right = unary();
			return std::make_unique<Unary_Expression>(Unary_Expression(op, right));
		}
	}

	return primary();
}

std::unique_ptr<Expression> Parser::primary() {
	if (!end()) {
		Token current = get();

		switch (current.type) {

		case token_type::TRUE_LITERAL:
			return std::make_unique<Literal_Expression_Bool>(Literal_Expression_Bool(true));

		case token_type::FALSE_LITERAL:
			return std::make_unique<Literal_Expression_Bool>(Literal_Expression_Bool(false));

		case token_type::NULL_LITERAL:
			return std::make_unique<Literal_Expression_Null>(Literal_Expression_Null());

		case token_type::STRING_LITERAL:
			return std::make_unique<Literal_Expression_Wstring>(Literal_Expression_Wstring(current.string));

		case token_type::INTEGER_LITERAL:
			return std::make_unique<Literal_Expression_Int>(Literal_Expression_Int(std::stoi(current.string)));

		case token_type::FLOAT_LITERAL:
			return std::make_unique<Literal_Expression_Float>(Literal_Expression_Float(std::stof(current.string)));

		case token_type::IDENTIFIER:
			return std::make_unique<Variable_Expression>(current);

		case token_type::LEFT_PARENTHESIS:
		{
			std::unique_ptr<Expression> expr = expression();
			if (get().type == token_type::RIGHT_PARENTHESIS) {
				return std::make_unique<Group_Expression>(Group_Expression(expr));
			}
			else {
				throw Invalid_Syntax_Exception(std::wstring(L"parenthesis never closed"), current);
			}
		}

		default:
			throw Invalid_Syntax_Exception(std::wstring(L"invalid token type"), current);

		}

	}
}

bool Parser::check_errors() const {
	return errors;
}

stmt_ptr Parser::declaration() {
	if (peek().type == token_type::VAR) {
		return variable_declaration();
	}
	else {
		return statement();
	}
}

stmt_ptr Parser::variable_declaration() {
	get(); //Consume var
	if (!end()) {
		if (peek().type != token_type::IDENTIFIER) {
			throw Invalid_Syntax_Exception(L"expected identifier", peek());
		}

		Token name = get();

		if (peek().type != token_type::ASSIGNMENT) {
			expr_ptr uninitialized = nullptr;
			return std::make_unique<Statement_Variable_Declaration>(Statement_Variable_Declaration(uninitialized, name));
		}
		else {
			get(); //Consume assignment

			expr_ptr initializer = expression();

			if (!end()) {
				if (peek().type != token_type::SEMICOLON) {
					throw Invalid_Syntax_Exception(L"expected ;", previous());
				}

				get(); //Consume semicolon
				return std::make_unique<Statement_Variable_Declaration>(initializer, name);
			}
			else {
				throw Invalid_Syntax_Exception(L"unexpected program termination", previous());
			}
		}
	}
	else {
		throw Invalid_Syntax_Exception(L"unexpected program termination", previous());
	}
}

stmt_ptr Parser::statement() {
	stmt_ptr expr;

	if (peek().type == token_type::PRINT) {
		expr = statement_print();
	}
	else {
		expr = statement_expr();
	}

	return expr;
}

stmt_ptr Parser::statement_expr() {
	expr_ptr expr = expression();

	if (!end()) {
		if (peek().type != token_type::SEMICOLON) {
			throw Invalid_Syntax_Exception(L"expected ;", previous());
		}

		get(); //Consume semicolon
		return std::make_unique<Statement_Expression>(Statement_Expression(expr));
	}
	else {
		throw Invalid_Syntax_Exception(L"unexpected program termination", previous());
	}
}

stmt_ptr Parser::statement_print() {
	get(); //Consume print
	expr_ptr expr = expression();

	if (!end()) {
		if (peek().type != token_type::SEMICOLON) {
			throw Invalid_Syntax_Exception(L"expected ;", previous());
		}

		get(); //Consume semicolon
		return std::make_unique<Statement_Print>(Statement_Print(expr));
	}
	else {
		throw Invalid_Syntax_Exception(L"unexpected program termination", previous());
	}
}

void Parser::synchronize() {
	while (!end()) {
		if (get().type == token_type::SEMICOLON) {
			return;
		}
	}
	return;
}

Token Parser::previous() const {
	return *(current-1);
}

Token Parser::peek() const {
	return *current;
}

Token Parser::get() {
	return *(current++);
}

bool Parser::end() const {
	return current >= tokens.end();
}

bool Parser::peek_match(std::initializer_list<token_type> list) const {
	if (!end()) {
		for (auto t : list) {
			if (peek().type == t) {
				return true;
			}
		}
	}
	return false;
}
