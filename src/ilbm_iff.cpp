#include <ilbm_iff.h>
#include <file_loader.h>
#include <memory.h>
#include <iostream>


ilbm_iff::~ilbm_iff() {

}

ilbm_iff::ilbm_iff() {
 
  path = "";
  
  has_form = false;
  size = 0;
 
  has_bmhd = false;
  bmhd.w = 0;
  bmhd.h = 0; 
  bmhd.x = 0;
  bmhd.y = 0;
  bmhd.n_planes = 0;
  bmhd.masking = 0;
  bmhd.compression = 0;
  bmhd.pad1 = 0;
  bmhd.transparent_color = 0;
  bmhd.x_aspect = 0;
  bmhd.y_aspect = 0;
  bmhd.page_width = 0;
  bmhd.page_height = 0;

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
  if(!(has_bmhd && has_body)) 
    return nullptr;

  int byte_per_plane = bmhd.w * bmhd.h / 8;

  char unsigned bytes[bmhd.n_planes];
  char unsigned pos;
  std::vector<char unsigned> rgb;
  
  std::cout << "Byte per plane: " << byte_per_plane << std::endl;
  
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
  std::cout << "std::vector size " << rgb.size() << std::endl; 
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
  
  std::cout << "Byte per plane: " << byte_per_plane << std::endl;
  
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
  std::cout << "std::vector size " << rgba.size() << std::endl; 
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
  
  std::cout << "Byte per plane: " << byte_per_plane << std::endl;
  
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
  std::cout << "std::vector size " << bgr.size() << std::endl; 
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
  
  std::cout << "Byte per plane: " << byte_per_plane << std::endl;
  
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
  std::cout << "std::vector size " << bgra.size() << std::endl; 
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

#ifdef DEBUG
  std::cout << "loaded: " << length << " bytes" << std::endl;
#endif

  parse(body.get(), length);
  char *decoded_body;
  int unsigned decoded_body_size;

  std::cout << "body before decoding: " << body_size << std::endl;
  body = run_length_encoding::decode(std::move(body), body_size);
  if(body)
    std::cout << "Body not empty" << std::endl;
  std::cout << "body after decoding: " << body_size << std::endl;
  return true;
}

void ilbm_iff::parse(char unsigned *data, int unsigned len) {
  
  int dword = 0;
  int step = 0;
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
        std::cout << "Width: " << bmhd.w << std::endl;
        std::cout << "Height: " << bmhd.h << std::endl; 
        std::cout << "Planes: " << (int)bmhd.n_planes << std::endl;
        std::cout << "Masking: " << (int)bmhd.masking << std::endl;
        std::cout << "Compressed: " << (int)bmhd.compression << std::endl; 
        std::cout << "Aspect: " << (int)bmhd.x_aspect 
          << " x " << (int)bmhd.y_aspect << std::endl;
        std::cout << "Page width x Page height: " 
          << bmhd.page_width << " x " << bmhd.page_height << std::endl;  
        break; 
      case ANNO:
        step += parse_anno(data, pos, len);
        std::cout << "Anno: " << annotation << std::endl;
        break;
      case DPI:
        step += parse_dpi(data, pos, len);
        std::cout << dpi_w << " x " << dpi_h << "dpi" << std::endl;
        break;
      case CMAP:
        step += parse_cmap(data, pos, len);
        break;
      case GRAB:
        std::cout << "GRAB" << std::endl;
        break;
      case DEST:
        std::cout << "DEST" << std::endl;
        break;
      case SPRT:
        std::cout << "SPRT" << std::endl;
        break;
      case CAMG:
        step = parse_camg(data, pos, len);
        std::cout << std::hex << "Camg ham: " << (camg & CAMG_HAM) 
          <<  std::dec << std::endl;
        std::cout << std::hex << "Camg ehb: " << (camg & CAMG_EHB) 
          << std::dec << std::endl;
        break;
      case BODY:
        step = parse_body(data, pos, len);
        break;
      case CRNG:
        std::cout << "CRNG" << std::endl;
        break;
      case CCRT:
        std::cout << "CCRT" << std::endl;
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

int ilbm_iff::parse_form(char unsigned *data, int unsigned pos, 
  int unsigned len) {
  has_form = true;
  int offset = SECTION_LENGTH;
  size = memory::get_dword(data, pos + offset, len);
  return offset + SIZE_LENGTH;
}

int ilbm_iff::parse_ilbm(char unsigned *data, int unsigned pos, 
  int unsigned len) {
  return SECTION_LENGTH;
}

int ilbm_iff::parse_bmhd(char unsigned *data, int unsigned pos, 
  int unsigned len) {
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

int ilbm_iff::parse_anno(char unsigned *data, int unsigned pos, 
  int unsigned len) {
  has_anno = true;
  int offset = SECTION_LENGTH;
  int length = memory::get_dword(data, pos + offset, len);
  offset += SIZE_LENGTH;
  annotation = "";
  for(int i = 0; i < length; ++i) {
    annotation += (char)memory::get_byte(data, pos + offset + i, len);
  }
  return length + offset;
}

int ilbm_iff::parse_dpi(char unsigned *data, int unsigned pos,
  int unsigned len) {
  has_dpi = true;
  int offset = SECTION_LENGTH;
  int length = memory::get_dword(data, pos + offset, len);
  offset += SIZE_LENGTH;
  dpi_w = memory::get_word(data, pos + offset, len);
  offset += WORD_LENGTH;
  dpi_h = memory::get_word(data, pos + offset, len);
  return offset + WORD_LENGTH; 
}

int ilbm_iff::parse_cmap(char unsigned *data, int unsigned pos,
  int unsigned len) {
  has_color_register = true;  
  int offset = SECTION_LENGTH;
  int length = memory::get_dword(data, pos + offset, len);
  offset += SIZE_LENGTH;
  std::cout << "RGB in CMAP: " << (length / 3) << std::endl;
  color = std::unique_ptr<color_register_type[]>(
    new color_register_type[length / 3]);

  color_size = length / 3;
  for(int i = 0; i < color_size; ++i) {
    color[i].red = memory::get_byte(data, pos + offset + i * 3 , len);
    color[i].green = memory::get_byte(data, pos + offset + i * 3 + 1, len);
    color[i].blue = memory::get_byte(data, pos + offset + i * 3 + 2 , len);
    std::cout << "Red: " << (int)color[i].red 
      << " Green: " << (int)color[i].green 
      << " Blue: " <<(int)color[i].blue << std::endl;
  }

  return length + offset;
}

int ilbm_iff::parse_camg(char unsigned *data, int unsigned pos,
  int unsigned len) {
  has_camg = true;
  int offset = SECTION_LENGTH;
  int length = memory::get_dword(data, pos + offset, len);
  offset += SIZE_LENGTH;
  camg = memory::get_dword(data, pos + offset, len);

  return length + offset;
}

int ilbm_iff::parse_body(char unsigned *data, int unsigned pos,
  int unsigned len) {
  has_body = true;
  int offset = SECTION_LENGTH;
  int length = memory::get_dword(data, pos + offset, len);
  offset += SIZE_LENGTH;
  body_size = length;
  body = std::unique_ptr<char unsigned[]>(new char unsigned[length]);
  for(int i = 0; i < length; ++i) 
    body[i] = data[pos + offset + i];
  
  return length + offset;
}
