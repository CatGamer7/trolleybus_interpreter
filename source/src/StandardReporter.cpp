#include "StandardReporter.h"
#include <iostream>

void StandardReporter::report(std::wstring message) {
	std::wcout << message << std::endl;
}