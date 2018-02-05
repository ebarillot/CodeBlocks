#ifndef _Pmap_struct_H
#define _Pmap_struct_H

#include "Pmap_const.h"


// un numero par type :
// commence à 0 pour pouvoir utiliser ce numéro comme indice de tableau
enum Pmaptype { Pmap_notype     = -1,
                Pmap_short      =  0,
                Pmap_int        =  1,
                Pmap_long       =  2,
                Pmap_longlong   =  3,
                Pmap_ulong      =  4,
                Pmap_ulonglong  =  5,
                Pmap_double     =  6,
                Pmap_charp      =  7,
                Pmap_voidp      =  8};
typedef short Pmaptype;



typedef union _Pmapval
{
  short               si    ;
  int                 i     ;
  long                l     ;
  long long           ll    ;
  unsigned long       ul    ;
  unsigned long long  ull   ;
  double              d     ;
  char*               s     ;
  void*               p     ;
} Pmapval;

// Macros pour manipuler une Propval sans entrer dans le détail de la structure
#define Pmapval_to_short(pv,val)        (pv.si=(short)val)
#define Pmapval_to_int(pv,val)          (pv.i=(int)val)
#define Pmapval_to_long(pv,val)         (pv.l=(long)val)
#define Pmapval_to_longlong(pv,val)     (pv.ll=(long long)val)
#define Pmapval_to_ulong(pv,val)        (pv.ul=(unsigned long)val)
#define Pmapval_to_ulonglong(pv,val)    (pv.ull=(unsigned long long)val)
#define Pmapval_to_double(pv,val)       (pv.d=(double)val)
#define Pmapval_to_char(pv,ptr)         (pv.s=(char*)ptr)
#define Pmapval_to_voidp(pv,ptr)        (pv.p=(void*)ptr)

#define Pmapval_from_short(pv,val)      (val=(short)pv.si)
#define Pmapval_from_int(pv,val)        (val=(int)pv.i)
#define Pmapval_from_long(pv,val)       (val=(long)pv.l)
#define Pmapval_from_longlong(pv,val)   (val=(long long)pv.ll)
#define Pmapval_from_ulong(pv,val)      (val=(unsigned long)pv.ul)
#define Pmapval_from_ulonglong(pv,val)  (val=(unsigned long long)pv.ull)
#define Pmapval_from_double(pv,val)     (val=(double)pv.d)
#define Pmapval_from_char(pv,ptr)       (ptr=(char*)pv.s)
#define Pmapval_from_voidp(pv,ptr)      (ptr=(void*)pv.p)


// les associations type - format d'affichage
// les formats sont impérativement dans l'ordre des types déclarés ci dessus
// Au besoin, il faut augmenter le nb de formats disponibles
#define N_Pmap_formats    10
typedef struct _Pmaptypefmt
{
  char      f[N_Pmap_formats];
} Pmaptypefmt;
static Pmaptypefmt pmaptypefmt[N_Pmap_formats] =
{
    {.f = "%d"   }
  , {.f = "%d"   }
  , {.f = "%ld"  }
  , {.f = "%lld" }
  , {.f = "%uld" }
  , {.f = "%ulld"}
  , {.f = "%.3g" }
  , {.f = "0x%x" }
  , {.f = "0x%x" }
};


typedef struct _Pmapelm
{
  char      n[L_Pmap_name+1]  ;   // nom du champ + indicateur d'entrée utilisée
  Pmaptype  t                 ;   // type = contient le n° du champ dans la structure qui suit (meme ordre que dans l'enum)
  Pmapval   v                 ;
  void      (*free_p)(void*)  ; // fonction de desallocation unitaire pour les données de type pointeur
} Pmapelm;

typedef struct _Pmaps
{
  int       nbAlloc   ;   // nb alloués
  int       nbUsed    ;   // nb utilisés (trous déduits)
  int       hwm       ;   // high water mark : indice max utilisé
  Pmapelm*  tab       ;   // tableau de Pmapelm
} Pmap;


#endif
