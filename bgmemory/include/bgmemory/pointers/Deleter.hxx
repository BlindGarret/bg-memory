// Copyright [2020] BlindGarret<lroe2930@gmail.com>
#ifndef BGMEMORY_INCLUDE_BGMEMORY_POINTERS_DELETER_HXX_
#define BGMEMORY_INCLUDE_BGMEMORY_POINTERS_DELETER_HXX_

#ifdef BG_MEMORY_MULTITHREAD

#endif // BG_MEMORY_MULTITHREAD

namespace bg
{
    /*
        Deleter interface used for cleaning up smart pointers.
    */
    template <class T>
    class Deleter
    {
    public:
        virtual void operator()(T *pointer) = 0;
    };

} // namespace bg

#endif // BGMEMORY_INCLUDE_BGMEMORY_POINTERS_DELETER_HXX_