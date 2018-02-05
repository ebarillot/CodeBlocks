#ifndef _Counts_basics_H
#define _Counts_basics_H

#include "Counts_struct.h"

// en memoire
int Counts_new  (Counts** counts);
int Counts_free (Counts** counts);
// creation / reset
int Counts_set (Counts* counts, char* name);
int Counts_reset (Counts* counts, char* name);
int Counts_resetAll (Counts* counts);

// creation d'un compteur
int Counts_setNewCount (Counts* counts, char* name);
// retourne la valeur du compteur
int Counts_getValByName (Counts* counts, char* name);
// retourne la valeur du compteur
int Counts_getCountByName (Counts* counts, char* name);
// retourne le numéro du compteur
int Counts_getIdxCountByName (Counts* counts, char* name);

// défini la valeur du compteur
int Counts_setVal (Counts* counts, char* name, int nb);
// ajoute 1 à la valeur du compteur
int Counts_plus1  (Counts* counts, char* name);
// soustrait 1 à la valeur du compteur
int Counts_minus1 (Counts* counts, char* name);

// affichage des compteurs
int Counts_print (Counts* counts);

// retourne le nombre de compteurs
int Counts_getNb (Counts* counts);

// restitution du nom et de la valeur d'un compteur par son indice
int Counts_getCountByIdx (Counts* counts, int idx, char* name, int* val);


#endif
