#include <cstdio>
#include <iostream>
#include <iterator>
#include <string>

#include "config/config.h"

using std::string;

int main() {
    string config_path = config::get_default_config_file_path();
    std::cout << "Reading configuration at " + config_path << std::endl;
    config::load_config();
    return 0;
}
