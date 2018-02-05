#ifndef temps_H
#define temps_H

#if defined _SOLARIS || defined _LINUX
  #include <sys/resource.h>
  #include <sys/time.h>
  #include <time.h>
  #include <sys/types.h>
  #include <unistd.h>
#endif

#ifdef _SOLARIS
  #include <sys/times.h>
#endif

#ifdef _LINUX
  #define hrtime_t clockid_t
#endif

#if defined _SOLARIS || defined _LINUX
typedef struct {
  /* durée décomposé en sous unités */
  long		  sec;
  long		  microSec;
  hrtime_t	nanoSec;
  /* cumuls */
  long		  csec;		/* temps ecoulé en sec */
  long		  cmicroSec;	/* temps ecoulé en micro sec */
  hrtime_t	cnanoSec;	/* temps ecoulé en nano sec */
} Chrono;
#endif
#ifdef _WINDOWS
typedef struct {
  /* durée décomposé en sous unités */
  long		sec;
  long		microSec;
  /* cumuls */
  long		csec;		/* temps ecoulé en sec */
  long		cmicroSec;	/* temps ecoulé en micro sec */
} Chrono;
#endif

Chrono* newChrono (void);
void startChrono (Chrono* chrono);
long topChrono (Chrono* chrono);
void startChronoDef (void);
long topChronoDef (void);
long getChronoDefMicroSec (void);
long getChronoNanoSec (Chrono* chrono);
long getChronoMicroSec (Chrono* chrono);
long getChronoSec (Chrono* chrono);

char* getDate (void);
char* getFormattedDate (char* fmt);
char* getAnneeMois (void);
char* getDatePid (void);

#endif
