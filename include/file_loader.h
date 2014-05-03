#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <string>
#include <fstream>

class file_loader {

public:
  int unsigned static get_data(std::string&, char*);
  file_loader() = delete;
  file_loader &operator=(file_loader const&) = delete;
};

#endif
