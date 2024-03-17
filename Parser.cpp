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
	std::unique_ptr<Expression> left = or_expr();
	if (!end()) {
		if (peek().type == token_type::ASSIGNMENT) {
			Token op = get();
			std::unique_ptr<Expression> right = expression();

			auto type_visitor = Expression_Type_Visitor();
			if ((Expression_Type)std::get<int>(left->accept(type_visitor)) == Expression_Type::Variable) { //To hell with downcast, return Token with Visitor pattern

				auto string_visitor = Expression_Token_String_Visitor();
				std::wstring token_string = std::get<std::wstring>(left->accept(string_visitor));
				Token assignment_token = Token(token_type::IDENTIFIER, token_string, op.line);

				return std::make_unique<Variable_Assignment_Expression>(Variable_Assignment_Expression(assignment_token, right));
			}

			else {
				throw Invalid_Syntax_Exception(std::wstring(L"invalid assignment target"), op);
			}
		}
	}
	return left;
}

expr_ptr Parser::or_expr() {
	expr_ptr left = and_expr();

	while (peek().type == token_type::OR) {
		Token op = get(); //consume "or"

		expr_ptr right = and_expr();
		left = std::make_unique<Binary_Logical_Expression>(Binary_Logical_Expression(op, left, right));
	}

	return left;
}

expr_ptr Parser::and_expr() {
	expr_ptr left = equality();

	while (peek().type == token_type::AND) {
		Token op = get(); //consume "and"

		expr_ptr right = equality();
		left = std::make_unique<Binary_Logical_Expression>(Binary_Logical_Expression(op, left, right));
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
	stmt_ptr stmt;

	switch (peek().type)
	{
	case token_type::PRINT:
		stmt = statement_print();
		break;

	case token_type::LEFT_CURLY:
		stmt = block();
		break;

	case token_type::FOR:
		stmt = for_stmt();
		break;

	case token_type::WHILE:
		stmt = while_stmt();
		break;

	case token_type::IF:
		stmt = if_stmt();
		break;

	default:
		stmt = statement_expr();
		break;
	}

	return stmt;
}

stmt_ptr Parser::block() {
	get(); //consume left curly

	std::vector<stmt_ptr> block_stmts;
	while ((!end()) && (peek().type != token_type::RIGHT_CURLY)) {
		block_stmts.push_back(std::move(declaration()));
	}

	if (!end()) {
		get(); //consume right curly
	}
	else {
		throw Invalid_Syntax_Exception(L"closing token \"}\" expected", previous());
	}

	return std::make_unique<Statement_Scope_Block>(block_stmts);
}

stmt_ptr Parser::while_stmt() {
	get(); //consume "while"

	if (!end() && (peek().type == token_type::LEFT_PARENTHESIS)) {
		get(); //consume "("

		expr_ptr cond = expression();

		if (!end() && (peek().type == token_type::RIGHT_PARENTHESIS)) {
			get(); //consume ")"
		}
		else {
			throw Invalid_Syntax_Exception(L"closing token \")\" expected", previous());
		}

		stmt_ptr body = statement();

		return std::make_unique<Statement_While>(Statement_While(cond, body));
	}
	else {
		throw Invalid_Syntax_Exception(L"opening token \"(\" expected before condition", previous());
	}
}

stmt_ptr Parser::for_stmt() {
	get(); //consume "for"


	if (!end() && (peek().type == token_type::LEFT_PARENTHESIS)) {
		get(); //consume "("

		stmt_ptr init = declaration();

		expr_ptr cond = expression(); //for "while" compatibility

		if (!end() && (peek().type == token_type::SEMICOLON)) {
			get(); //consume ";"
		}
		else {
			throw Invalid_Syntax_Exception(L"separating token \";\" after condition expected", previous());
		}

		expr_ptr increment = expression();

		if (!end() && (peek().type == token_type::RIGHT_PARENTHESIS)) {
			get(); //consume ")"
		}
		else {
			throw Invalid_Syntax_Exception(L"closing token \")\" expected", previous());
		}

		stmt_ptr body = statement();

		return std::make_unique<Statement_For>(Statement_For(init, cond, increment, body));
	}
	else {
		throw Invalid_Syntax_Exception(L"opening token \"(\" expected before condition", previous());
	}

}

stmt_ptr Parser::if_stmt() {
	get(); //consume "if"
	
	if (!end() && (peek().type == token_type::LEFT_PARENTHESIS)) {
		get(); //consume "("

		expr_ptr cond = expression();
		stmt_ptr if_branch;

		if (!end() && (peek().type == token_type::RIGHT_PARENTHESIS)) {
			get(); //consume ")"

			if_branch = statement();
		}
		else {
			throw Invalid_Syntax_Exception(L"closing token \")\" expected", previous());
		}

		if (!end() && (peek().type == token_type::ELSE)) {
			get(); //consume "else"

			stmt_ptr else_branch = statement();

			return std::make_unique<Statement_If>(Statement_If(cond, if_branch, else_branch));
		}

		return std::make_unique<Statement_If>(Statement_If(cond, if_branch));
	}
	else {
		throw Invalid_Syntax_Exception(L"opening token \"(\" expected before condition", previous());
	}
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
