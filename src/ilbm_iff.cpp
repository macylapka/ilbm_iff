#include <ilbm_iff.h>
#include <file_loader.h>
#include <memory.h>
#include <iostream>
#include <glog/logging.h>


ilbm_iff::~ilbm_iff() {

}

ilbm_iff::ilbm_iff() {
 
  path = "";
  
  has_form = false;
  size = 0;
 
  has_bmhd = false;
  memset(&bmhd, 0, sizeof(bmhd));
  
  has_anno = false;
  annotation = "";

  has_dpi = false;
  dpi_w = 0;
  dpi_h = 0;

  has_color_register = false;
  color_size = 0;
  color = nullptr;
  color_4 = nullptr;

  has_grab = false;
  memset(&point_2D, 0, sizeof(point_2D));

  has_dest = false;
  memset(&dest_merge, 0, sizeof(has_dest));

  has_sprt = false;
  precedence = 0;

  has_camg = false;
  camg = 0;

  has_body = false;
  body = nullptr;

  has_c_range = false;
  memset(&c_range, 0, sizeof(c_range));

  has_ccrt = false;
  memset(&cycle_info, 0, sizeof(cycle_info));
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
  if(!(has_bmhd && has_body)) 
    return nullptr;

  int byte_per_plane = bmhd.w * bmhd.h / 8;

  char unsigned bytes[bmhd.n_planes];
  char unsigned pos;
  std::vector<char unsigned> rgb;
  
  LOG(INFO) << "Byte per plane: " << byte_per_plane << std::endl;
  
  for(int i = 0; i < byte_per_plane; ++i) { 
    for(int j = 0; j < bmhd.n_planes; ++j) {
      bytes[j] = body[i + byte_per_plane * j];   
    }

    for(int j = 7; j >= 0; --j) {
      pos  = (((bytes[0] & (1 << j)) >> j) << 0);
      pos |= (((bytes[1] & (1 << j)) >> j) << 1); 
      pos |= (((bytes[2] & (1 << j)) >> j) << 2);
      pos |= (((bytes[3] & (1 << j)) >> j) << 3);
      rgb.push_back(color[pos].red); 
      rgb.push_back(color[pos].green);
      rgb.push_back(color[pos].blue);
      pos = 0;
    }  
  }

  char unsigned *rgb_data = new char unsigned[rgb.size()];
  LOG(INFO) << "std::vector size " << rgb.size() << std::endl; 
  memory::cpy(rgb.data(), (int unsigned)0, rgb_data, 
    (int unsigned)0, (int unsigned)rgb.size());

  return rgb_data;
}

char unsigned *ilbm_iff::convert_to_rgba() {
  if(!(has_bmhd && has_body)) 
    return nullptr;

  int byte_per_plane = bmhd.w * bmhd.h / 8;

  char unsigned bytes[bmhd.n_planes];
  char unsigned pos;
  std::vector<char unsigned> rgba;
  
  LOG(INFO) << "Byte per plane: " << byte_per_plane << std::endl;
  
  for(int i = 0; i < byte_per_plane; ++i) { 
    for(int j = 0; j < bmhd.n_planes; ++j) {
      bytes[j] = body[i + byte_per_plane * j];   
    }

    for(int j = 7; j >= 0; --j) {
      pos = ((((bytes[0] & (1 << j)) >> j))
        | (((bytes[1] & (1 << j)) >> j) << 1) 
        | (((bytes[2] & (1 << j)) >> j) << 2)
        | (((bytes[3] & (1 << j)) >> j) << 3));
      rgba.push_back(color[pos].red);
      rgba.push_back(color[pos].green);
      rgba.push_back(color[pos].blue);
      rgba.push_back(255);
      pos = 0;
    }   
  }

  char unsigned *rgba_data = new char unsigned[rgba.size()];
  LOG(INFO) << "std::vector size " << rgba.size() << std::endl; 
  memory::cpy(rgba.data(), (int unsigned)0, rgba_data, 
    (int unsigned)0, (int unsigned)rgba.size());

  return rgba_data;
}

char unsigned *ilbm_iff::convert_to_bgr() {
  if(!(has_bmhd && has_body)) 
    return nullptr;

  int byte_per_plane = bmhd.w * bmhd.h / 8;

  char unsigned bytes[bmhd.n_planes];
  char unsigned pos;
  std::vector<char unsigned> bgr;
  
  LOG(INFO) << "Byte per plane: " << byte_per_plane << std::endl;
  
  for(int i = 0; i < byte_per_plane; ++i) { 
    for(int j = 0; j < bmhd.n_planes; ++j) {
      bytes[j] = body[i + byte_per_plane * j];   
    }

    for(int j = 7; j >= 0; --j) {
      pos = ((((bytes[0] & (1 << j)) >> j))
        | (((bytes[1] & (1 << j)) >> j) << 1) 
        | (((bytes[2] & (1 << j)) >> j) << 2)
        | (((bytes[3] & (1 << j)) >> j) << 3));
      bgr.push_back(color[pos].blue);
      bgr.push_back(color[pos].green);
      bgr.push_back(color[pos].red);
      pos = 0;
    }   
  }

  char unsigned *bgr_data = new char unsigned[bgr.size()];
  LOG(INFO) << "std::vector size " << bgr.size() << std::endl; 
  memory::cpy(bgr.data(), (int unsigned)0, bgr_data, 
    (int unsigned)0, (int unsigned)bgr.size());

  return bgr_data;
}

