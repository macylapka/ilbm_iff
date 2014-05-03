#ifndef RLE_H
#define RLE_H

class run_length_encoding {
public:
  bool static decode(char*, int unsigned&, 
    char*, int unsigned&);

private:
  int unsigned static array_length;
  int const static increase_size = 1000;
 
  // pobieramy adres wskaźnika na tablice 
  // i powiekszamy ją o podany rozmiar
  // zwracamy rozmiar powiększonej tablicy
  int static increase_array_size(
    char**, 
    int unsigned, 
    int unsigned);
  
  bool static memdup(
    char*,
    int  unsigned, 
    char,
    int unsigned);
    
  bool static memcpy(
    char*,
    int unsigned,
    char*,
    int unsigned,
    int unsigned);

  run_length_encoding();
};

#endif /*RLE_H*/
