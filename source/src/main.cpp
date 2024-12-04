#include <iostream>
#include <fstream>
#include <string>
#include "SimpleScanner.h"
#include "StandardReporter.h"
#include "Expression.h"
#include "ExpressionPrinter.h"
#include "Parser.h"
#include "Interpreter.h"

int main(int argc, char* argv[]) {
	if (argc > 3) {
		std::wcout << "Incorrect number of arguments";
		return EXIT_FAILURE;
	} 

	std::string in_file = argv[1];

	std::wifstream in_stream = std::wifstream(in_file);

	auto scanner = SimpleScanner();
	auto error_reporter = StandardReporter();
	auto token_vector = scanner.scan(error_reporter, in_stream);

	if (scanner.check_errors()) {
		return EXIT_FAILURE;
	}

	Parser parser = Parser(token_vector);
	auto stmts = parser.parse(error_reporter);

	if (parser.check_errors()) {
		return EXIT_FAILURE;
	}

	Interpreter interpreter = Interpreter();
	interpreter.interpret(*stmts, error_reporter);
}