#include "allocator.h"

ak::LinearAllocator::~LinearAllocator()
{
    delete m_buffer;
}

ak::LinearAllocator::LinearAllocator(std::size_t maxSize)
{
    m_buffer = (char *)(operator new(maxSize));
    m_offset = 0;
    m_size = maxSize;
}

char *ak::LinearAllocator::alloc(size_t req_size)
{
    if ((m_size - m_offset) < req_size)
        return NULL;
    char *ret = m_buffer + m_offset;
    m_offset += req_size;
    return ret;
}

void ak::LinearAllocator::reset()
{
    m_offset = 0;
}
