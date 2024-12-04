#pragma once
#include <exception>
#include "Error.h"

class SyntaxError : public Error {
public:
	SyntaxError(std::wstring in_message, Token in_token) {
		message = in_message;
		token = in_token;
	}

	operator std::wstring() {
		std::wstring printout = L"Syntax error: " + message + L" on line " + std::to_wstring(token.line) + L" \"" + token.string + L"\"";
		return printout;
	}
protected:
	std::wstring message;
	Token token;
};

class Invalid_Syntax_Exception : public std::exception {
public:
	Invalid_Syntax_Exception(std::wstring in_msg, Token in_token) : error(in_msg, in_token) {};

	std::wstring what() { return (std::wstring)error; };
private:
	SyntaxError error;
};