#ifndef CONFIG_PARSER_H_
#define CONFIG_PARSER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "item.h"

namespace config {

using std::string;
using std::unordered_map;
using std::vector;

struct SingleSetting {
	public:
		string section;
		string key;
		string override;
		string value;
};

class Parser {
	public:
		vector<string> parse_file(string);
		string parse_section(string);
		SingleSetting parse_setting(string, string);

		void escape_char(string&, const string&, const string&);
		bool is_section_valid(string);
		string strip_line(string);

		Item make_value_item(string);
};


} // namespace config

#endif // CONFIG_PARSER_H_
