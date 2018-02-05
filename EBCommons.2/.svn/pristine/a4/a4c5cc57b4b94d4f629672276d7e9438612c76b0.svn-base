
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> /* pour gérer les va_list */
#include <exploit.h>  /* pour CODE_FIN_ERREUR_APPLICATIVE */
#include "Erreur.h"
#include "inout.h"
#include "Utile.h"
#include "Message.h"

#include "Timer_struct.h"
#include "Timer_basics.h"

// Coefficient pour diviser les compteurs et obtenir une précision du timer final en millisec
#define MS_frac     1000000

// type d'horloge à utiliser
    // System-wide real-time clock. This clock is supported by all implementations and returns
    // the number of seconds and nanoseconds since the Epoch. This clock can be set via clock_settime
    // but doing so requires appropriate privileges. When the system time is changed, timers that measure
    // relative intervals are not affected but timers for absolute point in times are.
//#define CLOCK_ID            CLOCK_REALTIME
//#define CLOCK_ID_retenue    "CLOCK_REALTIME"

    // Represents monotonic time since some unspecified starting point. This clock cannot be set.
#define CLOCK_ID            CLOCK_MONOTONIC
#define CLOCK_ID_retenue    "CLOCK_MONOTONIC"

    // High-resolution per-process timer from the CPU.
//#define CLOCK_ID            CLOCK_PROCESS_CPUTIME_ID
//#define CLOCK_ID_retenue    "CLOCK_PROCESS_CPUTIME_ID"
    // Thread-specific CPU-time clock.
//#define CLOCK_ID            CLOCK_THREAD_CPUTIME_ID
//#define CLOCK_ID_retenue    "CLOCK_THREAD_CPUTIME_ID"



// précisions obtenues avec clock_getres()
// CLOCK_REALTIME: 999848 ns -> environ 1 ms
// CLOCK_MONOTONIC: 999848 ns -> environ 1 ms
// CLOCK_PROCESS_CPUTIME_ID: 1 ns
// CLOCK_THREAD_CPUTIME_ID: 1 ns
// Y-a-t-il un risque à utiliser une horloge de très grande précision ?

//************************************************************************
// taille statique (hors malloc des pointeurs internes) des types du module
//************************************************************************
void Timer_printSizeofTypes (void)
{
  IN();
  Message_printDebug(LOG,"Empreinte mémoire des types Motif_**** :");
  Message_printDebug(LOG,"sizeof(Timer)         = %d",    sizeof(Timer        ));
  Message_printDebug(LOG,"sizeof(TimerColl)     = %d",    sizeof(TimerColl    ));
  OUT(0);
  return;
}



//************************************************************************
// constructeur
//************************************************************************
int TimerColl_new (TimerColl** timerColl)
{
  IN();
  int ret = 0;
  if ((*timerColl = malloc (sizeof (TimerColl))) == NULL) {
    Erreur_set (CLA_SYS, nomFonction, "Probleme d'allocation memoire");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
  }
  else {
    memset (*timerColl, 0, sizeof (TimerColl));
  }
  Message_printDebugMem(TAG_MALLOC,*timerColl,1,sizeof(TimerColl),sizeof(TimerColl));
  OUT(0);
  return ret;
}

//************************************************************************
// destructeur
//************************************************************************
int TimerColl_free (TimerColl** timerColl)
{
  IN();
  Message_printDebug(TAG_FREE,"%s(), %s : %p",nomFonction,"*timerColl",*timerColl);
  if (*timerColl != NULL) {
    free (*timerColl);
    *timerColl = NULL;
  }
  OUT(0);
  return 0;
}

