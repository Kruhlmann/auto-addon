#include <iostream>
#include <cstdio>
#include <ostream>
#include <string>
#include <sstream>
#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "config.h"
#include "constants.h"
#include "../io/io.h"
#include "errors.h"
#include "handler.h"

using std::string;

namespace config {

Handler handler;

string get_default_config_file_path() {
    string home_directory = io::get_user_home_directory();
    string config_directory = home_directory + "/.config/auto-addon/";
    return config_directory + constants::DEFAULT_CONFIG_FILENAME;
}

Handler* load_config() {
    return load_config(get_default_config_file_path());
}

Handler* load_config(string config_file_path) {
    if (!io::file_exists(config_file_path)) {
        std::cout << "Creating default config at " + config_file_path << std::endl;
    }

    // Create config directories if missing.
    std::experimental::filesystem::create_directories(config_file_path);

    // Create the file by opening and closing a stream to it.
    FILE *config_file_object = std::fopen(config_file_path.c_str(), "r");
    if (config_file_object == NULL) {
		throw std::runtime_error(errors::FILE_OPEN);
    }
    std::fclose(config_file_object);

    // Load the config since it's now been verified to exist.
    handler.load(config_file_path);
    return &handler;
}

void make_config_if_not_exists(string config_file_path) {
    std::ofstream stream;
    stream.open(config_file_path);
    if (!stream) {
        std::cout << "Error opening " + config_file_path << std::endl;
    }
    stream << constants::DEFAULT_CONFIG_CONTENTS;
    if (!stream) {
        std::cout << "Error writing to " + config_file_path << std::endl;
    }
}

} // namespace config
