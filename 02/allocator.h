#pragma once

#include <cstddef>

namespace ak {

class LinearAllocator
{
  private:
    char *m_buffer;
    size_t m_offset;
    size_t m_size;

  public:
    LinearAllocator(size_t maxSize);
    char *alloc(size_t req_size);
    void reset();
    ~LinearAllocator();
};

} // namespace ak
