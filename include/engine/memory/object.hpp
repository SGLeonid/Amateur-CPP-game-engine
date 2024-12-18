#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include <engine/memory/pool.hpp>

template <typename T>
class PooledObject : public T {
    public:
    template <typename ...Args>
    PooledObject(Args &&... args) : T(std::forward<Args>(args)...) {}
    ~PooledObject() {}

    void *operator new(size_t size) {
        (void)size;
        return PoolAllocator::alloc(pool);
    }

    void operator delete(void *ptr) {
        PoolAllocator::free(ptr);
    }

    private:
    static int pool;
};

template<typename T>
int PooledObject<T>::pool = PoolAllocator::createPool(sizeof(T));

#endif