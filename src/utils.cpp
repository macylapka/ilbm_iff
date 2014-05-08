#include <utils.h>

char memory::get_byte(char *data, int position, int length) {
  if(position >= length)
    throw "out of range exceptpion";

  return data[position];
}

short memory::get_word(char *data, int position, int length) {
  if(position >= length)
    throw "out of range exceptpion";
  
  short word = data[position];
  word <<= 8;
  word |= data[position + 1];
  
  return word; 
}

int memory::get_dword(char *data, int position, int length) {
  if(position >= length)
    throw "out of range exceptpion";
  
  int dword = data[position];
  dword <<= 8;
  dword |= data[position + 1];
  dword <<= 8;
  dword |= data[position + 2];
  dword <<= 8;
  dword |= data[position + 3];

  return dword;
}
