#ifndef UTILS_H
#define UTILS_H

class memory {

public:
  
  char static get_byte(char unsigned*, int, int);
  short static get_word(char unsigned*, int, int);
  int static get_dword(char unsigned*, int, int);

  bool static dup(char*, int  unsigned,  char,
    int unsigned);
  bool static cpy(char unsigned*, int unsigned, 
    char unsigned*, int unsigned, int unsigned);

  memory() = delete;
  memory(memory const&) = delete;
  memory &operator=(memory const&) = delete;
};

#endif /*UTLIS_H*/ 
