#ifndef LRUBuf_h
#define LRUBuf_h

// author: emmanuel_barillot
// creation: 2016-11
// goal: module de gestion d'un buffer qui est un tableau de lignes, toutes de même taille
//
//
//

#ifndef __LRUBuf_C__
// la variable n'est déclarée externe que en dehors de LRUBuf.c
// elle permet à un programme qui est interrompu par le module Erreur (kill SIGUSR1) de savoir
// que c'est bien ce module qui a un pb de place mémoire.
extern int  __LRUBuf_for_SIGUSR1_flag__ ;
extern char __LRUBuf_for_SIGUSR1_msg__[] ;
#endif


typedef size_t LRUBufId;
#define DEFAULT_LRUBufId  (LRUBufId) 0
#define N_LRUBufId        (LRUBufId) 100      // nb de buffers possibles



// buffers LRU dynamiques: taille de chaque ligne et nb de ligne alloués au départ
// plusieurs buffers peuvent être gérés; repérés chacun par leur id

// nettoyage du buffer statique
void LRUBuf_clean(LRUBufId id);
// initialisation (allocation) d'un buffer LRU
LRUBufId LRUBuf_init(unsigned short nlines, unsigned short szline);
// libération d'un LRUBuf
void LRUBuf_free(LRUBufId id);
// retourne les dimensions du buffer LRU statique par défaut
size_t  LRUBuf_get_nlines(LRUBufId id);
size_t  LRUBuf_get_lineSize(LRUBufId id);
// retourne l'adresse d'une ligne libre dans le buffer LRU indiqué
char*   LRUBuf_get(LRUBufId id);


#endif
