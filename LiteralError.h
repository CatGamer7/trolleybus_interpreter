#pragma once
#include "Error.h"

class LiteralError : public Error {
public:
	LiteralError(std::wstring in_message, Token in_token) : Error(in_message, in_token) {};
	operator std::wstring() {
		std::wstring printout = L"Literal error: following literal is invalid \"" + token.string + L"\" on line " + std::to_wstring(token.line) + L"(" + message + L")";
		return printout;
	};
};