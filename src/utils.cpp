#include <utils.h>

char memory::get_byte(char unsigned *data, int position, int length) {
  if(position >= length)
    throw "out of range exceptpion";

  return data[position];
}

short memory::get_word(char unsigned *data, int position, int length) {
  if(position >= length)
    throw "out of range exceptpion";
  
  short word = data[position];
  word <<= 8;
  word |= data[position + 1];
  
  return word; 
}

int memory::get_dword(char unsigned *data, int position, int length) {
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

bool memory::dup(char *array, int unsigned position, char byte,
  int unsigned times) {
  
  if(times <= 0)
    return false;

  for (int unsigned i = 0; i < times; ++i)
      array[position + i] = byte;

  return true;
}

bool memory::cpy(unsigned char *src_array, 
  int unsigned src_pos, unsigned char *dest_array,
  int unsigned dest_pos, int unsigned number) {
  
  if(number <= 0)
    return false;

  for(int unsigned i = 0; i < number; ++i)
    dest_array[src_pos + i] = src_array[dest_pos + i];

  return true;
}
