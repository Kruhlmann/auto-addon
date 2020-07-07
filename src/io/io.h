#ifndef IO_IO_H_
#define IO_IO_H_

#include <string>

using std::string;

namespace io {

    string get_user_home_directory();
    bool file_exists(string& file_path);

} // namespace io

#endif // IO_IO_H_
