#include <stdexcept>

#include "item.h"

namespace config {

ValueType Item::get_value_type() {
	return value_type;
}

std::string Item::get_string() {
	if (value_type != ValueType::STRING) {
		throw std::runtime_error(errors::TYPE_MISMATCH);
	}
	return string_value;
}

bool Item::get_bool() {
	if (value_type != ValueType::BOOLEAN) {
		throw std::runtime_error(errors::TYPE_MISMATCH);
	}
	return boolean_value;
}

int64_t Item::get_int() {
	if (value_type != ValueType::INTEGER) {
		throw std::runtime_error(errors::TYPE_MISMATCH);
	}
	return integer_value;
}

double Item::get_double() {
	if (value_type != ValueType::DOUBLE) {
		throw std::runtime_error(errors::TYPE_MISMATCH);
	}
	return double_value;
}

std::vector<std::string> Item::get_list() {
	if (value_type != ValueType::LIST) {
		throw std::runtime_error(errors::TYPE_MISMATCH);
	}
	return list_value;
}

void Item::set_string(std::string in) {
	value_type = ValueType::STRING;
	string_value = in;
}

void Item::set_bool(bool in) {
	value_type = ValueType::BOOLEAN;
	boolean_value = in;
}

void Item::set_int(int64_t in) {
	value_type = ValueType::INTEGER;
	integer_value = in;
}

void Item::set_double(double in) {
	value_type = ValueType::DOUBLE;
	double_value = in;
}

void Item::set_list(std::vector<std::string> in) {
	value_type = ValueType::LIST;
	list_value = in;
}

} // namespace config
