#ifndef MEMORYFUNCTIONS_HXX__INCLUDED
#define MEMORYFUNCTIONS_HXX__INCLUDED

#include <stdlib.h>

namespace bg {
    void* allocateAlligned(size_t sizeInBytes, size_t alignment);
}

#endif //MEMORYFUNCTIONS_HXX__INCLUDED_INCLUDED