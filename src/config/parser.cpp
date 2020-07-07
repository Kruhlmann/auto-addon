#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "constants.h"
#include "parser.h"

using std::string;
using std::unordered_map;
using std::vector;

namespace config {

vector<string> Parser::parse_file(string filename) {
	// Try to open file
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error(errors::FILE_OPEN);
	}

	// Read each line
	vector<string> lines;
	string line;
	while (std::getline(file, line)) {
		lines.push_back(line);
	}
	return lines;
}

void Parser::escape_char(string& str, const string& from, const string& to) {
	size_t start_pos = str.find(from);
	if(start_pos == string::npos) {
		return;
	}
	str.replace(start_pos, from.length(), to);
}

string Parser::strip_line(string input) {
	string out = "";
	bool in_quotes = false;

	// Replace weird quotes with standard UTF-8 versions
	this->escape_char(input, "\u201C","\u0022"); // replaces “
	this->escape_char(input, "\u201D","\u0022"); // replaces ”

	for (const auto& c : input) {
        // If we encounter comment delimiter, skip rest of line
        // (but only if we're not inside quotes)
        if (c == constants::COMMENT_DELIM && !in_quotes) {
            break;
        }

        // Did we enter/leave a quote block?
        // Toggle flag, and normalize quote.
        if (c == constants::QUOTE) {
            in_quotes = !in_quotes;
            out += constants::QUOTE;
            continue;
        }

        // If not in quote block, skip spaces
        if (!in_quotes) {
            if (c == constants::SPACE) {
                continue;
            }
        }

        // Add character to out string
        out += c;
	}

	return out;
}

bool Parser::is_section_valid(string input) {
	// Note: all whitespaces have been stripped
	// A valid section must be at least 3 characters long
	// Example: "[a]"
	if (input.length() < 3) {
		return false;
	}

	// First character must be '['
	if (input[0] != constants::SECTION_START) {
		return false;
	}

	// Last character must be ']'
	if (input[input.length() - 1] != constants::SECTION_END) {
		return false;
	}

	// Check remaining substring for permitted characters
	for (unsigned int i = 1; i < input.length() - 1; i++) {
		const char& c = input[i];
        const bool not_hypen = c != constants::HYPHEN;
        const bool not_underscore = c != constants::UNDERSCORE;
		if (!std::isalnum(c) && not_hypen && not_underscore) {
			return false;
		}
	}

	return true;
}

string Parser::parse_section(string input) {
	// Note: all whitespaces have been stripped
	// Lines are expected to be valid sections
	string section = input.substr(1, input.length() - 2);
	return section;
}

SingleSetting Parser::parse_setting(string input, string section) {
	// Note: all whitespaces have been stripped
	// Lines are expected to be settings, not sections

	// Prepare default, empty setting
	SingleSetting setting;
	setting.section = "";
	setting.key = "";
	setting.override = "";
	setting.value = "";

	// A valid setting line must be at least 3 characters long
	// Example: "a=b"
	if (input.length() < 3) {
		return setting;
	}

	// Split string into key and value separated by equals
	string::size_type pos = input.find(constants::EQUALS);
	if (pos == string::npos || pos == 0 || pos == input.length()-1) {
		// Either no equals found or it is the first/last character
		// Return as invalid
		return setting;
	}

	// Grab everything on the left of quals
	string left = input.substr(0, pos);

	// Search for override
	string::size_type o_start = left.find(constants::OVERRIDE_START);
	string::size_type o_end = left.find(constants::OVERRIDE_END);
	bool found_start = (o_start != string::npos);
	bool found_end = (o_end != string::npos);
	if (found_start != found_end) {
		// Reject if we found one but not the other
		return setting;
	}
	if (found_start && (o_start == 0 || o_end != left.length()-1)) {
		// In order for a valid override, there must be:
		// - exactly one <, not at position 0 of left
		// - exactly one >, at position length()-1 of left
		return setting;
	}
	string override = "";
	if (found_start) {
		for (unsigned int i = o_start+1; i < o_end; i++) {
			const char& c = left[i];
            const bool not_hypen = c != constants::HYPHEN;
            const bool not_underscore = c != constants::UNDERSCORE;
			if (!std::isalnum(c) && not_hypen && not_underscore) {
				// Only allow any combination of alphanum, hyphen, and
                // underscore.
				std::cout << left[i+1] << "\t";
				return setting;
			} else {
				override += c;
			}
		}
	}

	// We liberally allow all remaining string permutations for the key & value.
	// This means the string can contain additional equal signs, and special
	// characters. This is a known design decision that can be expanded
	// with additional time. A future improvement would thus be to build
	// helper functions that validate the key and value values below.

	// Populate key
	string key = "";
	if (found_start) {
		key = input.substr(0, o_start);
	} else {
		key = input.substr(0, pos);
	}

	// Grab everything on the right of equals as value
	string value = input.substr(pos + 1);

	// If we reached here, line is a valid setting
	setting.section = section;
	setting.key = key;
	setting.override = override;
	setting.value = value;
	return setting;
}

