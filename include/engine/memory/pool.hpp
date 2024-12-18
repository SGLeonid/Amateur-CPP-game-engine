#ifndef BASE_POOL_H
#define BASE_POOL_H

#include <cstdint>
#include <vector>
#include <list>

#include <assert.hpp>

#define POOL_OBJECTS_PER_FREELIST 256
#define POOL_OBJECT_GRANULARITY 8
#define POOL_MAX_OBJECT_SIZE 256

class PoolAllocator {
public:

	PoolAllocator(const PoolAllocator &_instance) = delete;
	void operator=(const PoolAllocator &_instance) = delete;

	static PoolAllocator &getInstance();

    static int createPool(size_t size, size_t objectsPerBlock = POOL_OBJECTS_PER_FREELIST);
    static void destroyPool(int pool);
    static void *alloc(int pool);
    static void free(void *ptr);

private:
    typedef struct Pool {
        Pool();
        Pool(size_t _objectSize, size_t _objectsPerBlock = POOL_OBJECTS_PER_FREELIST);
        ~Pool();
        struct _Block;
        typedef struct _Block Block;

        typedef struct Node {
            Node        *next;
            Block       *block;
            uint64_t      free;
            uint64_t      reserved;
        } Node;

        struct _Block {
            Pool        *pool;
            Node        *freelist;
            Node        *objects;
            uint64_t      count;
        };

        std::list<Block*> blocks;
        size_t objectSize;
        size_t count;
        size_t objectsPerBlock;

        void *alloc();
        void free(void *ptr);
        Block *expand();
        void clear();
    } Pool;

	PoolAllocator();
    ~PoolAllocator();

    std::vector<Pool*> pools;
};

#endif