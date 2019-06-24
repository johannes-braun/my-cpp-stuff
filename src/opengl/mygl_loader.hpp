#define MYGL_DISPATCH_NAME dispatch_table
#pragma once
/* This header contains the function loader functionalities.
*/
#include "mygl_functions.hpp"

namespace mygl { 
void load(loader_function fun);
void load(MYGL_DISPATCH_NAME* d, loader_function fun);
}

#if defined(MYGL_IMPLEMENTATION)
    #include "mygl_loader.inl"
#endif
#undef MYGL_DISPATCH_NAME