config::Item Parser::make_value_item(string input) {
	// Note: all whitespaces have been stripped except inside quotes.
	config::Item config_item;

	// An empty string should never be sent to this function, but
	// early return to avoid accessing an index that doesn't exist later.
	if (input.length() == 0) {
		config_item.set_string(input);
		return config_item;
	}

	// Early return for quoted strings. Strip quotes before storing.
	// This is by design. When callers ask for the setting, it shouldn't
	// include the quotes -- the quotes are there to demonstrate intent that
	// the type within is a string.
	if (input[0] == constants::QUOTE) {
		string stripped_string = input.substr(1, input.length()- 2);
		config_item.set_string(stripped_string);
		return config_item;
	}

	// Check if string is a number
	bool is_number = true;
	bool found_sign = false;
	bool found_decimal = false;
	// Is it a signed number?
	if (input[0] == constants::HYPHEN) {
		found_sign = true;
	}
	// Check if remaining numbers are only digits.
	// Only one decimal dot allowed, and no more signs allowed.
	unsigned int i = found_sign ? 1 : 0;
	for (; i < input.length(); i++) {
		const auto& c = input[i];
		if (c == constants::DECIMAL) {
			if (found_decimal) {
				// Already found a decimal point, this is not a valid number.
				is_number = false;
				break;
			}
			found_decimal = true;
			continue;
		}
		if (!isdigit(c)) {
			is_number = false;
			break;
		}
	}

	// Try to convert to number
	if (is_number) {
		if (found_decimal) {
			try {
				double double_value = std::stod(input);
				config_item.set_double(double_value);
				return config_item;
			} catch (std::exception e) {
				throw std::runtime_error(errors::SETTING_MAX_DOUBLE);
			}
		} else {
			try {
				int64_t int_value = std::stoll(input);
				config_item.set_int(int_value);
				return config_item;
			} catch (std::exception e) {
				throw std::runtime_error(errors::SETTING_MAX_INTEGER);
			}
		}
	}

	// If we reached here, we have ruled out number.
	// Next up is Bool. As a design decision, in order for the config item
	// to write itself as a boolean primitive, we will support the following
	// strings as read from the raw config file. In order to avoid ambiguity,
	// "1" and "0" without quotes will always be considered numbers, and always
	// considered strings with quotes.
	unordered_map<string, bool> permitted_bool_values = {
		{"yes", true},
		{"no", false},
		{"true", true},
		{"false", false}
	};
	if (permitted_bool_values.count(input) > 0) {
		// Found a match for bool
		config_item.set_bool(permitted_bool_values[input]);
		return config_item;
	}

	// The only remaining supported types are string and list of strings.
	// Check if the input string is a list of comma-separated values.
	// As a design decision, we do not permit empty strings in the list.
	string token;
	vector<string> list;
	std::istringstream ss(input);
	while(std::getline(ss, token, constants::COMMA)) {
		if (token != "") {
			list.push_back(token);
		}
	}
	if (list.size() > 1) {
		config_item.set_list(list);
		return config_item;
	}

	// If we reached here, the only remaining supported type is string.
	config_item.set_string(input);
	return config_item;
}

} // namespace config
