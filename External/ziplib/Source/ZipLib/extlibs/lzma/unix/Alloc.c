/* Alloc.c -- Memory allocation functions
2008-09-24
Igor Pavlov
Public domain */

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#ifdef _7ZIP_LARGE_PAGES
#ifdef __linux__
#ifndef _7ZIP_ST
#include <pthread.h>
#endif
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <mntent.h>
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

#ifdef _7ZIP_LARGE_PAGES

#ifdef __linux__
#define _7ZIP_MAX_HUGE_ALLOCS 64
static void *g_HugePageAddr[_7ZIP_MAX_HUGE_ALLOCS] = { NULL };
static size_t g_HugePageLen[_7ZIP_MAX_HUGE_ALLOCS];
static char *g_HugetlbPath;
#endif

#endif

static void *VirtualAlloc(size_t size, int memLargePages)
{
  #ifdef _7ZIP_LARGE_PAGES
  if (memLargePages)
  {
    #ifdef __linux__
    /* huge pages support for Linux; added by Joachim Henke */
    #ifndef _7ZIP_ST
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    #endif
    int i;

    void * address = NULL;
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
        unlink(tempname);
        if (fd < 0)
        {
          fprintf(stderr,"cant't open %s (%s)\n",tempname,strerror(errno));
          break;
        }
        address = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
        if (address == MAP_FAILED)
        {
          address = NULL;
          break;
        }
        g_HugePageLen[i] = size;
        g_HugePageAddr[i] = address;
// fprintf(stderr,"HUGE[%d]=%ld %p\n",i,(long)size,address);
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

static int VirtualFree(void *address)
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
  return VirtualAlloc(size, 0);
}

void MidFree(void *address)
{
  #ifdef _SZ_ALLOC_DEBUG
  if (address != 0)
    fprintf(stderr, "\nFree_Mid; count = %10d", --g_allocCountMid);
  #endif
  if (address == 0)
    return;
  VirtualFree(address);
}

#ifdef _7ZIP_LARGE_PAGES
size_t g_LargePageSize = 0;
#ifdef _WIN32
typedef SIZE_T (WINAPI *GetLargePageMinimumP)();
#elif defined(__linux__)
size_t largePageMinimum()
{
  size_t size;

  g_HugetlbPath = getenv("HUGETLB_PATH");

  if (g_HugetlbPath == NULL)
  {
    // not defined => try to find out the directory
    static char dir_hugetlbfs[1024];
    const char * filename = "/etc/mtab"; // mounted filesystems
    FILE *fp;
    struct mntent * info;

    dir_hugetlbfs[0]=0;

    fp = setmntent(filename,"r");
    if (fp)
    {
      info = getmntent(fp);
      while(info)
      {
/*
        printf("%s:\n",info->mnt_fsname);
        printf("  dir='%s'\n",info->mnt_dir);
        printf("  type='%s'\n",info->mnt_type);
*/

        if (strcmp(info->mnt_type,"hugetlbfs") == 0)
        {
          strcpy(dir_hugetlbfs,info->mnt_dir);
          break;
        }

        info = getmntent(fp);
      }
      endmntent(fp);
    }

    if (dir_hugetlbfs[0])
    {
      g_HugetlbPath = dir_hugetlbfs;
      // fprintf(stderr," Found hugetlbfs = '%s'\n",g_HugetlbPath);
    }
  }
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
  // fprintf(stderr,"SetLargePageSize : %ld\n",(long)g_LargePageSize);
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
    void *res = VirtualAlloc( (size + g_LargePageSize - 1) & (~(g_LargePageSize - 1)), 1);
    if (res != 0)
      return res;
  }
  #endif
  return VirtualAlloc(size, 0);
}

void BigFree(void *address)
{
  #ifdef _SZ_ALLOC_DEBUG
  if (address != 0)
    fprintf(stderr, "\nFree_Big; count = %10d", --g_allocCountBig);
  #endif
  
  if (address == 0)
    return;
  VirtualFree(address);
}
