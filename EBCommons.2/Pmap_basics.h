#ifndef _Pmap_basics_H
#define _Pmap_basics_H

#include "Pmap_const.h"
#include "Pmap_struct.h"

// allocation :
// on transmet aussi la fonction de desallocation de la valeur pointeur
int Pmapelm_new  (Pmapelm** pmapelm, void(*free_p)(void* ptr) );

// déallocation
int Pmapelm_free (Pmapelm** pmapelm);

// nettoyage
int Pmapelm_clean (Pmapelm* pmapelm);

// definition
int Pmapelm_set (Pmapelm* pmapelm, char* name, Pmaptype t, Pmapval val, void (*free_p)(void*));

// restitution : name, type et valeur d'un pmapelm
int Pmapelm_get (Pmapelm* pmapelm, char* name, Pmaptype* t, Pmapval* v);

// test du type
int Pmapelm_hasType (Pmapelm* pmapelm, Pmaptype t, int* yes);


////////////////////////////////////////////////////////////////////////
// COLLECTION
////////////////////////////////////////////////////////////////////////
//
// allocation
int Pmap_new  (Pmap** pmap, int nbAlloc);

// déallocation
int Pmap_free (Pmap** pmap);

// nb utilsés dans la collection
int Pmap_getNbUsed (Pmap* pmap);

// nb de places disponibles dans la collection
int Pmap_getNbAlloc (Pmap* pmap);

// high water mark de la collection
int Pmap_getHwm (Pmap* pmap);

// creation / reset
int Pmap_put (Pmap* pmap, char* name, Pmaptype t, Pmapval val, int* idx, void (*free_p)(void*));

// suppression
int Pmap_remove (Pmap* pmap, char* name);

// retourne l'index d'un element de la collection à partir de son nom
int Pmap_getIdxByName (Pmap* pmap, char* name, int* idx);

// retourne l'index du premier element vide
// etend la collection si necessaire
int Pmap_getNextFreeIdx (Pmap* pmap, int* idx);

// restitution de la valeur à partir de son indice
int Pmap_getValByIdx (Pmap* pmap, int idx, char* name, Pmaptype* t, Pmapval* v);

// restitution de la valeur à partir de son nom
int Pmap_getValByName (Pmap* pmap, char* name, Pmaptype* t, Pmapval* v, int* idx);

// affichage
int Pmap_print (Pmap* pmap);

#endif
