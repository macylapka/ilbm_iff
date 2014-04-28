#ifndef RLE_H
#define RLE_H

class run_length_encoding {
public:
  bool static decode(char unsigned*, int unsigned&, 
    char unsigned*, int unsigned&);
private:
  int static array_size;
  int static position;
  int const static increase_size = 1000;
 
  // pobieramy adres wskaźnika na tablice 
  // i powiekszamy ją o podany rozmiar
  // zwracamy rozmiar powiększonej tablicy
  int static increase_array_size(
    char unsigned **input_array, 
    int unsigned length, 
    int unsigned size);
  run_length_encoding();
};

#endif /*RLE_H*/
