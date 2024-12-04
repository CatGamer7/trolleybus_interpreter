#pragma once
#include <string>
#include "Token.h"

class Error {
public:
	Error() {};
	Error(std::wstring in_message, Token in_token) {
		message = in_message;
		token = in_token;
	}
	virtual operator std::wstring() = 0;
protected:
	std::wstring message;
	Token token;
};