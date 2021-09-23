
#ifndef USPM_MEM_H
#define USPM_MEM_H

#include <stddef.h>
#include <stdio.h>
#include "build_config.h"

struct MemoryStruct {
    char *memory;
    size_t size;
};


static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif