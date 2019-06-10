#define MYGL_DISPATCH_NAME dispatch_table
#pragma once
/* This header contains the function loader functionalities.
*/
#include "mygl_functions.hpp"

namespace mygl { 
void read();
void read(loader_function fun);
void read(MYGL_DISPATCH_NAME* d);
void read(MYGL_DISPATCH_NAME* d, loader_function fun);
}

#if defined(MYGL_IMPLEMENTATION)
    #include "mygl_loader.inl"
#endif
#undef MYGL_DISPATCH_NAME
