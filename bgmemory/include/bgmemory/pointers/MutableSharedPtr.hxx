// Copyright [2020] BlindGarret<lroe2930@gmail.com>
#ifndef BGMEMORY_INCLUDE_BGMEMORY_POINTERS_MUTABLESHAREDPTR_HXX_
#define BGMEMORY_INCLUDE_BGMEMORY_POINTERS_MUTABLESHAREDPTR_HXX_

#include <memory>
#include "bgmemory/pointers/Deleter.hxx"
#include "bgmemory/DefaultDeleter.hxx"
#include "bgmemory/pointers/inner/SharedPointerPayload.hxx"

#ifdef BG_MEMORY_MULTITHREAD

#endif // BG_MEMORY_MULTITHREAD

// TODO: Allow for specialized allocators so metadata isn't allocated on the stack

namespace bg
{
    // Forward Declarations
    template <class T>
    class SharedPtrMutator;

    template <class T>
    class MutableWeakPtr;

    /*
        Shared pointer class, to be used as a drop in replacement for the
        standard library shared_ptr. The main difference is the ability
        to mutate the underlying pointer for all copies, and all weak ptrs.

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

        Deleters may be provided via the deleter interface. The current implementation
        uses a pointer, and will clean up the pointer for you.

        When BG_MEMORY_MULTITHREAD is defined the pointer will start using mutex
        locks around assignments/deletions/reads. As with all smart pointers, there is
        no guarantee of thread safety with the stored object in memory. Any
        reference counting and deletion will be thread safe. The object will only be
        deleted once, and references will be atomically decremented and incremented.
        No other guarantees are made.
    */
    template <class T>
    class MutableSharedPtr
    {
        inner::SharedPointerPayload<T> *payload = nullptr;
        friend class SharedPtrMutator<T>;
        friend class MutableWeakPtr<T>;

    public:
        /*
            Constructs a shared pointer with no owned object.
        */
        MutableSharedPtr()
        {
            payload = new inner::SharedPointerPayload<T>();
            payload->count++;
        }

        /*
            Constructs a shared pointer with no owned object.
        */
        constexpr MutableSharedPtr(std::nullptr_t) noexcept
        { // NOLINT
            payload = new inner::SharedPointerPayload<T>();
            payload->count++;
        }

        /*
            Constructs a shared pointer which takes ownership of the pointer
            passed to it.

            @param pointer the pointer to take ownership of.
        */
        explicit MutableSharedPtr(T *pointer) noexcept
        {
            payload = new inner::SharedPointerPayload<T>();
            payload->managedObject = pointer;
            payload->count++;
        }

        /*
            Constructs a shared pointer which takes ownership of the pointer
            passed to it.

            This constructor will take ownership of the DeleterT Pointer passed to it.
            DeleterT must be of type bg::Deleter
            Cleanup of deleter will be handled by the pointer

            @param pointer the pointer to take ownership of.
            @param d a reference to an instance of the deleter.
        */
        template <class DeleterT>
        MutableSharedPtr(T *pointer, DeleterT *d) noexcept
        {
            payload = new inner::SharedPointerPayload<T>(d);
            payload->managedObject = pointer;
            payload->count++;
        }

        /*
            Copy constructor. Constructs a pointer shared with another pointer,
            using the internals of the original pointer and incrementing the pointer
            count.
            
            @param original the pointer to copy.
        */
        MutableSharedPtr(const MutableSharedPtr<T> &original)
        {
            payload = original.payload;
            payload->count++;
        };

        /*
            Destructor
        */
        ~MutableSharedPtr()
        {
            payload->count--;
            if (payload->count < 1)
            {
                (*payload->deleter)(payload->managedObject);
                if (payload->weakCount < 1)
                {
                    delete payload;
                }
            }
        }

        /*
            Replaces the managed object, cleaning up the previous pointer.

            @param ptr pointer to the object to take ownership of.
        */
        void reset(T *ptr) noexcept
        {
            (*payload->deleter)(payload->managedObject);
            payload->managedObject = ptr;
        }

        /*
            Swaps ownership of objects between this instance and the
            given one.

            @param other the instace of the shared pointer to swap with.
        */
        void swap(MutableSharedPtr<T> // NOLINT
                      &other) noexcept
        {
            auto holdPayload = other.payload;
            other.payload = payload;
            payload = holdPayload;
        }

        /*
            Gets the current count of different MutableSharedPtr instances including the current one.
            If no object is being managed, returns 0.

            @return count of shared pointers refering to this object.
        */
        long useCount() const noexcept
        {
            return payload->managedObject != nullptr ? payload->count : 0;
        }

        /*
            Gets a pointer to the managed object.

            @return raw pointer to the managed object.
        */
        T *get() const noexcept
        {
            return payload->managedObject;
        }

        /*
            Gets a reference to the deleter defined for this instance.

            @return reference to the defined deleter.
        */
        Deleter<T> &getDeleter() noexcept
        {
            return *payload->deleter;
        }

        /*
            Gets a constant reference to the deleter defined for this
            instance.

            @return constant reference to the defined deleter.
        */
        const Deleter<T> &getDeleter() const noexcept
        {
            return *payload->deleter;
        }

        /*
            Checks whether there is an object being managed, or whether
            the object has been released or never set in the first place.

            @return whether there is an object being managed.
        */
        explicit operator bool() const noexcept
        {
            return payload->managedObject != nullptr;
        }

        /*
            Provides access to undlying reference controlled by smart
            pointer.

            If underlying memory is released or uninitialized the results
            of this function are undefined, like accessing uninitialized memory.

            @return l-value reference to the underlying memory.
        */
        typename std::add_lvalue_reference<T>::type operator*() const
        {
            return *payload->managedObject;
        }

        /*
            Provides access to undlying reference controlled by smart
            pointer.

            If underlying memory is released or uninitialized the results
            of this function are undefined, like accessing uninitialized memory.

            @return reference to the underlying memory.
        */
        T *operator->() const noexcept
        {
            return payload->managedObject;
        }
    };

} // namespace bg

#endif // BGMEMORY_INCLUDE_BGMEMORY_POINTERS_MUTABLESHAREDPTR_HXX_