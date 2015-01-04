#ifndef RLE_H
#define RLE_H

#include <vector>
#include <memory>

class run_length_encoding {
public:
  std::unique_ptr<char unsigned[]> static decode(
    std::unique_ptr<char unsigned[]>, int unsigned&);

  run_length_encoding() = delete;
  run_length_encoding &operator=(run_length_encoding const&) = delete;

};

#endif /*RLE_H*/
