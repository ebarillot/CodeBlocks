/*
  gettime - get time via clock_gettime
  N.B.: OS X does not support clock_gettime

  Paul Krzyzanowski
*/

#include <stdio.h>  /* for printf */
#include <stdint.h> /* for uint64 definition */
#include <stdlib.h> /* for exit() definition */
#include <time.h> /* for clock_gettime */

#define BILLION 1000000000L

int localpid(void) {
  static int a[9] = { 0 };
  return a[0];
}

main(int argc, char **argv)
{
  uint64_t diff;
  struct timespec start, end;
  int i;

  /* measure monotonic time */
//#define CLOCK_ID CLOCK_THREAD_CPUTIME_ID

#define CLOCK_ID        CLOCK_MONOTONIC
#define CLOCK_ID_NAME   "CLOCK_MONOTONIC"
  printf("%s\n", CLOCK_ID_NAME);
  {
    clock_gettime(CLOCK_ID, &start); /* mark start time */
    sleep(1); /* do stuff */
    clock_gettime(CLOCK_ID, &end); /* mark the end time */

    printf("start = %ld sec %ld nanoseconds\n",start.tv_sec,start.tv_nsec);
    printf("end   = %ld sec %ld nanoseconds\n",end  .tv_sec,end  .tv_nsec);

    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);

    /* now re-do this and measure CPU time */
    /* the time spent sleeping will not count (but there is a bit of overhead */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);  /* mark start time */
    sleep(1); /* do stuff */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);    /* mark the end time */

    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) diff);
  }
#undef CLOCK_ID
#undef CLOCK_ID_NAME

#define CLOCK_ID        CLOCK_PROCESS_CPUTIME_ID
#define CLOCK_ID_NAME   "CLOCK_PROCESS_CPUTIME_ID"
  printf("%s\n", CLOCK_ID_NAME);
  {
    clock_gettime(CLOCK_ID, &start); /* mark start time */
    sleep(1); /* do stuff */
    clock_gettime(CLOCK_ID, &end); /* mark the end time */

    printf("start = %ld sec %ld nanoseconds\n",start.tv_sec,start.tv_nsec);
    printf("end   = %ld sec %ld nanoseconds\n",end  .tv_sec,end  .tv_nsec);

    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);

    /* now re-do this and measure CPU time */
    /* the time spent sleeping will not count (but there is a bit of overhead */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);  /* mark start time */
    sleep(1); /* do stuff */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);    /* mark the end time */

    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) diff);
  }
#undef CLOCK_ID
#undef CLOCK_ID_NAME


  exit(0);
}

