#include "utils.h"
#include <omp.h>
#define parallel_for _Pragma("omp parallel for") for

namespace ligra{
template<typename Type>
void resize_value(std::vector<Type>&value, Type initial)
{
    u32 n = value.size();
    parallel_for(u32 i=0;i<n;i++)
    {
        value[i] = initial;
    }
}
}

