#pragma once
#include <string>

class AbstractReporter {
public:
	virtual void report(std::wstring message) = 0;
};