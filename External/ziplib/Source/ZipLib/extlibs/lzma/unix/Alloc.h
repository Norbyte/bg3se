/* Alloc.h -- Memory allocation functions
2009-02-07 : Igor Pavlov : Public domain */

#ifndef __COMMON_ALLOC_H
#define __COMMON_ALLOC_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void *MyAlloc(size_t size);
void MyFree(void *address);

void SetLargePageSize();

void *MidAlloc(size_t size);
void MidFree(void *address);
void *BigAlloc(size_t size);
void BigFree(void *address);

#ifdef __cplusplus
}
#endif

#endif
