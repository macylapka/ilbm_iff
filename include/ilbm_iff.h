#ifndef ILBM_IFF_H
#define ILBM_IFF_H 

#include <image.h>
#include <rle.h>
#include <string>
#include <memory>

#define MASK_NONE                   0
#define MASK_HAS_MASK               1
#define MASK_HAS_TRANSPARENT_COLOR  2
#define MASK_LASSO                  3

#define CMP_NONE      0
#define CMP_BYTE_RUN  1

#define CAMG_HAM 0x800
#define CAMG_EHB 0x80

#define RNG_ACTIVE  1
#define RNG_REVERSE 2

typedef struct {
  char unsigned red;
  char unsigned green;
  char unsigned blue;
} color_register_type;

typedef struct {
  char unsigned pad1:4;
  char unsigned red:4;
  char unsigned green:4;
  char unsigned blue:4;
} color_4_type;

typedef struct {
  short x;
  short y;
} point_2D_type;

typedef struct {
  char unsigned depth;
  char unsigned pad1;
  short unsigned plane_pick;
  short unsigned plane_on_off;
  short unsigned plane_mask;
} dest_merge_type;

typedef struct {
  short pad1;
  short rate;
  short flags;
  char unsigned low;
  char unsigned high;
} c_range_type;

typedef struct {
  short direction;
  char unsigned start;
  char unsigned end;
  int seconds;
  int microseconds;
  short pad;
} cycle_info_type;

class ilbm_iff : public image {
public:
  ~ilbm_iff();
  ilbm_iff();
  ilbm_iff(std::string&);
  
  bool set_path_and_load(std::string&);
  std::string get_path(); 

  virtual char unsigned *convert_to_rgb();
  virtual char unsigned *convert_to_rgba();
  virtual int unsigned get_width();
  virtual int unsigned get_height();
  
protected:
  bool load();
  void parse(char*, int unsigned);
  
private:
  
  std::string path;
  int unsigned ilbm_size;

  /*
   * BMHD
   */
  bool has_ilbm;
  short unsigned w, h;
  short x, y;
  char unsigned n_planes;
  char unsigned masking;
  char unsigned compression;
  char unsigned pad1;
  short transparent_color;
  char unsigned x_aspect, y_aspect;
  short page_width, page_height;
  
  /*
   * CMAP
   */
  bool has_color_register;
  std::unique_ptr<color_register_type> color;
  std::unique_ptr<color_4_type>        color_4;  

  /*
   * GRAB
   */
  bool has_grab; 
  point_2D_type point_2D;
  
  /*
   * DEST
   */
  bool has_dest; 
  dest_merge_type dest_merge;

  /*
   * SPRT
   */ 
  bool has_sprt;
  unsigned short precedence;

  /*
   * CAMG
   */
  bool has_camg;
  int camg;

  /*
   * BODY
   */ 
  bool has_body;
  std::unique_ptr<char[]> body;
  
  /*
   * CRNG
   */
  bool has_c_range; 
  c_range_type c_range;
 
  /*
   * CCRT
   */
  bool has_ccrt;
  cycle_info_type cycle_info; 

};

#endif /*ILBM_IFF_H */
