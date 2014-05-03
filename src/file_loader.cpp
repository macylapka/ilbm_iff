#include <file_loader.h>

unsigned int file_loader::get_data(std::string &full_file_path,
    char *buffer) {
  std::ifstream input_file_stream(full_file_path.c_str(), 
    std::ifstream::binary);
  if(input_file_stream.is_open()) {
    input_file_stream.seekg(0, input_file_stream.end);
    int length = input_file_stream.tellg();
    input_file_stream.seekg(0, input_file_stream.beg);
    
    buffer = new char[length];
    input_file_stream.read(buffer, length);
    input_file_stream.close();
    return length; 
  } 
  throw "Unable load file";
}
