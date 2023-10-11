#include <cstring>
#include <cassert>

#include "MemoryPool.h"

namespace Utils {

MemoryPool::MemoryPool()
    : _blocks(0),
      _allocatedBlocks(0),
      _blockCount(-1),
      _ptr(0),
      _end(0)
{ }

MemoryPool::~MemoryPool()
{
    if (_blocks) {
        for (int i = 0; i < _allocatedBlocks; ++i) {
            if (char *b = _blocks[i])
                std::free(b);
        }

        std::free(_blocks);
    }
}

void MemoryPool::reset()
{
    _blockCount = -1;
    _ptr = _end = 0;
}

void *MemoryPool::allocate_helper(size_t size)
{
    assert(size < BLOCK_SIZE);

    if (++_blockCount == _allocatedBlocks) {
        if (! _allocatedBlocks)
            _allocatedBlocks = DEFAULT_BLOCK_COUNT;
        else
            _allocatedBlocks *= 2;

        _blocks = (char **) realloc(_blocks, sizeof(char *) * _allocatedBlocks);

        for (int index = _blockCount; index < _allocatedBlocks; ++index)
            _blocks[index] = 0;
    }

    char *&block = _blocks[_blockCount];

    if (! block)
        block = (char *) std::malloc(BLOCK_SIZE);

    _ptr = block;
    _end = _ptr + BLOCK_SIZE;

    void *addr = _ptr;
    _ptr += size;
    return addr;
}

RecursiveMemoryPool::RecursiveMemoryPool(MemoryPool *pool)
    : _pool(pool),
      _blockCount(pool->_blockCount),
      _ptr(pool->_ptr),
      _end(pool->_end)
{
}

RecursiveMemoryPool::~RecursiveMemoryPool()
{
    _pool->_blockCount = _blockCount;
    _pool->_ptr = _ptr;
    _pool->_end = _end;
}

Managed::Managed()
{ }

Managed::~Managed()
{ }

void *Managed::operator new(size_t size, MemoryPool *pool)
{ return pool->allocate(size); }

void Managed::operator delete(void *)
{ }

void Managed::operator delete(void *, MemoryPool *)
{ }

}   //namespace Utils
