#include "handler.h"

namespace config {

using std::string;
using std::unordered_map;
using std::vector;

const char SECTION_DELIM = '.';

bool Handler::load(string filename) {
    return Handler::load(filename, {});
}

bool Handler::load(string filename, vector<string> overrides) {
	config::Parser parser;

	// Make a map of overrides for quick look-up
	unordered_map<string, bool> overrides_map;
	for (const auto& override : overrides) {
		overrides_map[override] = true;
	}

	// Make another temporary map to store only keys which
	// were overriden.
	unordered_map<string, bool> overriden_keys;

	// Step 1: Open file and convert it into a
	// vector of strings for each line.
	vector<string> lines = parser.parse_file(filename);

	string section = "";
	// For each line in the config file...
	for (auto line : lines) {
		// Step 2: Strip the line of all whitespace and comments.
		// Note: this will not strip whitespaces inside quoted values.
		line = parser.strip_line(line);

		// Skip empty lines.
		if (line.length() == 0) {
			continue;
		}

		if (parser.is_section_valid(line)) {
			// Step 3: Is this line a valid section? If so, extract and set.
			section = parser.parse_section(line);
		} else {
			// Step 4: Parse the setting into disparate strings.
			// SingleSetting contains: section, key, override, and value.
			config::SingleSetting setting = parser.parse_setting(line, section);

			// Return false if this is a malformed setting.
			if (setting.key == "") {
				return false;
			}

			// Step 5: Construct a Item object from the value string.
			config::Item final_value = parser.make_value_item(setting.value);

			// Step 6: Compute concatenated section key
			string section_key = setting.section + SECTION_DELIM + setting.key;

			// Step 7: Process overrides
			bool is_override = (overrides_map.count(setting.override) > 0);
			if (is_override) {
				// Current is override, remember that we processed this
				overriden_keys[section_key] = true;
			} else {
				// Current isn't override, but is this a key that was
				// previously overriden? If so, don't do anything.
				if (overriden_keys.count(section_key) > 0) {
					continue;
				}
			}

			// Step 8: Now that we have a config item, add but only if
			// override matches or is none.
			if (is_override || setting.override == "") {
				// -- A. Individual Map for fast access of setting
				settings_single[section_key] = final_value;

				// -- B. Section Map for fast access of section
				if (settings_section.count(section) == 0) {
					settings_section[section] = unordered_map<string, config::Item>();
				}
				settings_section[section][setting.key] = final_value;
			}
		}
	}
	return true;
}

config::Item* Handler::get(string key) {
	if (settings_single.count(key) > 0) {
		return &settings_single[key];
	}
	return NULL;
}

unordered_map<string, config::Item>* Handler::get_section(string section) {
	if (settings_single.count(section) > 0) {
		return &settings_section[section];
	}
	return NULL;
}

} // namespace config
