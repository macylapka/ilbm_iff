#include <ilbm_iff.h>

ilbm_iff::~ilbm_iff() {

}

ilbm_iff::ilbm_iff() {
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
}

ilbm_iff::ilbm_iff(std::string &full_path_with_name_and_ext) {

}