//************************************************************************
//* Recycle l'espace memoire occupé par une structure de données         *
//* précedemment allouée                                                 *
//* Permet d'éviter d'allouer et de désallouer constamment la même       *
//* structure de données, car cela coûte un appel système et de          *
//* réutiliser l'espace mémoire en le nettoyant                          *
//************************************************************************
int TimerColl_recycle (TimerColl** timerColl)
{
  int   ret = 0;
  IN();
  if (timerColl == NULL) {
    Erreur_set (CLA_APP, nomFonction, "Impossible de recycler une structure NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
  }
  else {
    memset (timerColl, 0, sizeof (TimerColl));
  }
  OUT(ret);
  return ret;
}


//*****************************************************************************
// creation
int TimerColl_set (TimerColl* timerColl, char* name)
{
  int   ret =  0;
  int   idx = -1;
  IN();
  if (NULL == timerColl) {
    Erreur_set (CLA_APP, nomFonction, "timerColl est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (name == NULL) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (strlen(name) >= L_timerName) {
    Erreur_set (CLA_APP, nomFonction, "nom trop long");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if ((ret = TimerColl_getIdxByName (timerColl, name, &idx)) != 0) goto XIT;
  if (-1 == idx) {
    idx = timerColl->nbUsed;
    timerColl->nbUsed += 1;
    strcpy (timerColl->coll[idx].name, name);
    TimerColl_reset (timerColl, name);
  }
  else {
    Erreur_set (CLA_APP, nomFonction, "Le timer existe déjà, name = %s, idx = %d",
                                      timerColl->coll[idx].name, idx);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
 XIT:
  OUT(ret);
  return ret;
}


//*****************************************************************************
int TimerColl_getIdxByName (TimerColl* timerColl, char* name, int* idx)
{
  int   ret = 0;
  IN();
  if (NULL == timerColl) {
    Erreur_set (CLA_APP, nomFonction, "timerColl est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (NULL == name) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (timerColl->nbUsed<=0) {
    *idx = -1;
  }
  else {
    int i;
    for (i=0;i<timerColl->nbUsed;i++)
    {
      if (strcmp (timerColl->coll[i].name, name) == 0) {*idx = i; break;}
    }
    if (i>=timerColl->nbUsed) { *idx = -1; } // pas trouvé
  }
 XIT:
  OUT(ret);
  if (ret != 0) *idx = -1;
  return ret;
}


//*****************************************************************************
// reset
int TimerColl_reset (TimerColl* timerColl, char* name)
{
  int   ret =  0;
  int   idx = -1;
  IN();
  if ((ret = TimerColl_getIdxByName (timerColl, name, &idx)) != 0) goto XIT;
  if (-1 == idx) {
    Erreur_set (CLA_APP, nomFonction, "Le timer n'existe pas, name = %s, idx = %d",
                                      timerColl->coll[idx].name, idx);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  else {
    memset(&timerColl->coll[idx].TM_beg,0,sizeof (struct timespec));
    memset(&timerColl->coll[idx].TM_end,0,sizeof (struct timespec));
    timerColl->coll[idx].cnt  = 0;
    timerColl->coll[idx].cur  = 0;
    timerColl->coll[idx].sum  = 0;
    timerColl->coll[idx].avg  = 0;
    timerColl->coll[idx].min  = 0;
    timerColl->coll[idx].max  = 0;
    timerColl->coll[idx].imin = 0;
    timerColl->coll[idx].imax = 0;
  }
 XIT:
  OUT(ret);
  return ret;
}


//*****************************************************************************
// T0 du timer
int TimerColl_begin (TimerColl* timerColl, char* name)
{
  IN();
  int   ret =  0;
  int   idx = -1;
  if (NULL == timerColl) {
    Erreur_set (CLA_APP, nomFonction, "timerColl est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (name == NULL) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if ((ret = TimerColl_getIdxByName (timerColl, name, &idx)) != 0) goto XIT;
  if (-1 == idx)
  {
    if ((ret = TimerColl_set (timerColl, name)) != 0) goto XIT;
    // apres creation => recherche de l'indice du nouveau compteur pour initialisation
    if ((ret = TimerColl_getIdxByName (timerColl, name, &idx)) != 0) goto XIT;
    if (-1 == idx) {
      Erreur_set (CLA_APP, nomFonction, "Pb à la creation du compteur");
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }
  // on ne met que ces champs là à jour : les autres sont cumulatifs dans le temps
  timerColl->coll[idx].cur = 0;
  // initialisation du T_0
  clock_gettime(CLOCK_ID, &timerColl->coll[idx].TM_beg);
 XIT:
  OUT(ret);
  return ret;
}

//*****************************************************************************
// mise à jour
int TimerColl_updateTimer (TimerColl* timerColl, char* name)
{
  int   ret =  0;
  int   idx = -1;
  long long sec  = 0;
  long long nsec = 0;
  IN();
  if (NULL == timerColl) {
    Erreur_set (CLA_APP, nomFonction, "timerColl est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (name == NULL) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if ((ret = TimerColl_getIdxByName (timerColl, name, &idx)) != 0) goto XIT;
  if (-1 == idx) {
    if ((ret = TimerColl_set (timerColl, name)) != 0) goto XIT;
  }
  timerColl->coll[idx].cnt += 1   ;
  clock_gettime(CLOCK_ID, &timerColl->coll[idx].TM_end);
  sec  = (long long) timerColl->coll[idx].TM_end.tv_sec  - (long long) timerColl->coll[idx].TM_beg.tv_sec  ;
  nsec = (long long) timerColl->coll[idx].TM_end.tv_nsec - (long long) timerColl->coll[idx].TM_beg.tv_nsec ;
  // calcul du temps écoulé depuis le passage précédent dans la fonction : en nanosec
  timerColl->coll[idx].cur = sec * 1000000000LL + nsec;
  // duree cumulée en nanosec
  timerColl->coll[idx].sum += (timerColl->coll[idx].cur > 0) ? timerColl->coll[idx].cur : 0;
  #if 0 // pour controler les valeurs d'un compteur
  if (strcmp (timerColl->coll[idx].name, "Entnum") == 0)
  //if (strcmp (timerColl->coll[idx].name, "Lecture BDD") == 0)
  {
    Message_print("TIMER","[%d, %d %s] Timer : beg = %lld + %lld , end = %lld + %lld  , sec = %lld  , nsec = %lld"
                  , timerColl->coll[idx].cnt, idx, timerColl->coll[idx].name
                  , (long long)timerColl->coll[idx].TM_beg.tv_sec
                  , (long long)timerColl->coll[idx].TM_beg.tv_nsec
                  , (long long)timerColl->coll[idx].TM_end.tv_sec
                  , (long long)timerColl->coll[idx].TM_end.tv_nsec
                  , (long long)sec
                  , (long long)nsec
                  );
  }
  #endif
  // calcul de la moyenne
  timerColl->coll[idx].avg = timerColl->coll[idx].sum / timerColl->coll[idx].cnt;
  if (timerColl->coll[idx].cnt == 1)
  {
    timerColl->coll[idx].min  = timerColl->coll[idx].cur;
    timerColl->coll[idx].max  = timerColl->coll[idx].cur;
    timerColl->coll[idx].imin = timerColl->coll[idx].cnt;
    timerColl->coll[idx].imax = timerColl->coll[idx].cnt;
  }
  if (timerColl->coll[idx].cur > 0 && timerColl->coll[idx].min > timerColl->coll[idx].cur)
  {
    timerColl->coll[idx].min  = timerColl->coll[idx].cur;
    timerColl->coll[idx].imin = timerColl->coll[idx].cnt;
  }
  if (timerColl->coll[idx].max < timerColl->coll[idx].cur)
  {
    timerColl->coll[idx].max  = timerColl->coll[idx].cur;
    timerColl->coll[idx].imax = timerColl->coll[idx].cnt;
  }
  // l'instant de fin devient l'instant du debut pour l'étape suivante
  timerColl->coll[idx].TM_beg = timerColl->coll[idx].TM_end;
 XIT:
  OUT(ret);
  return ret;
}


//*****************************************************************************
// affichage d'un compteur
int TimerColl_toString (TimerColl* timerColl, char* name, char* toString)
{
  int   ret = 0;
  int   idx = -1;
  #define L_10000c  10000
  IN();
  if (NULL == timerColl) {
    Erreur_set (CLA_APP, nomFonction, "timerColl est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (name == NULL) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (toString == NULL) {
    Erreur_set (CLA_APP, nomFonction, "toString est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  memset (toString, 0, L_10000c+1);
  if ((ret = TimerColl_getIdxByName (timerColl, name, &idx)) != 0) goto XIT;
  if (-1 != idx) {
    sprintf (toString, "cnt=%-7ld imin=%-7ld imax=%-7ld  cur=%-7.1f  avg=%-7.1f  min=%-7.1f  max=%-7.1f  sum=%-7.1f ms"
            , timerColl->coll[idx].cnt
            , timerColl->coll[idx].imin
            , timerColl->coll[idx].imax
            , (float)(timerColl->coll[idx].cur)/(float)MS_frac
            , (float)(timerColl->coll[idx].avg)/(float)MS_frac
            , (float)(timerColl->coll[idx].min)/(float)MS_frac
            , (float)(timerColl->coll[idx].max)/(float)MS_frac
            , (float)(timerColl->coll[idx].sum)/(float)MS_frac
            );
  }
 XIT:
  OUT(ret);
  return ret;
}


//*****************************************************************************
// affichage des compteurs
int TimerColl_print (TimerColl* timerColl, int (*Message_print)() )
{
  int   ret = 0;
  char* padding = "...........................................................";
  IN();
  if (NULL == timerColl) {
    Erreur_set (CLA_APP, nomFonction, "timerColl est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (timerColl->nbUsed >= 0)
  {
    int i;
    int lmaxName = 0;
    int lName = 0;
    for (i=0;i<timerColl->nbUsed;i++)
    {
      lName = strlen (timerColl->coll[i].name);
      lmaxName = (lmaxName < lName) ? lName : lmaxName;
    }
    Message_print (LOG, "[%d] Timers", timerColl->nbUsed);
    for (i=0;i<timerColl->nbUsed;i++)
    {
      lName = strlen (timerColl->coll[i].name);
      Message_print (LOG, "[%2d] %s %.*s : cnt=%-7ld imin=%-7ld imax=%-7ld  avg=%-7.1f  min=%-7.1f  max=%-7.1f  sum=%-7.1f ms"
            , i
            , timerColl->coll[i].name
            , 2+lmaxName-lName
            , padding
            , timerColl->coll[i].cnt
            , timerColl->coll[i].imin
            , timerColl->coll[i].imax
            , (float)(timerColl->coll[i].avg)/(float)MS_frac
            , (float)(timerColl->coll[i].min)/(float)MS_frac
            , (float)(timerColl->coll[i].max)/(float)MS_frac
            , (float)(timerColl->coll[i].sum)/(float)MS_frac
            );
    }
  }
 XIT:
  OUT(ret);
  return ret;
}


//*****************************************************************************
int TimerColl_getNbUsed (TimerColl* timerColl)
{
  int   ret = 0;
  IN();
  if (NULL == timerColl) {
    Erreur_set (CLA_APP, nomFonction, "timerColl est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
 XIT:
  OUT(ret);
  return timerColl->nbUsed;
}


//*****************************************************************************
int TimerColl_getClockRes (void)
{
  int ret;
  struct timespec res;

	//if ((ret = clock_getres(CLOCK_REALTIME, &res)) == 0) Message_print(LOG,"CLOCK_REALTIME: %ld ns", res.tv_nsec);
	if ((ret = clock_getres(CLOCK_MONOTONIC, &res)) == 0) Message_print(LOG,"CLOCK_MONOTONIC: %ld ns", res.tv_nsec);
	//if ((ret = clock_getres(CLOCK_PROCESS_CPUTIME_ID, &res)) == 0) Message_print(LOG,"CLOCK_PROCESS_CPUTIME_ID: %ld ns", res.tv_nsec);
	//if ((ret = clock_getres(CLOCK_THREAD_CPUTIME_ID, &res)) == 0) Message_print(LOG,"CLOCK_THREAD_CPUTIME_ID: %ld ns", res.tv_nsec);
	//if ((ret = clock_getres(CLOCK_REALTIME_HR, &res)) == 0) Message_print(LOG,"CLOCK_REALTIME: %ld ns", res.tv_nsec);
	//if ((ret = clock_getres(CLOCK_MONOTONIC_HR, &res)) == 0) Message_print(LOG,"CLOCK_MONOTONIC: %ld ns", res.tv_nsec);
  Message_print(LOG,"CLOCK retenue: %s", CLOCK_ID_retenue);
  return ret;
}
