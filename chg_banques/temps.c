/************************************************************************/
/* Objet: fonctions de gestion du temps et des dates systeme		*/
/*                                                                      */
/* Création: 11/12/2002 E.Barillot                                      */
/*                                                                      */
/*                                                                      */
/************************************************************************/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "temps.h"



/************************************************************************/
/* Pour remplacer le fonction gethrtime qui existe sous Solaris et      */
/* pas sous Linux                                           						*/
/************************************************************************/
#ifdef _LINUX
static long long gethrtime(void) {
  struct timespec sp;
  int ret;
  long long v;
  ret = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &sp);
  if (ret != 0) return 0;
  v = 1000000000LL; /* seconds->nanonseconds */
  v *= sp.tv_sec;
  v += sp.tv_nsec;
  return v;
}
#endif


/************************************************************************/
/* variables globales							*/
/************************************************************************/
static Chrono chronoDef;

/************************************************************************/
/* constructeur								*/
/************************************************************************/
Chrono*	newChrono(void)
{
  Chrono* chrono = malloc (sizeof (Chrono));
  if (chrono) memset (chrono, 0, sizeof (chrono));
  return chrono;
}

/************************************************************************/
/* sauvegarde le temps de depart du chrono				*/
/************************************************************************/
void startChrono (Chrono* chrono)
{
  chrono->cnanoSec = gethrtime();
  /* printf("startChrono() %lld\n", chrono->cnanoSec); */
  return;
}

/************************************************************************/
/* temps ecoulé depuis le start en micro secondes			*/
/************************************************************************/
long topChrono (Chrono* chrono)
{
  /* cumuls */
  chrono->cnanoSec = gethrtime() - chrono->cnanoSec;
  /* printf("topChrono() %lld\n", chrono->cnanoSec); */
  chrono->cmicroSec = (long)(chrono->cnanoSec/(hrtime_t)1000);
  chrono->csec = chrono->cmicroSec/1000000;
  /* répartition */
  chrono->sec = chrono->csec;
  chrono->microSec = chrono->cmicroSec - chrono->csec*1000000;
  chrono->nanoSec = chrono->cnanoSec - (hrtime_t)(chrono->cmicroSec*1000);
  return (chrono->cmicroSec);
}

/************************************************************************/
/* sauvegarde le temps de depart du chrono				*/
/************************************************************************/
void startChronoDef (void)
{
  startChrono (&chronoDef);
  return;
}

/************************************************************************/
/* temps ecoulé depuis le start						*/
/************************************************************************/
long topChronoDef (void)
{
  return (topChrono (&chronoDef));
}

/************************************************************************/
/* valeur de la variable						*/
/************************************************************************/
long getChronoDefMicroSec (void)
{
  return (chronoDef.cmicroSec);
}


/************************************************************************/
/* valeur de la variable chrono en microsecondes			*/
/************************************************************************/
long getChronoNanoSec (Chrono* chrono)
{
  return (chrono->cnanoSec);
}

/************************************************************************/
/* valeur de la variable chrono en microsecondes			*/
/************************************************************************/
long getChronoMicroSec (Chrono* chrono)
{
  return (chrono->cmicroSec);
}

/************************************************************************/
/* valeur de la variable chrono en sec					*/
/************************************************************************/
long getChronoSec (Chrono* chrono)
{
  return (chrono->csec);
}

/************************************************************************/
/* retourne la date en chaine de caracteres				*/
/************************************************************************/
char* getDate (void)
{
#define L_dateOut       50
  /* static pour que l'adresse retournée soit toujours valide, qu'elle soit
   * utilisée ou non immédiatement après le return
   * sinon pose probleme si getDate() est passé en argument d'une fonction
   * notamment quand elle a un nb d'arguments variables */
  static char   dateOut[L_dateOut+1];
  struct timeval tval;
  struct tm *s_heure;

  memset (dateOut, 0, L_dateOut);
  gettimeofday (&tval, NULL);
  s_heure = localtime (&(tval.tv_sec));
  strftime (dateOut, 50, "%Y/%m/%d:%H:%M:%S", s_heure);
  return dateOut;
#undef L_dateOut
}

/************************************************************************/
/* retourne la date en chaine de caracteres				*/
/* au format strftime()							*/
/************************************************************************/
char* getFormattedDate (char* fmt)
{
#define L_dateOut       50
  /* static pour que l'adresse retournée soit toujours valide, qu'elle soit
   * utilisée ou non immédiatement après le return
   * sinon pose probleme si getDate() est passé en argument d'une fonction
   * notamment quand elle a un nb d'arguments variables */
  static char   dateOut[L_dateOut+1];
  struct timeval tval;
  struct tm *s_heure;

  memset (dateOut, 0, L_dateOut);
  gettimeofday (&tval, NULL);
  s_heure = localtime (&(tval.tv_sec));
  strftime (dateOut, 50, fmt, s_heure);
  return dateOut;
#undef L_dateOut
}

/************************************************************************/
/* retourne l'année et le mois						*/
/************************************************************************/
char* getAnneeMois (void)
{
#define L_dateOut       50
  /* static pour que l'adresse retournée soit toujours valide, qu'elle soit
   * utilisée ou non immédiatement après le return
   * sinon pose probleme si getDate() est passé en argument d'une fonction
   * notamment quand elle a un nb d'arguments variables */
  static char   dateOut[L_dateOut+1];
  struct timeval tval;
  struct tm *s_heure;

  memset (dateOut, 0, L_dateOut);
  gettimeofday (&tval, NULL);
  s_heure = localtime (&(tval.tv_sec));
  strftime (dateOut, 50, "%Y%m", s_heure);
  return dateOut;
#undef L_dateOut
}

/************************************************************************/
/* retourne la date en chaine de caracteres				*/
/* avec pid du process							*/
/************************************************************************/
char* getDatePid (void)
{
#define L_dateOut       50
  /* static pour que l'adresse retournée soit toujours valide, qu'elle soit
   * utilisée ou non immédiatement après le return
   * sinon pose probleme si getDate() est passé en argument d'une fonction
   * notamment quand elle a un nb d'arguments variables */
  static char   dateOut[L_dateOut+1];
  struct timeval tval;
  struct tm *s_heure;
  char datime[32];
  char s_pid[10];

  memset (dateOut, 0, L_dateOut);
  gettimeofday (&tval, NULL);
  s_heure = localtime (&(tval.tv_sec));
  strftime (datime, 31, "%Y%m%d.%H%M%S", s_heure);
  sprintf (s_pid, "%6d", getpid());
  sprintf (dateOut, "%s,%03d [%s]", datime, (int)(tval.tv_usec/1000), s_pid);
  return dateOut;
}
#undef L_dateOut
