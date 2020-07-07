#ifndef CONFIG_ITEM_H_
#define CONFIG_ITEM_H_

#include <iostream>
#include <vector>

#include "errors.h"

namespace config {

// The different types supported by a config item.
enum ValueType {
	STRING,
	BOOLEAN,
	INTEGER,
	DOUBLE,
	LIST
};

// A single configuration item with its value type.
class Item {
	public:
		ValueType get_value_type();
		std::string get_string();
		bool get_bool();
		int64_t get_int();
		double get_double();
		std::vector<std::string> get_list();

		void set_string(std::string);
		void set_bool(bool);
		void set_int(int64_t);
		void set_double(double);
		void set_list(std::vector<std::string>);

	private:
		ValueType value_type;
		std::string string_value;
		bool boolean_value;
		int64_t	integer_value;
		double double_value;
		std::vector<std::string> list_value;
};

} // namespace config

#endif // CONFIG_ITEM_H_
