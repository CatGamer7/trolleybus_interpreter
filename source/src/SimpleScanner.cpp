#include "SimpleScanner.h"
#include <cwctype>

std::vector<Token> SimpleScanner::scan(AbstractReporter& reporter, std::wistream& stream) {

	std::vector<Token> token_vector;

	wchar_t check = stream.peek();
	while (!stream.eof()) {
		wchar_t c = stream.get();
		std::wstring token_string = L"";
		token_string += c;
		Token t;
		wchar_t next = stream.peek();
		bool incomplete = true;

		switch (c) {
		case '(':
			t.type = token_type::LEFT_PARENTHESIS;
			break;

		case ')':
			t.type = token_type::RIGHT_PARENTHESIS;
			break;

		case '{':
			t.type = token_type::LEFT_CURLY;
			break;

		case '}':
			t.type = token_type::RIGHT_CURLY;
			break;

		case '+':
			t.type = token_type::PLUS;
			break;

		case '-':
			t.type = token_type::MINUS;
			break;

		case '*':
			t.type = token_type::STAR;
			break;

		case '/':
			t.type = token_type::SLASH;
			break;

		case '!':
			if (next == L'=') {
				token_string += next;
				stream.get();
				t.type = token_type::NOT_EQUAL;
			}
			else {
				t.type = token_type::INVERSE;
			}
			break;

		case '=':
			if (next == L'=') {
				token_string += next;
				stream.get();
				t.type = token_type::EQUAL;
			}
			else {
				t.type = token_type::ASSIGNMENT;
			}
			break;

		case '>':
			if (next == L'=') {
				token_string += next;
				stream.get();
				t.type = token_type::GREATER_EQUAL;
			}
			else {
				t.type = token_type::GREATER;
			}
			break;

		case '<':
			if (next == L'=') {
				token_string += next;
				stream.get();
				t.type = token_type::LESS_EQUAL;
			}
			else {
				t.type = token_type::LESS;
			}
			break;

		case '\n':
			current_line++;
			continue;

		case ';':
			t.type = token_type::SEMICOLON;
			break;
		
		case '"':
			t.type = token_type::STRING_LITERAL;
			while (!stream.eof()) {
				wchar_t possible_string = stream.get();
				token_string += possible_string;
				if (possible_string == L'"') {
					incomplete = false;
					break;
				}
				else if (possible_string == '\n') {
					current_line++;
				}
			}
			if (incomplete) {
				error = true;
				reporter.report(LexicalError(L"string was never closed", t));
			}
			break;

		default:
			if (std::iswspace(c)) {
				continue;
			}
			if (std::iswdigit(c)) {
				unsigned int dots = 0;
				while (!stream.eof()) {
					wchar_t possible_number = stream.peek();
					if (possible_number == L'.') {
						dots++;
						token_string += stream.get();
						if (!std::iswdigit(stream.peek())) {
							error = true;
							reporter.report(LiteralError(L"missing fractional part", t));
							break;
						}
					}
					else if (std::iswdigit(possible_number)) {
						token_string += stream.get();
					}
					else {
						break;
					}
				}
				switch (dots)
				{

				case 0:
					t.type = token_type::INTEGER_LITERAL;
					break;
				case 1:
					t.type = token_type::FLOAT_LITERAL;
					break;
				default:
					error = true;
					reporter.report(LiteralError(L"too many decimal separators", t));
					break;
				}
			}
			else if (std::iswalpha(c)) {
				while (!stream.eof()) {
					wchar_t possible_char = stream.peek();
					if (std::iswdigit(possible_char) || std::iswalpha(possible_char) || (possible_char == L'_')) {
						token_string += stream.get();
					}
					else {
						break;
					}
				}
				auto search_res = KEYWORDS.find(token_string);
				if (search_res != KEYWORDS.end()) {
					t.type = search_res->second;
				}
				else {
					t.type = token_type::IDENTIFIER;
				}
			}
			break;
		}

		t.string = token_string;
		t.line = this->current_line;

		token_vector.push_back(t);
	}

	return token_vector;
}
