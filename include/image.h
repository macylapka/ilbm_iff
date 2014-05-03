#ifndef IMAGE_H
#define IMAGE_H

class image {
public: 
  virtual ~image() = 0;
  virtual char unsigned *convert_to_rgb() = 0;
  virtual char unsigned *convert_to_rgba() = 0;
  virtual int unsigned get_width() = 0;
  virtual int unsigned get_height() = 0;  
};

#endif /*IMAGE_H*/