char unsigned *ilbm_iff::convert_to_bgra() {
  if(!(has_bmhd && has_body)) 
    return nullptr;

  int byte_per_plane = bmhd.w * bmhd.h / 8;

  char unsigned bytes[bmhd.n_planes];
  char unsigned pos;
  std::vector<char unsigned> bgra;
  
  LOG(INFO) << "Byte per plane: " << byte_per_plane << std::endl;
  
  for(int i = 0; i < byte_per_plane; ++i) { 
    for(int j = 0; j < bmhd.n_planes; ++j) {
      bytes[j] = body[i + byte_per_plane * j];   
    }

    for(int j = 7; j >= 0; --j) {
      pos = ((((bytes[0] & (1 << j)) >> j))
        | (((bytes[1] & (1 << j)) >> j) << 1) 
        | (((bytes[2] & (1 << j)) >> j) << 2)
        | (((bytes[3] & (1 << j)) >> j) << 3));
      bgra.push_back(color[pos].blue);
      bgra.push_back(color[pos].green);
      bgra.push_back(color[pos].red);
      bgra.push_back(0);
      pos = 0;
    }   
  }

  char unsigned *bgra_data = new char unsigned[bgra.size()];
  LOG(INFO) << "std::vector size " << bgra.size() << std::endl; 
  memory::cpy(bgra.data(), (int unsigned)0, bgra_data, 
    (int unsigned)0, (int unsigned)bgra.size());

  return bgra_data;
}   

int unsigned ilbm_iff::get_width() {
  return bmhd.w;
}

int unsigned ilbm_iff::get_height() {
  return bmhd.h;
}

bool ilbm_iff::load() {

  int unsigned length = 0;
  body = file_loader::get_data(path, length);

  LOG(INFO) << "Loaded:     " << length << " bytes" << std::endl;

  parse(body.get(), length);
  char *decoded_body;
  int unsigned decoded_body_size;

  LOG(INFO) << "body before decoding: " << body_size << std::endl;
  body = run_length_encoding::decode(std::move(body), body_size);
  if(body)
    LOG(INFO) << "Body not empty" << std::endl;
  LOG(INFO) << "body after decoding: " << body_size << std::endl;
  
  return true;
}

void ilbm_iff::parse(char unsigned *data, int unsigned len) {
  
  int dword        = 0;
  int step         = 0;
  int unsigned pos = 0;  
  
  while(pos < len) {
    dword = memory::get_dword(data, pos, len);
    switch(dword) {
      case FORM:
        step += parse_form(data, pos, len);
        break;
      case ILBM:
        step += parse_ilbm(data, pos, len);
        break;
      case BMHD:
        step += parse_bmhd(data, pos, len);
        LOG(INFO) << "Width:      " << bmhd.w << std::endl;
        LOG(INFO) << "Height:     " << bmhd.h << std::endl;
        LOG(INFO) << "Planes:     " << (int)bmhd.n_planes << std::endl;
        LOG(INFO) << "Masking:    " << (int)bmhd.masking << std::endl;
        LOG(INFO) << "Compressed: " << (int)bmhd.compression << std::endl;
        LOG(INFO) << "Aspect:     " << (int)bmhd.x_aspect << " x " << (int)bmhd.y_aspect << std::endl;
        LOG(INFO) << "Page W x H: " << bmhd.page_width << " x " << bmhd.page_height << std::endl;  
        break; 
      case ANNO:
        step += parse_anno(data, pos, len);
        LOG(INFO) << "Anno: " << annotation << std::endl;
        break;
      case DPI:
        step += parse_dpi(data, pos, len);
        LOG(INFO) << dpi_w << " x " << dpi_h << "dpi" << std::endl;
        break;
      case CMAP:
        step += parse_cmap(data, pos, len);
        break;
      case GRAB:
        LOG(INFO) << "GRAB" << std::endl;
        break;
      case DEST:
        LOG(INFO) << "DEST" << std::endl;
        break;
      case SPRT:
        LOG(INFO) << "SPRT" << std::endl;
        break;
      case CAMG:
        step = parse_camg(data, pos, len);
        LOG(INFO) << std::hex << "Camg ham: " << (camg & CAMG_HAM) 
          <<  std::dec << std::endl;
        LOG(INFO) << std::hex << "Camg ehb: " << (camg & CAMG_EHB) 
          << std::dec << std::endl;
        break;
      case BODY:
        LOG(INFO) << "BODY" << std::endl;
        step = parse_body(data, pos, len);
        break;
      case CRNG:
        LOG(INFO) << "CRNG" << std::endl;
        break;
      case CCRT:
        LOG(INFO) << "CCRT" << std::endl;
        break;
    }
 
    if(!step) {
      ++pos;
    } else {
      pos += step;
      step = 0;
    }
  }
}

