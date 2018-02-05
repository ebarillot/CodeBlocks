

#include <stdio.h>
#include <string.h>


#include "ABS.h"


#define MIN_HWM -1

///////////////////////////////////////////////////////////////////////////////
// TODO pas besoin de reallouer l'enveloppe du conteneur, juste le conteneur d'elements
static ABS* _resize(ABS* abs, size_t sz)
{
  printf("_resize %u -> %u\n", (abs)?abs->sz:0, sz);
  // nouveau conteneur
  ABS* abs_new = (ABS*)calloc(1,sizeof(ABS));
  if (abs_new == NULL) fprintf(stdout,"ABS_resize calloc 1 failed");
  abs_new->sz   = sz;
  abs_new->arr  = calloc(sz,sizeof(any_t));
  if (abs_new->arr == NULL) fprintf(stdout,"ABS_resize calloc 2 failed");
  // recopie des elem de l'ancien conteneur vers le nouveau s'il existe
  if (abs != NULL) {
    abs_new->hwm  = abs->hwm;
    abs_new->used = abs->used;
    if (abs_new->hwm != MIN_HWM) {
      int i;
      for (i=0;i<=abs_new->hwm;i++) {
        abs_new->arr[i] = abs->arr[i];
      }
    }
    // free de l'ancien conteneur
    ABS_free(&abs);
  }
  else {
    abs_new->hwm  = MIN_HWM;
    abs_new->used = 0;
  }
  {
    // initialisation de toutes les nouvelles entrées du tableau de façon à
    // ce que aucune ne soit NULL
    int i;
    for (i=abs_new->hwm+1;i<abs_new->sz;i++) {
      abs_new->arr[i] = ABS_None;
    }
  }
  return abs_new;
}


///////////////////////////////////////////////////////////////////////////////
ABS* ABS_new(void)
{
  ABS* abs = _resize(NULL,ABS_INIT_SZ);
  return abs;
}


///////////////////////////////////////////////////////////////////////////////
void ABS_free(ABS** abs)
{
  if ((*abs) != NULL && (*abs)->arr != NULL) free ((*abs)->arr);
  if ((*abs) != NULL) free (*abs);
  *abs = NULL;
}


///////////////////////////////////////////////////////////////////////////////
ABS* ABS_set(ABS* abs, size_t ind, any_t elem)
{
  // on ne stocke pas les nouveaux elem vides
  if (elem != NULL && elem != ABS_None) {
    ABS* abs_new = abs;
    while (abs_new->sz <= ind) abs_new = _resize(abs_new,abs_new->sz*2);
    abs_new->arr[ind] = elem;
    if (abs_new->hwm == MIN_HWM || ind > abs_new->hwm) abs_new->hwm = ind;
    abs_new->used++;
    return abs_new;
  }
  else {
    return abs;
  }
}


///////////////////////////////////////////////////////////////////////////////
ABS* ABS_app(ABS* abs, any_t elem)
{
  return ABS_set(abs, abs->hwm+1, elem);
}


///////////////////////////////////////////////////////////////////////////////
ABS* ABS_rm(ABS* abs, size_t ind)
{
  // on ne supprime pas un elem déjà vides
  if (ind <= abs->hwm && abs->arr[ind] != ABS_None) {
    abs->used--;
    abs->arr[ind] = ABS_None;
    // calcul du nouveau hwm, si on vient de supprimer l'elem de plus grand indice
    if (abs->hwm != MIN_HWM && ind == abs->hwm) {
      while (abs->arr[abs->hwm] == ABS_None && abs->hwm != MIN_HWM) {
        abs->hwm--;
      }
    }
  }
  printf("abs->hwm = %u, abs->sz/4 = %u\n", abs->hwm, abs->sz/4);
  ABS* abs_new = abs;
  while (abs_new->hwm <= abs_new->sz/4) abs_new = _resize(abs_new,abs_new->sz/2);
  return abs_new;
}


///////////////////////////////////////////////////////////////////////////////
// retourne le plus grand indice occupé +1, donc la longueur effective de la sequence
size_t ABS_len(ABS* abs)
{
  return abs->hwm+1;
}


///////////////////////////////////////////////////////////////////////////////
// retourne le nb de cellules disponibles en mémoire
size_t ABS_sz(ABS* abs)
{
  return abs->sz;
}


///////////////////////////////////////////////////////////////////////////////
// retourne le nb d'entrées non None
size_t ABS_nb_not_None(ABS* abs)
{
  return abs->used;
}


///////////////////////////////////////////////////////////////////////////////
ABS* ABS_deepcopy(ABS* abs)
{
  ABS* abs_new = _resize(NULL,abs->sz);
  int i;
  for (i=0;i<abs->sz;i++) {
    if (abs->arr != NULL) {
        size_t szelem = sizeof(*abs->arr[i]);
        abs_new->arr[i] = calloc(1,szelem);
        memcpy(abs_new->arr[i], abs->arr[i],szelem);
    }
  }
  return abs_new;
}

///////////////////////////////////////////////////////////////////////////////
// retourne l'element stocké à la valeur de l'indice
any_t ABS_get_elem(ABS* abs, size_t ind)
{
  if (abs->hwm != MIN_HWM && ind <= abs->hwm) {
    any_t elem = abs->arr[ind];
    return elem;
  }
  else {
    return ABS_None;
  }
}
