#ifndef Timer_basics_h
#define Timer_basics_h

#include "Timer_struct.h"


#define OK_TIMER  0
#define NO_TIMER  -1

// taille statique (hors malloc des pointeurs internes) des types du module
void Timer_printSizeofTypes (void);

// constructeur
int TimerColl_new (TimerColl** timerColl);

// destructeur
int TimerColl_free (TimerColl** timerColl);

// Recycle l'espace memoire occupé par une structure de données
// précedemment allouée
// Permet d'éviter d'allouer et de désallouer constamment la même
// structure de données, car cela coûte un appel système et de
// réutiliser l'espace mémoire en le nettoyant
int TimerColl_recycle (TimerColl** timerColl);

// creation
int TimerColl_set (TimerColl* timerColl, char* name);

// reset
int TimerColl_reset (TimerColl* timerColl, char* name);

// recherche par le nom
int TimerColl_getIdxByName (TimerColl* timerColl, char* name, int* idx);

// T0 du timer
int TimerColl_begin (TimerColl* timerColl, char* name);

// mise à jour
int TimerColl_updateTimer (TimerColl* timerColl, char* name);

// affichage d'un compteur
int TimerColl_toString (TimerColl* timerColl, char* name, char* toString);

// affichage des compteurs
int TimerColl_print (TimerColl* timerColl, int (*Message_print)() );

// retourne le nb de timers utilisés
int TimerColl_getNbUsed (TimerColl* timerColl);

// affiche la résolution des différents horloges disponibles
int TimerColl_getClockRes (void);

#endif
