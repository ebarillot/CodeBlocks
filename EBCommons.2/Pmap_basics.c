
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> /* pour g�rer les va_list */
#include <exploit.h>  /* pour CODE_FIN_ERREUR_APPLICATIVE */

#include "Erreur.h"
#include "inout.h"
#include "Utile.h"
#include "Message.h"

#include "Pmap_basics.h"


////////////////////////////////////////////////////////////////////////
// allocation
int Pmapelm_new  (Pmapelm** pmapelm, void(*free_p)(void* ptr))
{
  int   ret = 0;
  IN();
  *pmapelm = (Pmapelm*) malloc(sizeof(Pmapelm));
  if (*pmapelm == NULL) {
    Message_print(LOG, "%s() : Impossible d'allouer", nomFonction);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  Message_printDebugMem(TAG_MALLOC,*pmapelm,1,sizeof(Pmapelm),sizeof(Pmapelm));
  if (free_p !=  NULL) (*pmapelm)->free_p = free_p;
  else  (*pmapelm)->free_p = free; // la fonction standard
 XIT:
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// d�allocation
int Pmapelm_free (Pmapelm** pmapelm)
{
  IN();
  int   ret = 0;
  Message_printDebug(TAG_FREE,"%s(), %s : %p",nomFonction,"*pmapelm",*pmapelm);
  if (*pmapelm)
  {
    Pmapelm_clean (*pmapelm);
    free(*pmapelm);
  }
  *pmapelm = NULL;
 XIT:
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// nettoyage : avec la fonction specifique si elle existe,
// sinon aucune action sur la valeur
int Pmapelm_clean (Pmapelm* pmapelm)
{
  IN();
  int   ret = 0;
  Message_printDebug(TAG_FREE,"%s(), %s : %p",nomFonction,"pmapelm->v.p",pmapelm->v.p);
  if (pmapelm)
  {
    if  (   (pmapelm->t == Pmap_voidp || pmapelm->t == Pmap_charp)
          && pmapelm->free_p != NULL
          && pmapelm->v.p    != NULL
        )
    {
      pmapelm->free_p(pmapelm->v.p);
      pmapelm->v.p = NULL;
    }
    else
    {
      pmapelm->v.ll = 0;
    }
    memset (pmapelm->n, 0, L_Pmap_name+1);
    pmapelm->t = Pmap_notype;
    pmapelm->free_p = NULL;
  }
 XIT:
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// definition
int Pmapelm_set (Pmapelm* pmapelm, char* name, Pmaptype t, Pmapval v, void (*free_p)(void*))
{
  int   ret = 0;
  IN();
  if (NULL == pmapelm) {
    Erreur_set (CLA_APP, nomFonction, "pmapelm est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (name == NULL) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (strlen(name) >= L_Pmap_name) {
    Erreur_set (CLA_APP, nomFonction, "nom trop long");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  strcpy(pmapelm->n, name);
  pmapelm->t = t;
  pmapelm->v = v;
  // si l'argument est NULL, on n'ecrase pas la valeur pr�sente
  if (free_p != NULL) pmapelm->free_p = free_p;
 XIT:
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// restitution : name, type et valeur d'une pmapelm
int Pmapelm_get (Pmapelm* pmapelm, char* name, Pmaptype* t, Pmapval* v)
{
  int   ret = 0;
  IN();
  if (NULL == pmapelm) {
    Erreur_set (CLA_APP, nomFonction, "pmapelm est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (name == NULL) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  strcpy (name, pmapelm->n);
  *t = pmapelm->t;
  *v = pmapelm->v;
 XIT:
  OUT(ret);
  return ret;
}


////////////////////////////////////////////////////////////////////////
// test du type
int Pmapelm_hasType (Pmapelm* pmapelm, Pmaptype t, int* yes)
{
  int   ret = 0;
  IN();
  if (NULL == pmapelm) {
    Erreur_set (CLA_APP, nomFonction, "pmapelm est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  *yes = (t == pmapelm->t)? 1 : 0 ;
 XIT:
  OUT(ret);
  return ret;
}



////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//
// COLLECTION
//
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// allocation
int Pmap_new  (Pmap** pmap, int nbAlloc)
{
  int   ret = 0;
  IN();
  *pmap = (Pmap*) malloc(sizeof(Pmap));
  if (*pmap == NULL) {
    Message_print(LOG, "%s() : Impossible d'allouer pmap", nomFonction);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  Message_printDebugMem(TAG_MALLOC,*pmap,1,sizeof(Pmap),sizeof(Pmap));
  if (nbAlloc <= 0)
  {
    nbAlloc = N_Pmap_default;
  }
  (*pmap)->tab     = (Pmapelm*) malloc(sizeof(Pmapelm)*nbAlloc)  ;   // tableau de Pmapelm
  if ((*pmap)->tab == NULL) {
    Message_print(LOG, "%s() : Impossible d'allouer pmap->tab", nomFonction);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  Message_printDebugMem(TAG_MALLOC,(*pmap)->tab,nbAlloc,sizeof(Pmapelm),sizeof(Pmapelm)*nbAlloc);
  (*pmap)->nbAlloc = nbAlloc ;   // nb allou�s
  (*pmap)->nbUsed  = 0       ;   // nb utilis�s
  (*pmap)->hwm     = 0       ;   // high water mark : indice max utilis�
 XIT:
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// extension du tableau en memoire avec un paquet supplementaire de
int Pmap_extend (Pmap* pmap)
{
  IN();
  int   ret       = 0;
  int   nbAlloc   = pmap->nbAlloc+N_Pmap_extend; // extension lineaire
  //int   nbAlloc   = pmap->nbAlloc *2 ; // extension geometrique
  pmap->tab      = (Pmapelm*) realloc(pmap->tab, sizeof(Pmapelm)*nbAlloc)  ;   // tableau de Pmapelm
  if (pmap->tab == NULL) {
    Message_print(LOG, "%s() : Impossible d'etendre pmap->tab", nomFonction);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  pmap->nbAlloc = nbAlloc ;   // nb allou�s
  // nbUsed et hwm ne changent pas
 XIT:
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// d�sallocation de toute la collection
int Pmap_free (Pmap** pmap)
{
  int   ret = 0;
  IN();
  if (pmap == NULL) {
    Message_print(LOG, "%s() : pmap est NULL", nomFonction);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (*pmap == NULL) {
    Message_print(LOG, "%s() : *pmap est NULL", nomFonction);
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  // desallocation individuelle des valeurs
  int i;
  for (i=0;i<(*pmap)->nbAlloc;i++)
  {
    if ((*pmap)->tab[i].n[0] != 0x00) // si la place est occup�e
    {
      if ((ret = Pmapelm_clean (&(*pmap)->tab[i]) != 0)) goto XIT;
    }
  }
  // desallocation du conteneur (table) des valeurs
  Message_printDebug(TAG_FREE,"%s(), %s : %p",nomFonction,"(*pmap)->tab",(*pmap)->tab);
  if ((*pmap)->tab != NULL) free ((*pmap)->tab);
  // desallocation du conteneur pmap
  Message_printDebug(TAG_FREE,"%s(), %s : %p",nomFonction,"*pmap",*pmap);
  if (*pmap) free(*pmap);
  *pmap = NULL;
 XIT:
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// nb de places disponibles dans la collection
int Pmap_getNbAlloc (Pmap* pmap)
{
  IN();
  int   ret = 0;
  if (NULL == pmap) {
    Erreur_set (CLA_APP, nomFonction, "pmap est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
 XIT:
  OUT(ret);
  return pmap->nbAlloc;
}

////////////////////////////////////////////////////////////////////////
// nb utils�s dans la collection
int Pmap_getNbUsed (Pmap* pmap)
{
  IN();
  int   ret = 0;
  if (NULL == pmap) {
    Erreur_set (CLA_APP, nomFonction, "pmap est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
 XIT:
  OUT(ret);
  return pmap->nbUsed;
}

////////////////////////////////////////////////////////////////////////
// high water mark de la collection
int Pmap_getHwm (Pmap* pmap)
{
  IN();
  int   ret = 0;
  if (NULL == pmap) {
    Erreur_set (CLA_APP, nomFonction, "pmap est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
 XIT:
  OUT(ret);
  return pmap->hwm;
}

////////////////////////////////////////////////////////////////////////
// insertion
int Pmap_put (Pmap* pmap, char* name, Pmaptype t, Pmapval v, int* idx, void (*free_p)(void*))
{
  IN();
  int   ret  = 0;
  int   lidx = -1;
  if (NULL == pmap) {
    Erreur_set (CLA_APP, nomFonction, "prop est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (name == NULL) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (strlen(name) >= L_Pmap_name) {
    Erreur_set (CLA_APP, nomFonction, "nom trop long");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  // on cherche un element avec le m�me nom
  if ((ret = Pmap_getIdxByName(pmap, name, &lidx)) != 0) goto XIT;
  // s'il n'existe pas, on cherche la 1ere place vide
  if (-1 == lidx)
  {
    if ((ret = Pmap_getNextFreeIdx (pmap, &lidx)) != 0) goto XIT;
    if (-1 == lidx) {
      Erreur_set (CLA_SYS, nomFonction, "impossible de trouver une place");
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }
  else  // si la place est d�j� prise, on libere d'abord la place proprement
  {
    if ((ret = Pmapelm_clean (&pmap->tab[lidx])) != 0) goto XIT;
    pmap->nbUsed -=1;
    if (lidx == pmap->hwm) { // on descend le hwm le plus bas possible
      while (pmap->hwm>0 && pmap->tab[pmap->hwm].n[0] == 0x00) { pmap->hwm -=1; }
    }
  }
  ret = Pmapelm_set (&pmap->tab[lidx], name, t, v, free_p);
  pmap->nbUsed += 1;
  if (lidx > pmap->hwm) pmap->hwm = lidx;

 XIT:
  if (idx) *idx = lidx;
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// suppression d'une entr�e dans la collection
int Pmap_remove (Pmap* pmap, char* name)
{
  IN();
  int   ret  = 0;
  int   lidx = -1;
  if (NULL == pmap) {
    Erreur_set (CLA_APP, nomFonction, "prop est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (name == NULL) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (strlen(name) >= L_Pmap_name) {
    Erreur_set (CLA_APP, nomFonction, "nom trop long");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if ((ret = Pmap_getIdxByName(pmap, name, &lidx)) != 0) goto XIT;
  if (lidx != -1)
  {
    ret = Pmapelm_clean (&pmap->tab[lidx]);
    pmap->nbUsed -=1;
    if (lidx == pmap->hwm) { // on descend le hwm le plus bas possible
      while (pmap->hwm>0 && pmap->tab[pmap->hwm].n[0] == 0x00) { pmap->hwm -=1; }
    }
  }
 XIT:
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// retourne l'index d'un element de la collection � partir de son nom
int Pmap_getIdxByName (Pmap* pmap, char* name, int* idx)
{
  IN();
  int   ret = 0;
  int   lidx = -1;
  if (NULL == pmap) {
    Erreur_set (CLA_APP, nomFonction, "prop est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (NULL == name) {
    Erreur_set (CLA_APP, nomFonction, "name est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (pmap->hwm<=0) {
    lidx = -1;
  }
  else {
    int i;
    // boucle basique: on compare chaque cl� de la Pmap avec le parametre name
    // TODO: introduire une fonction de hachage
    for (i=0;i<=pmap->hwm;i++)
    {
      if (strcmp(pmap->tab[i].n, name) == 0) {lidx = i; break;}
    }
  }
 XIT:
  if (ret != 0) lidx = -1;
  if (idx) *idx = lidx;
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// retourne l'index du premier element vide
// etend la collection si necessaire
int Pmap_getNextFreeIdx (Pmap* pmap, int* idx)
{
  IN();
  int   ret = 0;
  int   lidx = -1;
  if (NULL == pmap) {
    Erreur_set (CLA_APP, nomFonction, "prop est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (NULL == idx) {
    Erreur_set (CLA_APP, nomFonction, "idx est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  // on cherche la 1ere entr�e vide
  lidx = -1;
  int i;
  for (i=0;i<=pmap->hwm;i++)   // attention au <=
  {
    if (pmap->tab[i].n[0] == 0x00) {lidx = i; break;}
  }
  // aucune place vide ou tout est vide
  if (lidx == -1)
  {
    if ((pmap->hwm+1) >= pmap->nbAlloc)
    { // plus assez de place memoire => on etend
      if ((ret = Pmap_extend (pmap)) != 0) goto XIT;
    }
    lidx = pmap->hwm+1;  // c'est bien la prochaine place vide
  }
 XIT:
  if (ret != 0) lidx = -1;
  if (idx) *idx = lidx;
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// restitution de la valeur � partir de son indice
int Pmap_getValByIdx (Pmap* pmap, int idx, char* name, Pmaptype* t, Pmapval* v)
{
  IN();
  int   ret = 0;
  if (NULL == pmap)
  {
    Erreur_set (CLA_APP, nomFonction, "prop est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (idx > pmap->hwm)
  {
    Erreur_set (CLA_APP, nomFonction, "indice inconnu");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  // pour faire simple, on parcourt le tableau jusqu'� tomber sur le bon indice
  strcpy (name, pmap->tab[idx].n);
  *t = pmap->tab[idx].t;
  *v = pmap->tab[idx].v;
 XIT:
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// restitution de la valeur � partir de son nom
int Pmap_getValByName (Pmap* pmap, char* name, Pmaptype* t, Pmapval* v, int* idx)
{
  IN();
  int   ret = 0;
  int   lidx = -1;
  if (NULL == pmap) {
    Erreur_set (CLA_APP, nomFonction, "prop est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if ((ret = Pmap_getIdxByName(pmap, name, &lidx)) != 0) goto XIT;
  if (lidx >= 0) {
    strncpy (name, pmap->tab[lidx].n, L_Pmap_name);
    *t = pmap->tab[lidx].t;
    *v = pmap->tab[lidx].v;
  }
  else
  {
    if (idx == NULL)  // on ne peut pas signaler "non trouv�" par l'indice
    {
      Erreur_set (CLA_APP, nomFonction, "valeur \"%s\" non trouv�e", name);
      ret = CODE_FIN_ERREUR_APPLICATIVE;
      goto XIT;
    }
  }
 XIT:
  if (ret != 0) lidx = -1;
  if (idx) *idx = lidx;
  OUT(ret);
  return ret;
}

////////////////////////////////////////////////////////////////////////
// affichage
int Pmap_print (Pmap* pmap)
{
  int   ret = 0;
  char* padding = "...........................................................";
  IN();
  if (NULL == pmap) {
    Erreur_set (CLA_APP, nomFonction, "pmap est NULL");
    ret = CODE_FIN_ERREUR_APPLICATIVE;
    goto XIT;
  }
  if (pmap->nbUsed >= 0)
  {
    int i;
    int lmaxName = 0;
    int lName = 0;
    for (i=0;i<=pmap->hwm;i++)
    {
      lName = strlen(pmap->tab[i].n);
      lmaxName = (lmaxName < lName) ? lName : lmaxName;
    }
    Message_print (LOG, "[%d] valeurs", pmap->nbUsed);
    for (i=0;i<=pmap->hwm;i++)
    {
      lName = strlen(pmap->tab[i].n);
      if (lName > 0)  // on affiche que les entr�es occup�es
      {
        char  format[100]; memset(format,0,100);
        sprintf (format, "[%%2d] %%s %%.*s : %s", pmaptypefmt[pmap->tab[i].t]);
        Message_print (LOG, format, i, pmap->tab[i].n, 2+lmaxName-lName, padding, pmap->tab[i].v);
      }
    }
  }
 XIT:
  OUT(ret);
  return ret;
}

