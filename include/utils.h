#ifndef UTILS_H
#define UTILS_H

class memory {

public:
  
  char static get_byte(char*, int, int);
  short static get_word(char*, int, int);
  int static get_dword(char*, int, int);

  memory() = delete;
  memory(memory const&) = delete;
};

#endif /*UTLIS_H*/ 