int ilbm_iff::parse_form(char unsigned *data, 
                         int unsigned   pos, 
                         int unsigned   len) {
  has_form   = true;
  int offset = SECTION_LENGTH;
  size       = memory::get_dword(data, pos + offset, len);
  
  return offset + SIZE_LENGTH;
}

int ilbm_iff::parse_ilbm(char unsigned *data, 
                         int unsigned   pos, 
                         int unsigned   len) {
  return SECTION_LENGTH;
}

int ilbm_iff::parse_bmhd(char unsigned *data, 
                         int unsigned   pos, 
                         int unsigned   len) {
  has_bmhd = true;
  
  int offset = SECTION_LENGTH + SIZE_LENGTH;
  
  bmhd.w = memory::get_word(data, pos + offset, len);
  offset += WORD_LENGTH;
  
  bmhd.h = memory::get_word(data, pos + offset, len);
  offset += WORD_LENGTH;
  
  bmhd.x = memory::get_word(data, pos + offset, len);
  offset += WORD_LENGTH;
  
  bmhd.y = memory::get_word(data, pos + offset, len);
  offset += WORD_LENGTH;
  
  bmhd.n_planes = memory::get_byte(data, pos + offset, len);
  offset += BYTE_LENGTH;
  
  bmhd.masking = memory::get_byte(data, pos + offset, len);
  offset += BYTE_LENGTH;
  
  bmhd.compression = memory::get_byte(data, pos + offset, len);
  offset += BYTE_LENGTH;
  
  bmhd.pad1 = memory::get_byte(data, pos + offset, len);
  offset += BYTE_LENGTH;
  
  bmhd.transparent_color = memory::get_word(data, pos + offset, len);
  offset += WORD_LENGTH;
  
  bmhd.x_aspect = memory::get_byte(data, pos + offset, len);
  offset += BYTE_LENGTH;
  
  bmhd.y_aspect = memory::get_byte(data, pos + offset, len);
  offset += BYTE_LENGTH;
  
  bmhd.page_width = memory::get_word(data, pos + offset, len);
  offset += WORD_LENGTH;
  
  bmhd.page_height = memory::get_word(data, pos + offset, len);
  
  return offset + WORD_LENGTH;
}

int ilbm_iff::parse_anno(char unsigned *data, 
                         int unsigned   pos, 
                         int unsigned   len) {
  has_anno   = true;
  int offset = SECTION_LENGTH;
  int length = memory::get_dword(data, pos + offset, len);
  offset += SIZE_LENGTH;
  
  annotation = "";
  for(int i = 0; i < length; ++i)
    annotation += (char)memory::get_byte(data, pos + offset + i, len);

  return length + offset;
}

int ilbm_iff::parse_dpi(char unsigned *data, 
                        int unsigned   pos,
                        int unsigned   len) {
  has_dpi = true;
  int offset = SECTION_LENGTH;
  int length = memory::get_dword(data, pos + offset, len);
  offset += SIZE_LENGTH;
  dpi_w = memory::get_word(data, pos + offset, len);
  offset += WORD_LENGTH;
  dpi_h = memory::get_word(data, pos + offset, len);
  return offset + WORD_LENGTH; 
}

int ilbm_iff::parse_cmap(char unsigned *data, 
                         int unsigned   pos,
                         int unsigned   len) {
  has_color_register = true;  
  int offset = SECTION_LENGTH;
  int length = memory::get_dword(data, pos + offset, len);
  offset += SIZE_LENGTH;
  LOG(INFO) << "RGB in CMAP: " << (length / 3) << std::endl;
  color = std::unique_ptr<color_register_type[]>(new color_register_type[length / 3]);

  color_size = length / 3;
  for(int i = 0; i < color_size; ++i) {
    color[i].red = memory::get_byte(data, pos + offset + i * 3 , len);
    color[i].green = memory::get_byte(data, pos + offset + i * 3 + 1, len);
    color[i].blue = memory::get_byte(data, pos + offset + i * 3 + 2 , len);
    LOG(INFO) << "Red: " << (int)color[i].red 
              << " Green: " << (int)color[i].green 
              << " Blue: " <<(int)color[i].blue << std::endl;
  }

  return length + offset;
}

int ilbm_iff::parse_camg(char unsigned *data, 
                         int unsigned   pos,
                         int unsigned   len) {
  has_camg   = true;
  int offset = SECTION_LENGTH;
  int length = memory::get_dword(data, pos + offset, len);
  offset += SIZE_LENGTH;
  camg = memory::get_dword(data, pos + offset, len);

  return length + offset;
}

int ilbm_iff::parse_body(char unsigned *data, 
                         int unsigned   pos,
                         int unsigned   len) {
  has_body   = true;
  int offset = SECTION_LENGTH;
  int length = memory::get_dword(data, pos + offset, len);
  offset += SIZE_LENGTH;
  body_size = length;
  body = std::unique_ptr<char unsigned[]>(new char unsigned[length]);
  for(int i = 0; i < length; ++i) 
    body[i] = data[pos + offset + i];
  
  return length + offset;
}
