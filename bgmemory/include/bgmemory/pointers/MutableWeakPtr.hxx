// Copyright [2020] BlindGarret<lroe2930@gmail.com>
#ifndef BGMEMORY_INCLUDE_BGMEMORY_POINTERS_MUTABLEWEAKPTR_HXX_
#define BGMEMORY_INCLUDE_BGMEMORY_POINTERS_MUTABLEWEAKPTR_HXX_

#include <memory>
#include "bgmemory/pointers/SharedPointerPayload.hxx"
#include "bgmemory/pointers/MutableSharedPtr.hxx"
#include "bgmemory/DefaultDeleter.hxx"

#ifdef BG_MEMORY_MULTITHREAD

#endif // BG_MEMORY_MULTITHREAD

//todo: Cleanup Comments invalidated by refactor

namespace bg
{
    // Forward Declarations
    template <class T>
    class SharedPtrMutator;

    /*
        Weak pointer class, to be used as a drop in replacement for the
        standard library weak_ptr. The main difference is the ability
        for to mutate the underlying pointer for all shared and weak ptrs.

        The reason for this class is that some of our memory pool structures
        require the ability to swap out a pointer, in order to defragment a
        heap or some such functionality, and have that swap effect all pointers
        referring to the current object. Swap and reset do not accomplish
        this as they only mutate the current pointer.

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
    template <class T>
    class MutableWeakPtr
    {
        SharedPointerPayload<T> *payload = nullptr;
        friend class SharedPtrMutator<T>;

    public:
        /*
            Constructs a weak pointer with no object.
        */
        constexpr MutableWeakPtr() noexcept
        { // NOLINT
            payload = new SharedPointerPayload<T>();
        }

        /*
            Constructs a weak pointer copying from another weak pointer.

            @param r weak pointer to copy from
        */
        MutableWeakPtr(const MutableWeakPtr<T> &r) noexcept
        {
            payload = r.payload;
            payload->weakCount++;
        }

        /*
            Constructs a weak pointer copying from a shared pointer.

            @param r weak pointer to copy from
        */
        MutableWeakPtr(const MutableSharedPtr<T> &r) noexcept
        {
            payload = r.payload;
            payload->weakCount++;
        }

        /*
            Constructs a weak pointer copying from another weak pointer.

            @param r weak pointer to copy from
        */
        MutableWeakPtr(MutableWeakPtr<T> &&r) noexcept
        {
            payload = r.payload;
            payload->weakCount++;
        }

        /*
            Destructor
        */
        ~MutableWeakPtr()
        {
            payload->weakCount--;
            if (payload->count < 1 && payload->weakCount < 1)
            {
                delete payload;
            }
        }

        /*
            Assignment Operator from a shared pointer

            @param p a shared pointer you would like to copy into the weak pointer
        */
        void operator=(const MutableSharedPtr<T> &p)
        {
            reset();
            payload = p.payload;
            payload->weakCount++;
        }

        /*
            Assignment Operator from a weak pointer

            @param p a weak pointer you would like to copy
        */
        void operator=(const MutableWeakPtr<T> &p)
        {
            reset();
            payload = p.payload;
            payload->weakCount++;
        }

        /*
            Replaces the managed object.

            @param ptr pointer to the object to take ownership of.
        */
        void reset() noexcept
        {
            payload->weakCount--;
            if (payload->count < 1 && payload->weakCount < 1)
            {
                delete payload;
            }
            payload->managedObject = nullptr;
        }

        /*
            Swaps ownership of objects between this instance and the
            given one.

            @param other the instace of the shared pointer to swap with.
        */
        void swap(MutableWeakPtr<T> // NOLINT
                      &other) noexcept
        {
            auto holdPayload = other.payload;
            other.payload = payload;
            payload = holdPayload;
        }

        /*
            Gets the current count of different MutableSharedPtr instances not including this weak ptr or any others.
            If no object is being managed, returns 0.

            @return the count of shared pointers referring to the object.
        */
        long useCount() const noexcept
        {
            return payload->managedObject != nullptr ? payload->count : 0;
        }

        /*
            Gets whether there is an actively mantained object reference available

            @return whether the object has expired or not.
        */
        bool expired() const noexcept
        {
            return payload->count == 0;
        }

        /*
            Attempts to lock the pointer checking whether it is expired and either
            returning a new shared pointer with a reference to the object or a blank
            shared pointer accordingly.

            @return a shared pointer to the object if it isn't expired, else a blank shared pointer.
        */
        MutableSharedPtr<T> lock() const noexcept
        {
            auto ptr = MutableSharedPtr<T>();
            if (expired())
            {
                return ptr;
            }

            ptr.payload = payload;
            payload->count++;
            return ptr;
        }
    };
} // namespace bg

#endif // BGMEMORY_INCLUDE_BGMEMORY_POINTERS_MUTABLEWEAKPTR_HXX_
