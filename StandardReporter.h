#pragma once
#include "AbstractReporter.h"

class StandardReporter : public AbstractReporter{
public:
	void report(std::wstring message);
};