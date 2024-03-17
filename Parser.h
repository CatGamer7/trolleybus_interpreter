#pragma once
#include <vector>
#include "Token.h"
#include "Statement.h"
#include "AbstractReporter.h"
#include "SyntaxError.h"

class Parser {
public:
	Parser(std::vector<Token>& in_tokens);
	std::unique_ptr< std::vector<stmt_ptr> > parse(AbstractReporter& reporter);
	bool check_errors() const;
	
private:
	stmt_ptr declaration();
	stmt_ptr variable_declaration();
	stmt_ptr statement();
	stmt_ptr block();
	stmt_ptr while_stmt();
	stmt_ptr for_stmt();
	stmt_ptr if_stmt();
	stmt_ptr statement_expr();
	stmt_ptr statement_print();

	expr_ptr expression();
	expr_ptr assignment();
	expr_ptr or_expr();
	expr_ptr and_expr();
	expr_ptr equality();
	expr_ptr comparison();
	expr_ptr term();
	expr_ptr factor();
	expr_ptr unary();
	expr_ptr primary();

	void synchronize();

	Token previous() const;
	Token peek() const;
	Token get();
	bool end() const;
	bool peek_match(std::initializer_list<token_type> list) const;


	std::vector<Token> tokens;
	std::vector<Token>::iterator current;

	bool errors = false;
};