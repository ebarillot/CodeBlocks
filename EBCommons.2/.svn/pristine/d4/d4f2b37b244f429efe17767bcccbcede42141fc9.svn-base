#ifndef _Timer_struct_h
#define _Timer_struct_h

#include <time.h>   // pour timespec et clock_gettime()


#define L_timerName           60



//****************************************************************************

typedef struct _Timer {
  struct timespec  TM_beg;
  struct timespec  TM_end;
  long      cnt   ;
  long      imin  ;
  long      imax  ;
  long long cur   ;
  long long sum   ;
  long long avg   ;
  long long min   ;
  long long max   ;
  char name[L_timerName+1];
} Timer;




//////////////////////////////////////////////////////////////
// Stucture pour une collection d'enregistrements
#define N_MAX_TIMERS    100
typedef struct _TimerColl {
  int       nbUsed                ;   // nb utilisé
  Timer     coll[N_MAX_TIMERS]    ;   // collection
} TimerColl;
//////////////////////////////////////////////////////////////

#endif
