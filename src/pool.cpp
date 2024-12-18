#include <engine/memory/pool.hpp>
#include <assert.hpp>

#include <cstdint>

PoolAllocator::Pool::Pool() : objectSize(0), count(0) {}

PoolAllocator::Pool::Pool(size_t _objectSize, size_t _objectsPerBlock) : objectSize(_objectSize), objectsPerBlock(_objectsPerBlock), count(0) {
    expand();
}

PoolAllocator::Pool::~Pool() {
    clear();
}

void PoolAllocator::Pool::clear() {
    Debug::log(DEBUG_TRACE, "Cleaning up Pool for size: " + std::to_string(objectSize));

    while(!blocks.empty()) {
        Pool::Block *block = blocks.front();
        blocks.pop_front();

        assert(block -> count == POOL_OBJECTS_PER_FREELIST, "Pool has allocated objects: " + std::to_string(POOL_OBJECTS_PER_FREELIST - block -> count));

        char *blockPtr = (char*)block;
        delete [] block;

        count--;
    }

    assert(count == 0 && blocks.empty(), "Pool has allocated blocks: " + std::to_string(count));
}

void *PoolAllocator::Pool::alloc() {
    Pool::Block *block; 

    if (!(blocks.empty())) {
        block = blocks.front();
    } else {
        block = expand();
    }

    Pool::Node *node = block -> freelist;
    block -> freelist = node -> next;

    node -> free = false;
    void *data = (void*)(node + 1);

    block -> count--;
    if (block -> count == 0) {
        blocks.pop_front();
    }

    return data;
}

void PoolAllocator::Pool::free(void *ptr) {
    PoolAllocator::free(ptr);
}

PoolAllocator::Pool::Block *PoolAllocator::Pool::expand() {
    size_t blockSize = sizeof(Pool::Block) + (sizeof(Pool::Node) + objectSize) * objectsPerBlock;
    size_t chunkSize = sizeof(Pool::Node) + objectSize;
    char *blockPtr = new char[blockSize];

    assert(blockPtr, "Pool block allocation failed!");

    Pool::Block *block = (Pool::Block*)blockPtr;
    Pool::Node *freelist = (Pool::Node*)(block + 1);

    block -> pool = this;
    block -> freelist = freelist;
    block -> objects = freelist;
    block -> count = objectsPerBlock;

    Pool::Node *node;
    for (
        node = block -> freelist;
        node < (Pool::Node*)((char*)(block -> freelist) + (chunkSize * objectsPerBlock));
        node = (Pool::Node*)((char*)node + chunkSize)
    ) {
        node -> next = (Pool::Node*)((char*)node + chunkSize);
        node -> block = block;
        node -> free = true;
    }

    node -> next = nullptr;

    blocks.push_front(block);
    count++;

    Debug::log(DEBUG_TRACE, "Pool block has been allocated for size: " + std::to_string(objectSize));

    return block;
}

PoolAllocator::PoolAllocator() = default;

PoolAllocator::~PoolAllocator() {
    Debug::log(DEBUG_TRACE, "Cleaning up PoolAllocator");

    for (auto &pool : pools) {
        if (pool) {
            delete pool;
        }
    }
}

PoolAllocator &PoolAllocator::getInstance() {
	static PoolAllocator instance;
	return instance;
}

int PoolAllocator::createPool(size_t size, size_t objectsPerBlock) {
    PoolAllocator::Pool *pool = new Pool(size, objectsPerBlock);
    getInstance().pools.push_back(pool);
    return getInstance().pools.size() - 1;
}

void PoolAllocator::destroyPool(int pool) {
    delete getInstance().pools[pool];
    getInstance().pools[pool] = nullptr;
}

void *PoolAllocator::alloc(int pool) {
    return getInstance().pools[pool] -> alloc();
}

void PoolAllocator::free(void *ptr) {
    Pool::Node *node = (Pool::Node*)ptr - 1;
    Pool::Block *block = node -> block;
    Pool *pool = block -> pool;

    assert(!(node -> free), "Pool double free error!");
    
    node -> free = true;
    node -> next = block -> freelist;
    block -> freelist = node;

    block -> count++;
    if (block -> count == 1) {
        pool -> blocks.push_front(block);
    }
}