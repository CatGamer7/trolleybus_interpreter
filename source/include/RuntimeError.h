#pragma once
#include <exception>
#include "Error.h"

class RuntimeError : public Error {
public:
	RuntimeError(std::wstring in_message, Token in_token) {
		message = in_message;
		token = in_token;
	}

	operator std::wstring() {
		std::wstring printout = L"Runtime error: " + message + L". token: \"" + token.string + L"\"" + L" on line " + std::to_wstring(token.line);
		return printout;
	}
protected:
	std::wstring message;
	Token token;
};

class Runtime_Exception : public std::exception {
public:
	Runtime_Exception(std::wstring in_msg, Token& in_token) : error(in_msg, in_token) {};
	Runtime_Exception(std::wstring in_msg, const Token& in_token) : error(in_msg, in_token) {};
	std::wstring what() { return (std::wstring)error; };
private:
	RuntimeError error;
};