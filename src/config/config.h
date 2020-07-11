#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#include <string>

#include "handler.h"

using std::string;

namespace config {

string get_default_config_file_path();
Handler* load_config();
Handler* load_config(string config_file_path);
void make_config_if_not_exists(string config_file_path);

} // namespace config

#endif // CONFIG_CONFIG_H_
