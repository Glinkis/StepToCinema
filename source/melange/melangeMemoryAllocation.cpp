#include "../StepToCinema.h"

using namespace melange;

// memory allocation functions inside melange namespace
// (if you have your own memory management you can overload these functions)
namespace melange
{
    // alloc memory no clear
    void *MemAllocNC(Int size)
    {
        void *mem = malloc(size);
        return mem;
    }

    // alloc memory set to 0
    void *MemAlloc(Int size)
    {
        void *mem = MemAllocNC(size);
        memset(mem, 0, size);
        return mem;
    }

    // realloc existing memory
    void *MemRealloc(void* orimem, Int size)
    {
        void *mem = realloc(orimem, size);
        return mem;
    }

    // free memory and set pointer to null
    void MemFree(void *&mem)
    {
        if (!mem)
        {
            return;
        }

        free(mem);
        mem = nullptr;
    }
}