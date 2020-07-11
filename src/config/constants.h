#ifndef CONFIG_CONSTANTS_H_
#define CONFIG_CONSTANTS_H_

#include <string>

using std::string;

namespace constants {

const char SPACE = ' ';
const char HYPHEN = '-';
const char UNDERSCORE = '_';
const char EQUALS = '=';
const char COMMENT_DELIM = ';';
const char SECTION_START = '[';
const char SECTION_END = ']';
const char QUOTE = '\"';
const char OVERRIDE_START = '<';
const char OVERRIDE_END = '>';
const char DECIMAL = '.';
const char COMMA = ',';

const string DEFAULT_CONFIG_FILENAME = "config.ini";
const char* DEFAULT_CONFIG_CONTENTS =
    "# Auto generated configuration file\n"
    "[fs]\n"
    "addons_folder=~/Games/lutris/battlenet/drive_c/"
    "Program Files (x86)/World of Warcraft/_classic_/Interface/AddOns\n";

} // namespace constants

#endif // CONFIG_CONSTANTS_H_
