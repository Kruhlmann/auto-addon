#include <cstdlib>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>

#include "io.h"

using std::string;

namespace io {

string get_user_home_directory() {
    if (getenv("HOME")) {
        return getenv("HOME");
    }
    return getpwuid(getuid())->pw_dir;
}

bool file_exists(string& file_path) {
    struct stat buffer;
    return (stat (file_path.c_str(), &buffer) == 0);
}

} // namespace io
