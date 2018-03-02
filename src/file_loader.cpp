#include <file_loader.h>
#include <stdexcept>

std::unique_ptr<char unsigned[]> file_loader::get_data(std::string &full_file_path, 
                                                       int unsigned &length) {

  std::ifstream input_file_stream(full_file_path.c_str(), std::ifstream::binary);

  if(input_file_stream.is_open()) {
    input_file_stream.seekg(0, input_file_stream.end);
    length = input_file_stream.tellg();
    input_file_stream.seekg(0, input_file_stream.beg);
    
    std::unique_ptr<char unsigned[]> data(new char unsigned[length]);
    input_file_stream.read((char*)data.get(), length);
    input_file_stream.close();
    return data; 
  }

  throw std::runtime_error("Unable load file" + full_file_path);
}
