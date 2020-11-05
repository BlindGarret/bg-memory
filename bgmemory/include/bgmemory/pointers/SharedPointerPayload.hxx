// Copyright [2020] BlindGarret<lroe2930@gmail.com>
#ifndef BGMEMORY_INCLUDE_BGMEMORY_POINTERS_SHAREDPOINTERPAYLOAD_HXX_
#define BGMEMORY_INCLUDE_BGMEMORY_POINTERS_SHAREDPOINTERPAYLOAD_HXX_

#ifdef BG_MEMORY_MULTITHREAD

#endif // BG_MEMORY_MULTITHREAD

template <class T, class DeleterT>
struct SharedPointerPayload
{
    // The object being managed by the smart pointer
    T *managedObject = nullptr;
    
    // Count of actual references to the object, at zero the
    // managed object will be cleaned up
    long count = 1;

    // Count of weak references to the object. This count
    // does not keep the managed object alive but does keep
    // the payload metadata alive so any remaining weak references
    // can find out if the managed object is still alive
    long weakCount = 0;

    // Functional object in charge of cleaning up the managed object.
    DeleterT deleter;

    SharedPointerPayload() {}
    SharedPointerPayload(const DeleterT &d)
    {
        deleter = d;
    }
    SharedPointerPayload(DeleterT &d)
    {
        deleter = d;
    }
};

#endif // BGMEMORY_INCLUDE_BGMEMORY_POINTERS_SHAREDPOINTERPAYLOAD_HXX_