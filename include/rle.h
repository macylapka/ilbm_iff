#ifndef RLE_H
#define RLE_H

class run_length_encoding {
public:
  static bool decode(char unsigned*, char unsigned*, int unsigned&, int unsigned&);
private:
  run_length_encoding(); 
};

#endif /*RLE_H*/
