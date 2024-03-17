#pragma once
#include <unordered_map>
#include "Token.h"
#include "Value.h"
#include "RuntimeError.h"

class Environment {
public:
	Environment() : name_map() { outer = nullptr; };
	Environment(std::unique_ptr<Environment>& in_outer) : name_map(), outer(std::move(in_outer)){};

	void declare(const std::wstring& in_name, const Value& in_value) {
		name_map.insert({ in_name, in_value });
	};

	void assign(const Token& in_name, const Value& in_value) {
		auto res = name_map.extract(in_name.string);

		if (res.empty()) {
			if (outer != nullptr) {
				outer->assign(in_name, in_value);
			}
			else {
				throw Runtime_Exception(L"undefined variable referenced", in_name);
			}
		}
		else {
			name_map.insert({ in_name.string, in_value });
		}
	}

	Value& get(const Token& in_name) {
		auto lookup = name_map.find(in_name.string);

		if (lookup == name_map.end()) {
			if (outer != nullptr) {
				return outer->get(in_name);
			}
			else {
				throw Runtime_Exception(L"undefined variable referenced", in_name);
			}
		}
		else {
			return lookup->second;
		}
	};

	std::unique_ptr<Environment> get_outer() {
		return std::move(outer);
	}
private:
	std::unordered_map<std::wstring, Value> name_map;
	std::unique_ptr<Environment> outer = nullptr;
};