#ifndef _MACROS_H
#define _MACROS_H

#include <stdio.h>

#ifdef ENABLE_LOGS
    #define LOG(fmt, ...) printf("[LOG]: " fmt " [%s:%d]\n", ##__VA_ARGS__, __FILE__, __LINE__)
#else
    #define LOG(fmt, ...)
#endif

#endif // _MACROS_H