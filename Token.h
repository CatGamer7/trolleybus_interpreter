#pragma once
#include <string>
#include <unordered_map>

enum class token_type {
	INVERSE, EQUAL, NOT_EQUAL, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL, //Boolean
	PLUS, MINUS, STAR, SLASH, //Arithmetic
	LEFT_PARENTHESIS, RIGHT_PARENTHESIS, RIGHT_CURLY, LEFT_CURLY, ASSIGNMENT, SEMICOLON, //Special
	IDENTIFIER, STRING_LITERAL, FLOAT_LITERAL, INTEGER_LITERAL, TRUE_LITERAL, FALSE_LITERAL, NULL_LITERAL, //Literals
	FOR, WHILE, IF, ELSE, ELIF, PRINT, VAR //Keyword statements
};

struct Token {
	Token() {};
	Token(token_type in_type, std::wstring& in_string, int in_line) {
		type = in_type;
		string = in_string;
		line = in_line;
	};
	operator std::wstring() {
		std::wstring out = L"[type=" + std::to_wstring((int)type) + L";string=" + string + L";line=" + std::to_wstring(line) + L"]";
		return out;
	};

	token_type type;
	std::wstring string;
	int line;
};