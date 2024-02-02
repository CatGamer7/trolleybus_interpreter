#pragma once
#include <variant>
#include <string>

struct _null {
	bool operator==(const _null& rhs) const { return true; };
};

typedef std::variant<int, float, bool, std::wstring, _null> Value;

enum class value_index {
	_int = 0,
	_float,
	_bool,
	_wstring,
	_null,
};

std::wstring value_to_string(Value& in_val);
