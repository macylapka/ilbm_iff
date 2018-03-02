#include <rle.h>

std::unique_ptr<char unsigned[]> run_length_encoding::decode(std::unique_ptr<char unsigned[]> enc, 
                                                             int unsigned &len) {
  
  std::vector<char unsigned> dec_data;
 
  int unsigned pos = 0; 
  char *p_data = (char*)enc.get();
  
  while(pos < len) {
    if(p_data[pos] >= 0) {
      for(int j = 0; j < p_data[pos] + 1; ++j)
        dec_data.push_back(p_data[pos + j + 1]); 
      
      pos += p_data[pos] + 2;
    } else if(p_data[pos] >= -127 && p_data[pos] <= -1) {
      for(int j = 0; j < (-p_data[pos] + 1); ++j) 
        dec_data.push_back(-p_data[pos + 1]);
      
      pos += 2; 
    } else if(p_data[pos] == -128) {
      ++pos; 
    } 
  }
  
  //data.reserve(len);
  //std::copy(enc.get(), enc.get() + len, data.begin()); 
  len = dec_data.size();
  std::unique_ptr<char unsigned[]> result(new char unsigned[len]);
  std::copy(dec_data.begin(), dec_data.end(), result.get());

  return std::move(result);
}


