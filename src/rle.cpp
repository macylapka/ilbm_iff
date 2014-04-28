#include <rle.h>

bool run_length_encoding::decode(char unsigned *encoded, 
  int unsigned &encoded_length, char unsigned *decoded, 
  int unsigned &decoded_length) {
  
  if(encoded_length == 0)
    return false; 
  
  char unsigned *data;
   
  return true;
}

int run_length_encoding::increase_array_size(
  char unsigned **input_array, int unsigned length, 
  int unsigned size) {
  // zwiekszamy liczbe elementów w tablicy
  int new_array_size = length + size;

  // tworzymy tablice z powiekszona liczba elementow
  char unsigned *tmp = new char unsigned[new_array_size];
 
  // kopiujemy stara tablice do nowej
  for(int unsigned i = 0; i < length; ++i) {
    tmp[i] = (*input_array)[i];
  }
  
  // usuwamy starą tablice
  delete [](*input_array);

  // niech nasza nowa tablica zastapi starą
  (*input_array) = tmp;

  // zwrócmy ilość elementów nowej tablicy
  return new_array_size;
}
