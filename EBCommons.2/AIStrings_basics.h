#ifndef IAChar_basics_h
#define IAChar_basics_h

#include "AIStrings_struct.h"

void AIStrings_printSizeof(void);

// pseudo constructeur
AIStrings* AIStrings_new(int nb, size_t vol);
// ajoute un char* (= 1 ligne)  au AIStrings
int AIStrings_append(AIStrings *ais, char *s);
// destructeur
int AIStrings_free(AIStrings **ais);
// recycle / nettoie l'instance
int AIStrings_recycle(AIStrings *ais);

// retourne la ligne suivante
char* AIStrings_fetch(AIStrings *ais);
// remet l'indice de position au d�but
int AIStrings_rewind(AIStrings *ais);
// retourne l'indice de l'�l�ment courant
int AIStrings_getIcur(AIStrings *ais);
// retourne l'indice du dernier element
int AIStrings_getLastPos(AIStrings *ais);
// retourne le dernier element
char* AIStrings_getLast(AIStrings *ais);
// retourne la ligne � la position de l'indice
char* AIStrings_getDataAt(AIStrings *ais, int i);

// retourne le nb d'enregs
int AIStrings_getNb(AIStrings *ais);

// TODO AIStrings_newFromFile32() ??

#endif
