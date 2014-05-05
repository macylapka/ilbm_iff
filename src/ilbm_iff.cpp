#include <ilbm_iff.h>
#include <file_loader.h>
#include <iostream>

ilbm_iff::~ilbm_iff() {

}

ilbm_iff::ilbm_iff() {
 
  path = "";
  ilbm_size = 0;
 
  has_ilbm = false;
  w = h = x = y = 0;
  n_planes = 0;
  masking = 0;
  compression = 0;
  pad1 = 0;
  transparent_color = 0;
  x_aspect = y_aspect = 0;
  page_width = page_height = 0;

  has_color_register = false;
  color = nullptr;
  color_4 = nullptr;

  has_grab = false;
  point_2D.x = 0;
  point_2D.y = 0;

  has_dest = false;
  dest_merge.depth = 0;
  dest_merge.pad1 = 0;
  dest_merge.plane_pick = 0;
  dest_merge.plane_on_off = 0;
  dest_merge.plane_mask = 0;

  has_sprt = false;
  precedence = 0;

  has_camg = false;
  camg = 0;

  has_body = false;
  body = nullptr;

  has_c_range = false;
  c_range.pad1 = 0;
  c_range.rate = 0;
  c_range.flags = 0;
  c_range.low = 0;
  c_range.high = 0;

  has_ccrt = false;
  cycle_info.direction = 0;
  cycle_info.start = 0;
  cycle_info.end = 0;
  cycle_info.seconds = 0;
  cycle_info.microseconds = 0;
  cycle_info.pad = 0; 
}

ilbm_iff::ilbm_iff(std::string &full_path_with_name_and_ext) {
  path = full_path_with_name_and_ext;  
  if(!load())
    throw "Create ilbm image: Faild";
}

bool ilbm_iff::set_path_and_load(std::string &full_path_with_name_and_ext) {
  path = full_path_with_name_and_ext;
  if(!load())
    return false;
  return true;
}

std::string ilbm_iff::get_path() {
  return path; 
}

char unsigned *ilbm_iff::convert_to_rgb() {
  return nullptr;
}

char unsigned *ilbm_iff::convert_to_rgba() {
  return nullptr;
}

int unsigned ilbm_iff::get_width() {
  return w;
}

int unsigned ilbm_iff::get_height() {
  return h;
}

bool ilbm_iff::load() {

  int unsigned length = 0;
  body = file_loader::get_data(path, length);
  std::cout << "loaded: " << length << " bytes" << std::endl;
  parse(body.get(), length);
  return true;
}

void ilbm_iff::parse(char *data, int unsigned length) {
  
  int unsigned position = 0;
  
  //while(position < length) {
      
  //} 

}
