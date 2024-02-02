#pragma once
#include "Error.h"

class LexicalError : public Error {
public:
	LexicalError(std::wstring in_message, Token in_token) : Error(in_message, in_token) {};
	operator std::wstring() {
		std::wstring printout = L"Lexical error: invalid lexeme \"" + token.string + L"\" on line " + std::to_wstring(token.line) + L"(" + message +L")";
		return printout;
	};
};