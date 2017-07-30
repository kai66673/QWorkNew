#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <new>

#include "utils_global.h"

namespace Utils {

class MemoryPool;
class RecursiveMemoryPool;

class QTCREATOR_UTILS_EXPORT MemoryPool
{
    MemoryPool(const MemoryPool &other);
    void operator =(const MemoryPool &other);

public:
    MemoryPool();
    ~MemoryPool();

    void reset();

    inline void *allocate(size_t size)
    {
        size = (size + 7) & ~7;
        if (_ptr && (_ptr + size < _end)) {
            void *addr = _ptr;
            _ptr += size;
            return addr;
        }
        return allocate_helper(size);
    }

private:
    void *allocate_helper(size_t size);

private:
    char **_blocks;
    int _allocatedBlocks;
    int _blockCount;
    char *_ptr;
    char *_end;

    enum
    {
        BLOCK_SIZE = 2 * 1024,
        DEFAULT_BLOCK_COUNT = 8 * 2048
    };

    friend class RecursiveMemoryPool;
};

class QTCREATOR_UTILS_EXPORT RecursiveMemoryPool
{
    MemoryPool *_pool;
    int _blockCount;
    char *_ptr;
    char *_end;

public:
    RecursiveMemoryPool(MemoryPool *pool);
    ~RecursiveMemoryPool();
};

class QTCREATOR_UTILS_EXPORT Managed
{
    Managed(const Managed &other);
    void operator = (const Managed &other);

public:
    Managed();
    virtual ~Managed();

    void *operator new(size_t size, MemoryPool *pool);
    void operator delete(void *);
    void operator delete(void *, MemoryPool *);
};

}   // namespace Utils

#endif // MEMORYPOOL_H
