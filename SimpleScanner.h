#pragma once
#include <unordered_map>
#include "ScannerInterface.h"
#include "LexicalError.h"
#include "LiteralError.h"

class SimpleScanner : public ScannerInterface {
public:
	std::vector<Token> scan(AbstractReporter& reporter, std::wistream& stream);
private:
	const std::unordered_map<std::wstring, token_type> KEYWORDS = {
		{L"for", token_type::FOR},
		{L"while", token_type::WHILE},
		{L"if", token_type::IF},
		{L"else", token_type::ELSE},
		{L"elif", token_type::ELIF},
		{L"true", token_type::TRUE_LITERAL},
		{L"false", token_type::FALSE_LITERAL},
		{L"null", token_type::NULL_LITERAL},
		{L"print", token_type::PRINT},
		{L"var", token_type::VAR}
	};
};