// Copyright [2020] BlindGarret<lroe2930@gmail.com>
#ifndef BGMEMORY_INCLUDE_BGMEMORY_POINTERS_SHAREDPTRMUTATOR_HXX_
#define BGMEMORY_INCLUDE_BGMEMORY_POINTERS_SHAREDPTRMUTATOR_HXX_

#include <functional>
#include "bgmemory/pointers/inner/SharedPointerPayload.hxx"
#include "bgmemory/pointers/MutableSharedPtr.hxx"
#include "bgmemory/pointers/MutableWeakPtr.hxx"
#include "bgmemory/DefaultDeleter.hxx"

#ifdef BG_MEMORY_MULTITHREAD

#endif // BG_MEMORY_MULTITHREAD

namespace bg
{
    /*
        Limited Weak Pointer class used as a hook for mutating MutableSharedPtr
        and MutableWeakPtr pointers. Acts as a hook to mutate the underlying 
        memory.

        This acts as a weak pointer, and must be disposed of like a weak pointer
        before the metadata is freed from memory. However, since it's a weak pointer
        it will not keep references to the managed object alive.
    */
    template <class T>
    class SharedPtrMutator
    {
        inner::SharedPointerPayload<T> *payload = nullptr;

    public:
        /*
            Constructs a weak pointer copying from another weak pointer.

            @param r weak pointer to copy from
        */
        SharedPtrMutator(const MutableWeakPtr<T> &r) noexcept
        {
            payload = r.payload;
            payload->weakCount++;
        }

        /*
            Constructs a weak pointer copying from another weak pointer.

            @param r weak pointer to copy from
        */
        SharedPtrMutator(MutableWeakPtr<T> &&r) noexcept
        {
            payload = r.payload;
            payload->weakCount++;
        }

        /*
            Constructs a weak pointer copying from another weak pointer.

            @param r weak pointer to copy from
        */
        SharedPtrMutator(const MutableSharedPtr<T> &r) noexcept
        {
            payload = r.payload;
            payload->weakCount++;
        }

        /*
            Constructs a weak pointer copying from another weak pointer.

            @param r weak pointer to copy from
        */
        SharedPtrMutator(MutableSharedPtr<T> &&r) noexcept
        {
            payload = r.payload;
            payload->weakCount++;
        }

        /*
            Destructor
        */
        ~SharedPtrMutator()
        {
            payload->weakCount--;
            if (payload->count < 1 && payload->weakCount < 1)
            {
                delete payload;
            }
        }

        void mutate(T *ptr)
        {
            if (payload->managedObject != nullptr)
            {
                (*payload->deleter)(payload->managedObject);
            }
            if (payload->count > 0)
            {
                payload->managedObject = ptr;
            }
        }
    };
} // namespace bg

#endif // BGMEMORY_INCLUDE_BGMEMORY_POINTERS_SHAREDPTRMUTATOR_HXX_