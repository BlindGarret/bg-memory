// Copyright [2020] BlindGarret<lroe2930@gmail.com>
#ifndef BGMEMORY_INCLUDE_BGMEMORY_POINTERS_MUTABLEWEAKPTR_HXX_
#define BGMEMORY_INCLUDE_BGMEMORY_POINTERS_MUTABLEWEAKPTR_HXX_

#include <memory>
#include "bgmemory/DefaultDeleter.hxx"

#ifdef BG_MEMORY_MULTITHREAD

#endif // BG_MEMORY_MULTITHREAD

namespace bg {
    
    /*
        Weak pointer class, to be used as a drop in replacement for the
        standard library weak_ptr. The main difference is the ability
        for the shared_ptr attached to mutate the underlying pointer for 
        all copies, and all weak ptrs.

        The reason for this class is that some of our memory pool structures
        require the ability to swap out a pointer, in order to defragment a
        heap or some such functionality, and have that swap effect not just
        the current reference, but also effect the child references which are
        pointing at the current reference. Swap and reset do not accomplish
        this as they only mutate the current reference.

        This should not be used as a replacement for smart pointers without
        good cause. It will be used throughout this library for consistency,
        but the standard implementation is far better tested, and likely
        simply better. Use this implementation only if you have a good reason.

        When BG_MEMORY_MULTITHREAD is defined the pointer will start using mutex
        locks around assignments and deletion. As with all smart pointers, there is
        no guarantee of thread safety with the stored object in memory. Any
        reference counting and deletion will be thread safe. The object will only be
        deleted once, and references will be atomically decremented and incremented.
        No other guarantees are made.
    */
    template <class T, class DeleterT = DefaultDeleter<T>>
    class MutableSharedPtr
    {

    };
}

#endif // BGMEMORY_INCLUDE_BGMEMORY_POINTERS_MUTABLEWEAKPTR_HXX_
