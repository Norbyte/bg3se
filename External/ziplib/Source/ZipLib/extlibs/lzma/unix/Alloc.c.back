/* Alloc.c -- Memory allocation functions
2008-09-24
Igor Pavlov
Public domain */

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdlib.h>

#ifdef _7ZIP_LARGE_PAGES
#ifdef __linux__
#ifndef _7ZIP_ST
#include <pthread.h>
#endif
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#endif
#endif

#include "Alloc.h"

/* #define _SZ_ALLOC_DEBUG */

/* use _SZ_ALLOC_DEBUG to debug alloc/free operations */
#ifdef _SZ_ALLOC_DEBUG
#include <stdio.h>
int g_allocCount = 0;
int g_allocCountMid = 0;
int g_allocCountBig = 0;
#endif

void *MyAlloc(size_t size)
{
  if (size == 0)
    return 0;
  #ifdef _SZ_ALLOC_DEBUG
  {
    void *p = malloc(size);
    fprintf(stderr, "\nAlloc %10d bytes, count = %10d,  addr = %8X", size, g_allocCount++, (unsigned)p);
    return p;
  }
  #else
  return malloc(size);
  #endif
}

void MyFree(void *address)
{
  #ifdef _SZ_ALLOC_DEBUG
  if (address != 0)
    fprintf(stderr, "\nFree; count = %10d,  addr = %8X", --g_allocCount, (unsigned)address);
  #endif
  free(address);
}

#ifndef _WIN32

#define PAGE_READWRITE 0x04
#define MEM_COMMIT 0x1000
#define MEM_RELEASE 0x8000
#ifdef _7ZIP_LARGE_PAGES
#define MEM_LARGE_PAGES 0x20000000
#ifdef __linux__
#define _7ZIP_MAX_HUGE_ALLOCS 64
static void *g_HugePageAddr[_7ZIP_MAX_HUGE_ALLOCS] = { NULL };
static size_t g_HugePageLen[_7ZIP_MAX_HUGE_ALLOCS];
static char *g_HugetlbPath;
#endif
#endif

static void *VirtualAlloc(void *address, size_t size, unsigned int type, unsigned int protect)
{
  #ifdef _7ZIP_LARGE_PAGES
  if (type & MEM_LARGE_PAGES)
  {
    #ifdef __linux__
    /* huge pages support for Linux; added by Joachim Henke */
    #ifndef _7ZIP_ST
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    #endif
    int i;

    address = NULL;
    #ifndef _7ZIP_ST
    pthread_mutex_lock(&mutex);
    #endif
    for (i = 0; i < _7ZIP_MAX_HUGE_ALLOCS; ++i)
    {
      if (g_HugePageAddr[i] == NULL)
      {
        int fd, pathlen = strlen(g_HugetlbPath);
        char tempname[pathlen+12];

        memcpy(tempname, g_HugetlbPath, pathlen);
        memcpy(tempname + pathlen, "/7z-XXXXXX", 11);
        fd = mkstemp(tempname);
printf("BigAlloc(%s)=>%d\n",tempname,fd);
        unlink(tempname);
        if (fd < 0)
          break;
        address = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
        if (address == MAP_FAILED)
        {
          address = NULL;
          break;
        }
        g_HugePageLen[i] = size;
        g_HugePageAddr[i] = address;
printf("HUGE[%d]=%ld %p\n",i,(long)size,address);
        break;
      }
    }
    #ifndef _7ZIP_ST
    pthread_mutex_unlock(&mutex);
    #endif
    return address;
    #endif
  }
  #endif
  return malloc(size);
}

static int VirtualFree(void *address, size_t size, unsigned int type)
{
  #ifdef _7ZIP_LARGE_PAGES
  #ifdef __linux__
  int i;

  for (i = 0; i < _7ZIP_MAX_HUGE_ALLOCS; ++i)
  {
    if (g_HugePageAddr[i] == address)
    {
      munmap(address, g_HugePageLen[i]);
      g_HugePageAddr[i] = NULL;
      return 1;
    }
  }
  #endif
  #endif
  free(address);
  return 1;
}

#endif

void *MidAlloc(size_t size)
{
  if (size == 0)
    return 0;
  #ifdef _SZ_ALLOC_DEBUG
  fprintf(stderr, "\nAlloc_Mid %10d bytes;  count = %10d", size, g_allocCountMid++);
  #endif
  return VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);
}

void MidFree(void *address)
{
  #ifdef _SZ_ALLOC_DEBUG
  if (address != 0)
    fprintf(stderr, "\nFree_Mid; count = %10d", --g_allocCountMid);
  #endif
  if (address == 0)
    return;
  VirtualFree(address, 0, MEM_RELEASE);
}

#ifndef MEM_LARGE_PAGES
#undef _7ZIP_LARGE_PAGES
#endif

#ifdef _7ZIP_LARGE_PAGES
size_t g_LargePageSize = 0;
#ifdef _WIN32
typedef SIZE_T (WINAPI *GetLargePageMinimumP)();
#elif defined(__linux__)
size_t largePageMinimum()
{
  size_t size;

  g_HugetlbPath = getenv("HUGETLB_PATH");
  if (g_HugetlbPath == NULL || (size = pathconf(g_HugetlbPath, _PC_REC_MIN_XFER_SIZE)) <= getpagesize())
    return 0;
  return size;
}
#else
#define largePageMinimum() 0
#endif
#endif

void SetLargePageSize()
{
  #ifdef _7ZIP_LARGE_PAGES
  size_t size;
  #ifdef _WIN32
  GetLargePageMinimumP largePageMinimum = (GetLargePageMinimumP)
        GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetLargePageMinimum");
  if (largePageMinimum == 0)
    return;
  #endif
  size = largePageMinimum();
  if (size == 0 || (size & (size - 1)) != 0)
    return;
  g_LargePageSize = size;
printf("SetLargePageSize : %ld\n",(long)g_LargePageSize);
  #endif
}


void *BigAlloc(size_t size)
{
  if (size == 0)
    return 0;
  #ifdef _SZ_ALLOC_DEBUG
  fprintf(stderr, "\nAlloc_Big %10d bytes;  count = %10d", size, g_allocCountBig++);
  #endif
  
  #ifdef _7ZIP_LARGE_PAGES
  if (g_LargePageSize != 0 && g_LargePageSize <= (1 << 30) && size >= (1 << 18))
  {
    void *res = VirtualAlloc(0, (size + g_LargePageSize - 1) & (~(g_LargePageSize - 1)),
        MEM_COMMIT | MEM_LARGE_PAGES, PAGE_READWRITE);
printf("BigAlloc : %ld %ld => %p\n",(long)g_LargePageSize,(long)size,res);
    if (res != 0)
      return res;
  }
  #endif
  return VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);
}

void BigFree(void *address)
{
  #ifdef _SZ_ALLOC_DEBUG
  if (address != 0)
    fprintf(stderr, "\nFree_Big; count = %10d", --g_allocCountBig);
  #endif
  
  if (address == 0)
    return;
  VirtualFree(address, 0, MEM_RELEASE);
}
