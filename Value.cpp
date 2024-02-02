#include "Value.h"

std::wstring value_to_string(Value& in_val) {

	switch ((value_index)in_val.index()) {

	case value_index::_int:
		return std::to_wstring(std::get<int>(in_val));

	case value_index::_float:
		return std::to_wstring(std::get<float>(in_val));

	case value_index::_wstring:
		return std::get<std::wstring>(in_val);

	case value_index::_bool:
		if (std::get<bool>(in_val)) {
			return L"true";
		}
		else {
			return L"false";
		}

	case value_index::_null:
		return L"null";

	}
}