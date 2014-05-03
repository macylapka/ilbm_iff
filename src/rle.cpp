#include <rle.h>

int unsigned run_length_encoding::array_length = increase_size;

bool run_length_encoding::decode(char *encoded, 
  int unsigned &encoded_length, char *decoded, 
  int unsigned &decoded_length) {
  
  if(encoded_length == 0)
    return false; 
  
  int unsigned encoded_position = 0;
  int unsigned decoded_position = 0; 
  char *data = new char[array_length];
  
  while(encoded_position < encoded_length) {
    if(encoded[encoded_position] >= 0) {
      char number = encoded[encoded_position] + 1;
      ++encoded_position;
      
      // sprawdzamy czy się zmiesci jak się nie popiesci
      if(decoded_position + number >= array_length) 
        // jezeli nie da się zwiększyć nawet pieszczac to sorry 
        if(!increase_array_size(&data, array_length, increase_size)) {
          delete []data;
          return false;
        }
      
      // kopiujemy n + 1 bajtów tak jak jest 
      memcpy(encoded, encoded_position, data, decoded_position, number);
      
      decoded_position += number;
      encoded_position += number; 
             
    } else if(encoded[encoded_position] >= -127 &&
              encoded[encoded_position] <= -1) { 
      char unsigned number = -encoded[encoded_position] + 1;
      ++encoded_position;
      
      // sprawdzamy czy się zmiesci jak się nie popiesci
      if(decoded_position + number >= array_length) 
        // jezeli nie da się zwiększyć nawet pieszczac to sorry 
        if(!increase_array_size(&data, array_length, increase_size)) {
          delete []data;
          return false;
        }

      // powielamy bajt -n+1 razy
      char byte = encoded[decoded_position];  
      memdup(data, decoded_position, byte, number);

      decoded_position += number; 
      ++encoded_position;
 
    } else if(encoded[encoded_position] == -128) {
      // nie robimy nic

    } else {
      // tu nie powinnismy wejsć
    
    }
  }
 
  return true;
}

int run_length_encoding::increase_array_size(
  char **input_array, int unsigned length, 
  int unsigned size) {
  // zwiekszamy liczbe elementów w tablicy
  int new_array_size = length + size;

  // tworzymy tablice z powiekszona liczba elementow
  char *tmp = new char[new_array_size];
 
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

bool run_length_encoding::memdup(
  char *array, 
  int unsigned position,
  char byte,
  int unsigned times) {
  
  if(times <= 0)
    return false;

  for (int unsigned i = 0; i < times; ++i)
      array[position + i] = byte;

  return true;
}

bool run_length_encoding::memcpy(
  char *src_array,
  int unsigned src_pos,
  char *dest_array,
  int unsigned dest_pos,
  int unsigned number) {
  
  if(number <= 0)
    return false;

  for(int unsigned i = 0; i < number; ++i)
    dest_array[src_pos + i] = src_array[dest_pos + i];

  return true;
}
