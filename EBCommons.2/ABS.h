#ifndef ABS_H_INCLUDED
#define ABS_H_INCLUDED

#include <stdlib.h>

// ABS : array based sequence of adresses

typedef void* any_t;

struct _ABS {
  size_t  sz;     // taille r�serv�e pour le tableau arr
  size_t  hwm;    // le plus grand indice utilis� (high water mark) => "longueur = hwm+1"
  size_t  used;   // le nb d'entr�es utilis�es (not ABS_None)
  any_t*  arr;    // le tableau des pointeurs
};
typedef struct _ABS ABS;

extern any_t ABS_None;  // impl�mentation du ABS_None

#define ABS_INIT_SZ  1024

// cr�ation d'un nouvel ArrayBasedSequence
ABS* ABS_new(void);
// effacement de la m�moire d'un ArrayBasedSequence
void ABS_free(ABS** abs);
// ajout d'un �lement � la fin de la sequence
ABS* ABS_app(ABS* abs, any_t elem);
// ajout ou remplacement d'un �lement � l'indice ind
ABS* ABS_set(ABS* abs, size_t ind, any_t elem);
// suppression de l'�lement � l'indice ind
// mais l'espace memoire de l'element n'est pas supprim�; � charge de l'appelant
ABS* ABS_rm(ABS* abs, size_t ind);
// retourne le plus grand indice occup� +1, donc la lnogueur effective de la sequence
size_t ABS_len(ABS* abs);
// retourne le nb de cellules disponibles en m�moire
size_t ABS_sz(ABS* abs);
// retourne le nb d'entr�es non None
size_t ABS_nb_not_None(ABS* abs);
// copie profonde de toute la sequence, �lements compris, dans la mesure
// o� leur espace m�moire est mesurable avec sizeof()
ABS* ABS_deepcopy(ABS* abs);

// retourne l'�l�ment stock� dans la s�quence � l'indice ind
any_t ABS_get_elem(ABS* abs, size_t ind);


#endif // ABS_H_INCLUDED
