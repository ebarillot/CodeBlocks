#ifndef ABS_H_INCLUDED
#define ABS_H_INCLUDED

#include <stdlib.h>

// ABS : array based sequence of adresses

typedef void* any_t;

struct _ABS {
  size_t  sz;     // taille réservée pour le tableau arr
  size_t  hwm;    // le plus grand indice utilisé (high water mark) => "longueur = hwm+1"
  size_t  used;   // le nb d'entrées utilisées (not ABS_None)
  any_t*  arr;    // le tableau des pointeurs
};
typedef struct _ABS ABS;

extern any_t ABS_None;  // implémentation du ABS_None

#define ABS_INIT_SZ  1024

// création d'un nouvel ArrayBasedSequence
ABS* ABS_new(void);
// effacement de la mémoire d'un ArrayBasedSequence
void ABS_free(ABS** abs);
// ajout d'un élement à la fin de la sequence
ABS* ABS_app(ABS* abs, any_t elem);
// ajout ou remplacement d'un élement à l'indice ind
ABS* ABS_set(ABS* abs, size_t ind, any_t elem);
// suppression de l'élement à l'indice ind
// mais l'espace memoire de l'element n'est pas supprimé; à charge de l'appelant
ABS* ABS_rm(ABS* abs, size_t ind);
// retourne le plus grand indice occupé +1, donc la lnogueur effective de la sequence
size_t ABS_len(ABS* abs);
// retourne le nb de cellules disponibles en mémoire
size_t ABS_sz(ABS* abs);
// retourne le nb d'entrées non None
size_t ABS_nb_not_None(ABS* abs);
// copie profonde de toute la sequence, élements compris, dans la mesure
// où leur espace mémoire est mesurable avec sizeof()
ABS* ABS_deepcopy(ABS* abs);

// retourne l'élément stocké dans la séquence à l'indice ind
any_t ABS_get_elem(ABS* abs, size_t ind);


#endif // ABS_H_INCLUDED
