#pragma once
#include <vector>
#include <iostream>
#include "Token.h"
#include "AbstractReporter.h"

class ScannerInterface {
public:
	virtual std::vector<Token> scan(AbstractReporter& reporter, std::wistream& stream) = 0;
	bool check_errors() {
		return error;
	};
protected:
	int current_line = 1;
	bool error = false;
};