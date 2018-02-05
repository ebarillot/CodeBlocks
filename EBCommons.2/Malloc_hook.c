#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif


/* Prototypes for __malloc_hook, __free_hook */
#include <mcheck.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>



/* Prototypes for our hooks.  */
static void* (*old_malloc_hook)(size_t size, const void *caller);
static void  (*old_free_hook  )(void *ptr  , const void *caller);
static void  Malloc_hook_init(void);
static void* Malloc_hook(size_t size, const void *caller);
static void  Malloc_hook_free(void *, const void *caller);


void Malloc_hook_init(void)
{
  old_malloc_hook = __malloc_hook   ;
  old_free_hook   = __free_hook     ;
  __malloc_hook   = Malloc_hook     ;
  __free_hook     = Malloc_hook_free;
}

static void* Malloc_hook(size_t size, const void *caller)
{
  void *result;
  /* Restore all old hooks */
  __malloc_hook   = old_malloc_hook;
  __free_hook     = old_free_hook;
  /* Call recursively */
  result = malloc(size);
  /* Save underlying hooks */
  old_malloc_hook = __malloc_hook;
  old_free_hook   = __free_hook;
  /* printf might call malloc, so protect it too. */
  printf("malloc (%u) returns %p\n", (unsigned int) size, result);
  /* Restore our own hooks */
  __malloc_hook   = Malloc_hook;
  __free_hook     = Malloc_hook_free;
  return result;
}

static void Malloc_hook_free(void *ptr, const void *caller)
{
  /* Restore all old hooks */
  __malloc_hook = old_malloc_hook;
  __free_hook = old_free_hook;
  /* Call recursively */
  free(ptr);
  /* Save underlying hooks */
  old_malloc_hook = __malloc_hook;
  old_free_hook   = __free_hook;
  /* printf might call free, so protect it too. */
  printf("freed pointer %p\n", ptr);
  /* Restore our own hooks */
  __malloc_hook   = Malloc_hook;
  __free_hook     = Malloc_hook_free;
}

void display_mallinfo(void)
{
  struct mallinfo mi;

  mi = mallinfo();

  printf("Total non-mmapped bytes (arena):       %d\n", mi.arena);
  printf("# of free chunks (ordblks):            %d\n", mi.ordblks);
  printf("# of free fastbin blocks (smblks):     %d\n", mi.smblks);
  printf("# of mapped regions (hblks):           %d\n", mi.hblks);
  printf("Bytes in mapped regions (hblkhd):      %d\n", mi.hblkhd);
  printf("Max. total allocated space (usmblks):  %d\n", mi.usmblks);
  printf("Free bytes held in fastbins (fsmblks): %d\n", mi.fsmblks);
  printf("Total allocated space (uordblks):      %d\n", mi.uordblks);
  printf("Total free space (fordblks):           %d\n", mi.fordblks);
  printf("Topmost releasable block (keepcost):   %d\n", mi.keepcost);
}

int main(void)
{
  mtrace();

  Malloc_hook_init();

#define N_malloc  3
  int j;
  int* im[N_malloc];
  for (j = 0; j < N_malloc; j++)
    im[j] = malloc(100);

  calloc(16, 16);             /* Never freed--a memory leak */

  for (j = 0; j < N_malloc; j++)
    free(im[j]);

  display_mallinfo();
  //malloc_stats();
  exit(EXIT_SUCCESS);
}

