#pragma once
#include <unordered_map>
#include "Token.h"
#include "Value.h"
#include "RuntimeError.h"

class Environment {
public:
	void declare(const std::wstring& in_name, const Value& in_value) {
		name_map.insert({ in_name, in_value });
	};

	void assign(const Token& in_name, const Value& in_value) {
		auto res = name_map.extract(in_name.string);

		if (res.empty()) {
			throw Runtime_Exception(L"undefined variable referenced", in_name);
		}
		else {
			name_map.insert({ in_name.string, in_value });
		}
	}

	Value& get(const Token& in_name) {
		auto lookup = name_map.find(in_name.string);

		if (lookup == name_map.end()) {
			throw Runtime_Exception(L"undefined variable referenced", in_name);
		}
		else {
			return lookup->second;
		}
	};
private:
	std::unordered_map<std::wstring, Value> name_map;
};