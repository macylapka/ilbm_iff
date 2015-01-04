#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <string>
#include <fstream>
#include <memory>

class file_loader {

public:
  std::unique_ptr<char unsigned[]> static get_data(
    std::string&, int unsigned&);
  file_loader() = delete;
  file_loader(file_loader const&) = delete;
  file_loader &operator=(file_loader const&) = delete;
};

#endif
